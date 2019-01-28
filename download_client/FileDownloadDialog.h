#ifndef FILEDOWNLOADDIALOG_H
#define FILEDOWNLOADDIALOG_H

#include <QtWidgets/QMainWindow>
#include <qguiapplication.h>
#include <QObject>
#include <qobject.h>
#include <qapplication.h>
#include <QWidget>

#include <qurl.h>
#include <qmessagebox.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <DesktopScanner.h>
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qevent.h>
#include "qdir.h"
#include <NetworkChecker.h>
#include "ui_FileDownloadDialog.h"
#include <qthread.h>
#include <NetworkChecker.h>
#include <qdebug.h>
#include <qdialog.h>
class QFile;
class QLabel;
class QLineEdit;
class QPushButton;
class QNetworkReply;


class FileDownloadDialog : public QDialog
{
	Q_OBJECT

public:
	FileDownloadDialog(QDialog *parent = 0);
	~FileDownloadDialog();
	void FileDownloadDialog::startRequest(const QUrl &requestedUrl);
	int FileDownloadDialog::existsCheck(QString product, QString cam1, QString cam2, QString marginal);
	void setManuality();
private slots:

	void on_btnOK_clicked();
	void httpReadyRead();
	void FileDownloadDialog::httpFinished();
	void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
	void closeEvent(QCloseEvent *bar);
	void downloadStart();
	void networkDisconnection();
private:
	void downloadFile(QString product, QString cam1,QString cam2, int code);
	QFile *openFileForWrite(const QString &fileName);
	void FileRename();
	NetworkChecker* networkChecker = new NetworkChecker();
	Ui::FileDownloadDialog ui;
	QUrl url;
	QNetworkAccessManager qnam;
	QNetworkReply* reply;
	QFile *file;
	 

	QString product = "DTS381800001";
	QString cam_serial1 = "15137628";
	QString cam_serial2 = "15322940";

	
	
	bool httpRequestAborted;
	int code;
	int downloadComplete = 0;
	int cancelNotice = 0;
	int exitNotice = 0;
	int startNotice = 0;
	bool manuallyDownload = false;

signals:
	void endCheck();
	void dialogExit();
};

#endif 
