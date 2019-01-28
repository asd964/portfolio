#include "FileDownloadDialog.h"				






FileDownloadDialog::FileDownloadDialog(QDialog *parent)
	: QDialog(parent)
	, reply(nullptr)
	, file(nullptr)
	, httpRequestAborted(false)
{
	setWindowModality(Qt::WindowModal);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowTitle(tr("HTTP"));
	ui.setupUi(this);
	
}
FileDownloadDialog::~FileDownloadDialog()
{
}

void FileDownloadDialog::on_btnOK_clicked()
{	
	QString fileName1 = "calibration_data_cam" + cam_serial1 + ".cal";
	QString fileName2 = "calibration_data_cam" + cam_serial2 + ".cal";

	QDir dir;
	QString downloadDirectory = QDir::currentPath() + "/../../cal";

	bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();
	if (useDirectory)
	{
		fileName1.prepend(downloadDirectory + '/');
		fileName2.prepend(downloadDirectory + '/');
	}

	if (manuallyDownload == true)
	{
		if (QMessageBox::question(this, tr("Overwrite Existing File"),
			tr("There already exists a file called %1%2%3."
			" Overwrite?")
			.arg(fileName1, fileName2,
			useDirectory
			? QString()
			: QStringLiteral(" in the current directory")),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No)
			== QMessageBox::No) {
			return;
		}

	}
	

	if (networkChecker->isConnectedToNetwork()){
		FileDownloadDialog::downloadStart();
		ui.btnOK->setEnabled(false);

	}
	else
	{
		cancelNotice = 0;
		FileDownloadDialog::networkDisconnection(); 
	}
	
}

int FileDownloadDialog::existsCheck(QString product, QString cam1, QString cam2, QString marginal="marginal")
{
		int code=0;
		QString fileName1 = "calibration_data_cam" + cam1 + ".cal";
		QString fileName2 = "calibration_data_cam" + cam2 + ".cal";

		QDir dir;
		QString downloadDirectory = QDir::currentPath() + "/../../cal";
		QString tempDirectory = QDir::currentPath() + "/../../cal/temp";
		if (downloadDirectory.isEmpty() || !QFileInfo(downloadDirectory).isDir())
			dir.mkdir(downloadDirectory + "/../../cal");
		if (tempDirectory.isEmpty() || !QFileInfo(tempDirectory).isDir())
			dir.mkdir(tempDirectory + "/../../cal/temp");
		bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();
		if (useDirectory)
		{
			fileName1.prepend(downloadDirectory + '/');
			fileName2.prepend(downloadDirectory + '/');
		}

		
		
		if (manuallyDownload == true)
			return 3;
		
		if (!QFile::exists(fileName1) && !QFile::exists(fileName2))
			code = 3; //both cam serial numbers are not exist 
		else if (!QFile::exists(fileName1))
			code = 1;//cam1 serial number is not exist
		else if (!QFile::exists(fileName2))		
		     code =2;//cam2 serial number is not exist
		return code;
}

void FileDownloadDialog::downloadFile(QString product, QString cam1,QString cam2, int code)
{
	QString urlSpec;

	urlSpec = "http://192.168.10.2:8000/Home/DownloadFile?SerialN1=" + cam1 +
		"&product="
		+ product + "&marginal=empty";
	if (urlSpec.isEmpty())
		return;
	const QUrl newUrl = QUrl::fromUserInput(urlSpec);
	if (!newUrl.isValid()) {
		QMessageBox::information(this, tr("Error"),
			tr("Invalid URL: %1: %2").arg(urlSpec, newUrl.errorString()));
		return;
	}
	QString fileName = "calibration_data_cam" + cam1 + ".temp";
	QString downloadDirectory = QDir::currentPath() + "/../../cal/temp";
	bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();
	QDir dir;
	if (useDirectory)
		fileName.prepend(downloadDirectory + '/');
	file = openFileForWrite(fileName);
	if (!file)
		return;
	FileDownloadDialog::startRequest(newUrl);
}

QFile *FileDownloadDialog::openFileForWrite(const QString &fileName)
{
	QScopedPointer<QFile> file(new QFile(fileName));
	if (!file->open(QIODevice::ReadWrite)) {
		QMessageBox::information(this, tr("Error"),
			tr("Unable to save the file %1: %2.")
			.arg(QDir::toNativeSeparators(fileName),
			file->errorString()));
		return nullptr;
	}									
	
	return file.take();
}

