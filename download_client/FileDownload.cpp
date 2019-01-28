#include "FileDownload.h"

//QString product2 = "DTS381800001";
//QString cam_serial12 = "15137628";
//QString cam_serial22 = "15322940";
int code2;
int cancel2 = 0;
int progressValue2;
int abortNotice2 = 0;

FileDownload::FileDownload(QObject *parent)
	: QObject(parent)
{
	
}

FileDownload::~FileDownload()
{

}

int FileDownload::existsCheck(QString product, QString cam1, QString cam2, QString marginal = "marginal")
{
	/// temp 파일도 체크후 잇으면 삭제해버리기
	int code = 0;
	QString fileName1 = "calibration_data_cam" + cam1 + ".cal";
	QString fileName2 = "calibration_data_cam" + cam2 + ".cal";

	QDir dir;
	QString downloadDirectory = QDir::currentPath() + "/../../cal";



	if (downloadDirectory.isEmpty() || !QFileInfo(downloadDirectory).isDir())
		dir.mkdir(downloadDirectory + "/../../cal");
	//directory check & make directory
	bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();
	if (useDirectory)
	{
		fileName1.prepend(downloadDirectory + '/');
		fileName2.prepend(downloadDirectory + '/');
	}

	if (!QFile::exists(fileName1) && !QFile::exists(fileName2))
		code = 3; //both cam serial numbers are not exist 
	else if (!QFile::exists(fileName1))
		code = 1;//cam1 serial number is not exist
	else if (!QFile::exists(fileName2))
		code = 2;//cam2 serial number is not exist
	qDebug(code + "\n******************************************");
	return code;

}
