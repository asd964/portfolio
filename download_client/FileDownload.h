#ifndef FILEDOWNLOAD_H
#define FILEDOWNLOAD_H

#include <QObject>
#include <QtWidgets/QMainWindow>
#include <qguiapplication.h>
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

class QFile;
class QLabel;
class QLineEdit;
class QPushButton;
class QNetworkReply;

class FileDownload : public QObject
{
	Q_OBJECT

public:
	FileDownload(QObject *parent=0);
	~FileDownload();
	//void FileDownload::startRequest(const QUrl &requestedUrl);
	int FileDownload::existsCheck(QString product, QString cam1, QString cam2, QString marginal);
	private slots:
	/*void FileDownload::cancelDownload();
	void FileDownload::httpReadyRead();
	void FileDownload::httpFinished();
	void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);*/

private:
	/*void FileDownload::downloadFile(QString product, QString cam1, QString cam2, int code);*/
	

	/*QFile *openFileForWrite(const QString &fileName);*/

	bool httpRequestAborted;
	QUrl url;
	QNetworkAccessManager qnam;
	QNetworkReply *reply;
	QFile *file;
};

#endif // FILEDOWNLOAD_H