void FileDownloadDialog::httpFinished()
{

	bool JsonCheck = true;
	QFile *readFile = new QFile();
	QString readFileName;
	QString downloadDirectory = QDir::currentPath() + "/../../cal/temp";
	bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();



	
 	QFileInfo fi;
	
	if (httpRequestAborted) {
		reply->deleteLater();
		reply = nullptr;
		/*return;*/
	}



	ui.statusLabel->setText(tr("Downloaded %1 bytes to %2\nin\n%3")
		.arg(fi.size()).arg(fi.fileName(), QDir::toNativeSeparators(fi.absolutePath())));
	
	if (code == 1 || code == 3)
	{
		readFileName = QDir::currentPath() + "/../../cal/temp/" + "calibration_data_cam" + cam_serial1 + ".temp";
	}
	else if (code == 2 || code == 4)
	{
		readFileName = QDir::currentPath() + "/../../cal/temp/" + "calibration_data_cam" + cam_serial2 + ".temp";
	}
	readFile->setFileName(readFileName);
	readFile->open(QIODevice::ReadOnly);
	QTextStream read(readFile);
	QString json_str = read.readAll();
	readFile->close();
	QByteArray json_bytes = json_str.toLocal8Bit();
	QJsonDocument json_doc = QJsonDocument::fromJson(json_bytes);
	if (json_doc.isNull()) {
		JsonCheck = false; 
	}
	if (!json_doc.isObject()) {
		JsonCheck = false;
	}
	QJsonObject json_obj = json_doc.object();

	if (json_obj.isEmpty()) {
		JsonCheck = false;
	}


	if (cancelNotice == 0)
		if (useDirectory)
			if (!JsonCheck) 
			{
				
				FileDownloadDialog::FileRename();
			}
			else{
				try{
					

					QVariantMap& result = json_obj.toVariantMap();
					result.value("msg");
					QString msg = result.value("msg").toString();
					QString returncode = "ERROR CODE : " + result.value("returnCode").toString();
					QMessageBox::StandardButton ans;
					ans = QMessageBox::question(this, "Error", returncode + msg
						+ "\n download again?",
						QMessageBox::Yes | QMessageBox::Close);


					downloadComplete = 0;
					cancelNotice = 0;
					exitNotice = 0;
					startNotice = 0;
					if (ans == QMessageBox::Yes) {


						FileDownloadDialog::downloadStart();
						return;
					}
					else if (ans == QMessageBox::Close) {
						ui.btnOK->setEnabled(true);
						return;
					}
				}
				catch (exception e){
					QMessageBox::information(this, "error", " download process error", "ok");
					QMessageBox::StandardButton ans;
					ans = QMessageBox::question(this, "Error", " download process error \n download again?",
						QMessageBox::Yes | QMessageBox::Close);
					downloadComplete = 0;
					cancelNotice = 0;
					exitNotice = 0;
					startNotice = 0;
					if (ans == QMessageBox::Yes) {
						FileDownloadDialog::downloadStart();
						return;
					}
					else if (ans == QMessageBox::Close) {
						ui.btnOK->setEnabled(true);
						return;
					}
				}
			}

			if (file) {
				fi.setFile(file->fileName());
				file->close();
				delete file;
				file = nullptr;
			}
			
			if (code == 3&&cancelNotice==0){
				downloadFile(product, cam_serial2, cam_serial1, code);

				code++;
	}
	
	if (downloadComplete == 1 && (code == 1 || code == 2))
	{
		exitNotice = 1;
		QWidget::close();
	}
	if (downloadComplete == 2&& (code == 4||(code == 1 || code == 2)))
	{
		exitNotice = 1;
		QWidget::close();
	}	
}

void FileDownloadDialog::httpReadyRead()
{
	if (file)
		file->write(reply->readAll());
}

void FileDownloadDialog::startRequest(const QUrl &requestedUrl)
{
	url = requestedUrl;
	httpRequestAborted = false;
	reply = qnam.get(QNetworkRequest(url));
	connect(reply, &QNetworkReply::finished, this, &FileDownloadDialog::httpFinished);
	connect(reply, &QIODevice::readyRead, this, &FileDownloadDialog::httpReadyRead);
	connect(reply, &QNetworkReply::downloadProgress, this, &FileDownloadDialog::networkReplyProgress);
	ui.progressBarFile1->setAttribute(Qt::WA_DeleteOnClose);
	ui.statusLabel->setText(tr("Downloading %1...").arg(url.toString()));
}

void FileDownloadDialog::networkReplyProgress(qint64 bytesRead, qint64 totalBytes){
	int progressValue;
	if (code == 3){
		progressValue = bytesRead / 2;
		ui.progressBarFile1->setMaximum(totalBytes);
		ui.progressBarFile1->setValue(progressValue);
	}
	else if (code == 4){
		progressValue = bytesRead / 2 + totalBytes / 2;
		ui.progressBarFile1->setMaximum(totalBytes);
		ui.progressBarFile1->setValue(progressValue);
	}
	else{
		progressValue = bytesRead;
		ui.progressBarFile1->setMaximum(totalBytes);
		ui.progressBarFile1->setValue(progressValue);
	}
	if (bytesRead == totalBytes){
		downloadComplete++;
	}
	
}

void FileDownloadDialog::closeEvent(QCloseEvent *bar){
	QString fileName1 = QDir::currentPath() + "/../../cal/calibration_data_cam" + cam_serial1 + ".cal";
	QString fileName2 = QDir::currentPath() + "/../../cal/calibration_data_cam" + cam_serial2 + ".cal";
	manuallyDownload = false;
	if (exitNotice == 1 ){ // temp 폴더에 둘다 있을떄 로 바꾸기
		if (QFile::exists(fileName1))
			QFile::remove(fileName1);
		if (QFile::exists(fileName2))
			QFile::remove(fileName2);

		QFile::copy(QDir::currentPath() + "/../../cal/temp/" + "calibration_data_cam" + cam_serial1 + ".cal",
			QDir::currentPath() + "/../../cal/" + "calibration_data_cam" + cam_serial1 + ".cal");


		QFile::copy(QDir::currentPath() + "/../../cal/temp/" + "calibration_data_cam" + cam_serial2 + ".cal",
			QDir::currentPath() + "/../../cal/" + "calibration_data_cam" + cam_serial2 + ".cal");
	}

	if (startNotice == 1)
		if (exitNotice == 0)
			if (cancelNotice == 0)
				if (networkChecker->isConnectedToNetwork())
					if (httpRequestAborted==false)
					reply->abort();

	emit endCheck();
	emit dialogExit();
}

void FileDownloadDialog::FileRename(){
	bool rename;
	QDir dir;
	QString fileName1 = QDir::currentPath() + "/../../cal/temp/"+"calibration_data_cam" + cam_serial1 + ".cal";
	QString fileName2 = QDir::currentPath() + "/../../cal/temp/"+"calibration_data_cam" + cam_serial2 + ".cal";
	if (code == 1 || code == 3)
	{
		
		if (QFile::exists(fileName1))
			QFile::remove(fileName1);
		rename = file->rename(fileName1);
		
	}
	else if (code == 2 || code == 4)
	{
		
		if (QFile::exists(fileName2))
			QFile::remove(fileName2);
		rename = file->rename(fileName2);
		
	}
}
void FileDownloadDialog::downloadStart(){

	if (startNotice == 0){
		startNotice = 1;
		cancelNotice = 0;
		httpRequestAborted = 0;
				
		QString str;
		code = existsCheck(product, cam_serial1, cam_serial2, "marginal");

		if (manuallyDownload == true&&code==0)
		{
			code = 3;
			downloadFile(product, cam_serial1, cam_serial2, code);
			ui.progressBarFile1->setVisible(true);
			ui.lblFile1->setVisible(true);
			str = cam_serial1 + " calibration data \n" + cam_serial2 + " calibration data";
			ui.lblFile1->setText(str);
		}
		else if (code == 1){
			downloadFile(product, cam_serial1, cam_serial2, code);
			ui.progressBarFile1->setVisible(true);
			ui.lblFile1->setVisible(true);
			str = cam_serial1 + " calibration data";
			ui.lblFile1->setText(str);
		}
		else if (code == 2){
			downloadFile(product, cam_serial2, cam_serial2, code);
			ui.progressBarFile1->setVisible(true);
			ui.lblFile1->setVisible(true);
			str = cam_serial2 + " calibration data";
			ui.lblFile1->setText(str);
		}
		else if (code == 3)
		{
			downloadFile(product, cam_serial1, cam_serial2, code);
			ui.progressBarFile1->setVisible(true);
			ui.lblFile1->setVisible(true);
			str = cam_serial1 + " calibration data \n" + cam_serial2 + " calibration data";
			ui.lblFile1->setText(str);

		}
		if (code == 0)
			QWidget::close();
		ui.btnOK->setEnabled(false);

	}

}
void FileDownloadDialog::networkDisconnection(){

	ui.progressBarFile1->setValue(0);

	if (cancelNotice == 0){
		if (startNotice == 1){
			cancelNotice = 1;
			/*reply->abort();*/
			
			ui.statusLabel->setText(tr("Download canceled."));
			httpRequestAborted = true;
			reply->abort();
			/*if (reply->error()) {
				ui.statusLabel->setText(tr("Download failed:\n%1.").arg(reply->errorString()));
				reply->deleteLater();
				reply = nullptr;

				return;
			}*/
			/*if (file) {
				
				file->close();
				delete file;
				file = nullptr;
			}*/


			ui.btnOK->setEnabled(true);
			startNotice = 0;
		}
		cancelNotice = 1;
		QMessageBox::information(this, "Network not connected", "The network connection was not established. Please connect and try again.", "close");
	}
	

}

void FileDownloadDialog::setManuality(){
	manuallyDownload = true; 
}