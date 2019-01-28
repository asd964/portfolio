#pragma warning(disable : 4996)
#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <sstream> 
#include <cstdlib>
#include <string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <time.h>
#include<windows.h>

using namespace std;
using namespace cv;

CvMemStorage* storage = NULL;
CvSeq* contours1 = 0;
CvSeq* contours2 = 0;
CvSeq* contours3 = 0;
CvSeq* contours4 = 0;
CvSeq* contours5 = 0;
CvSeq* contours6 = 0;
CvRect rect;
int countSeat[6] = { 0, 0, 0, 0, 0, 0 };
int checkSeat[6] = { 0, 0, 0, 0, 0, 0 };
int saveSeatdata[6] = { 0, 0, 0, 0, 0, 0 };
String saveSeattime[6] = { "0", "0", "0", "0", "0", "0" };
int save = 0;
int curTime = 999;
String passAns;

#pragma comment(lib, "mysqlcppconn.lib")

const string server = "tcp://127.0.0.1";
const string username = "root";
const string password = "1234";

sql::Driver *driver;
sql::Connection *dbConn;
sql::Statement *stmt;



struct time {
	int ti_hour;
	int ti_min;
	int ti_sec;
};

void gettime(struct time *);


void connect_mysql()
{
	try
	{
		driver = get_driver_instance();
	}
	catch (sql::SQLException e)
	{
		cout << "Couldn't get database driver : " << e.what() << endl;
		system("pause");
		exit(1);
	}
	try {
		dbConn = driver->connect(server, username, password);
	}

	catch (sql::SQLException e)
	{
		cout << "Couldn't connect to database : " << e.what() << endl;
		system("pause");
		exit(1);
	}


}

int main()
{
	string info, timeinfo, sti, smi, ssec;

	struct time sttime;
	int num = 1;

	int flag = 1;
	storage = cvCreateMemStorage(0);
	connect_mysql();
	// 움직임을 검출할 비디오 파일을 포인터 capture에 저장한다.
	//CvCapture *capture = cvCaptureFromFile("tunnel.mpeg");
	//CvCapture *capture = cvCaptureFromFile("car.wmv");
	CvCapture *capture = cvCaptureFromFile("SangSangMoving.mp4");

	if (!capture)    {
		std::cout << "The video file was not found" << std::endl;
		return 0;
	}

	int width = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	CvSize size = cvSize(width, height);

	// Initial background image

	IplImage *bkgImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvGrabFrame(capture);
	IplImage *temp = cvRetrieveFrame(capture);
	cvCvtColor(temp, bkgImage, CV_BGR2GRAY);

	if (bkgImage == NULL)    return -1;

	// cvCreateImage 함수를 사용하여 그레이 스케일로 변환을 위한 grayImage와 차영상을 위한 diffImage를 생성한다.
	IplImage *grayImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *diffImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage *frame = NULL;

	int t = 0;    // frame count
	int nThreshold = 50;

	clock_t begin, end;
	begin = clock();        // 시간설정

	while (1)    {
		// capture로부터 프레임을 획득하여 포인터 frame에 저장한다.
		if (!capture)    exit;

		frame = cvQueryFrame(capture);

		//ROI설정
		//IplImage *frame1 = (IplImage*)cvClone(frame);
		//cvSetImageROI(frame1, cvRect(60, 220, 130, 100));
		// cvCvtColor 함수를 사용하여 cvQueryFrame 함수로 획득한 frame을 그레이 스케일 영상으로 변환하여 grayImage에 저장한다.

		if (!frame){
		system("pause");
		break;
		}

		cvCvtColor(frame, grayImage, CV_BGR2GRAY);
				
		IplImage *grayImage1 = (IplImage*)cvClone(grayImage);
		cvSetImageROI(grayImage1, cvRect(60, 220, 130, 100));
		IplImage *diffImage1 = (IplImage*)cvClone(diffImage);
		cvSetImageROI(diffImage1, cvRect(60, 220, 130, 100));
		IplImage *bkgImage1 = (IplImage*)cvClone(bkgImage);
		cvSetImageROI(bkgImage1, cvRect(60, 220, 130, 100));
		
		IplImage *grayImage2 = (IplImage*)cvClone(grayImage);
		cvSetImageROI(grayImage2, cvRect(430, 100, 130, 100));
		IplImage *diffImage2 = (IplImage*)cvClone(diffImage);
		cvSetImageROI(diffImage2, cvRect(430, 100, 130, 100));
		IplImage *bkgImage2 = (IplImage*)cvClone(bkgImage);
		cvSetImageROI(bkgImage2, cvRect(430, 100, 130, 100));

		IplImage *grayImage3 = (IplImage*)cvClone(grayImage);
		cvSetImageROI(grayImage3, cvRect(185, 128, 120, 100));
		IplImage *diffImage3 = (IplImage*)cvClone(diffImage);
		cvSetImageROI(diffImage3, cvRect(185, 128, 120, 100));
		IplImage *bkgImage3 = (IplImage*)cvClone(bkgImage);
		cvSetImageROI(bkgImage3, cvRect(185, 128, 120, 100));

		//last check point
		IplImage *grayImage4 = (IplImage*)cvClone(grayImage);
		cvSetImageROI(grayImage4, cvRect(325, 325, 120, 100));
		IplImage *diffImage4 = (IplImage*)cvClone(diffImage);
		cvSetImageROI(diffImage4, cvRect(325, 325, 120, 100));
		IplImage *bkgImage4 = (IplImage*)cvClone(bkgImage);
		cvSetImageROI(bkgImage4, cvRect(325, 325, 120, 100));

		IplImage *grayImage5 = (IplImage*)cvClone(grayImage);
		cvSetImageROI(grayImage5, cvRect(290, 80, 50, 40));
		IplImage *diffImage5 = (IplImage*)cvClone(diffImage);
		cvSetImageROI(diffImage5, cvRect(290, 80, 50, 40));
		IplImage *bkgImage5 = (IplImage*)cvClone(bkgImage);
		cvSetImageROI(bkgImage5, cvRect(290, 80, 50, 40));

		IplImage *grayImage6 = (IplImage*)cvClone(grayImage);
		cvSetImageROI(grayImage6, cvRect(413, 30, 58, 60));
		IplImage *diffImage6 = (IplImage*)cvClone(diffImage);
		cvSetImageROI(diffImage6, cvRect(413, 30, 58, 60));
		IplImage *bkgImage6 = (IplImage*)cvClone(bkgImage);
		cvSetImageROI(bkgImage6, cvRect(413, 30, 58, 60));

		// cvAbsDiff 함수로 현재의 입력 비디오 프레임과 그레이 스케일 영상인 grayImage와 배경 영상인 bkgImage와의 차이의 절대값을 계산하여 diffImage에 저장한다.
		//cvAbsDiff(grayImage, bkgImage, diffImage);  original code

		cvAbsDiff(grayImage1, bkgImage1, diffImage1);
		cvAbsDiff(grayImage2, bkgImage2, diffImage2);
		cvAbsDiff(grayImage3, bkgImage3, diffImage3);
		cvAbsDiff(grayImage4, bkgImage4, diffImage4);
		cvAbsDiff(grayImage5, bkgImage5, diffImage5);
		cvAbsDiff(grayImage6, bkgImage6, diffImage6);
		// diffImage에서 0인 화소는 변화가 없는 화소이며, 값이 크면 클수록 배경 영상과의 차이가 크게 일어난 화소이다.
		// cvThreshold 함수를 사용하여 cvThreshold=50 이상인 화소만을 255로 저장하고, 임계값 이하인 값은 0으로 저장한다.
		// 임계값은 실험 또는 자동으로 적절히 결정해야 한다.
		/*
		cvThreshold(diffImage, diffImage, nThreshold, 255, CV_THRESH_BINARY);
		cvErode(diffImage, diffImage, 0, 10);
		cvDilate(diffImage, diffImage, 0, 10);
		*original code
		*/
		cvThreshold(diffImage1, diffImage1, nThreshold, 255, CV_THRESH_BINARY);
		cvErode(diffImage1, diffImage1, 0, 10);
		cvDilate(diffImage1, diffImage1, 0, 10);
		cvFindContours(diffImage1, storage, &contours1);

		cvThreshold(diffImage2, diffImage2, nThreshold, 255, CV_THRESH_BINARY);
		cvErode(diffImage2, diffImage2, 0, 10);
		cvDilate(diffImage2, diffImage2, 0, 10);
		cvFindContours(diffImage2, storage, &contours2);

		cvThreshold(diffImage3, diffImage3, nThreshold, 255, CV_THRESH_BINARY);
		cvErode(diffImage3, diffImage3, 0, 10);
		cvDilate(diffImage3, diffImage3, 0, 10);
		cvFindContours(diffImage3, storage, &contours3);

		cvThreshold(diffImage4, diffImage4, nThreshold, 255, CV_THRESH_BINARY);
		cvErode(diffImage4, diffImage4, 0, 10);
		cvDilate(diffImage4, diffImage4, 0, 10);
		cvFindContours(diffImage4, storage, &contours4);

		cvThreshold(diffImage5, diffImage5, nThreshold, 255, CV_THRESH_BINARY);
		cvErode(diffImage5, diffImage5, 0, 10);
		cvDilate(diffImage5, diffImage5, 0, 10);
		cvFindContours(diffImage5, storage, &contours5);

		cvThreshold(diffImage6, diffImage6, nThreshold, 255, CV_THRESH_BINARY);
		cvErode(diffImage6, diffImage6, 0, 10);
		cvDilate(diffImage6, diffImage6, 0, 10);
		cvFindContours(diffImage6, storage, &contours6);

		if (contours1 == 0)
		{
			countSeat[0] = 0;
			checkSeat[0] = 0;
		}
		else{
			countSeat[0]++;
		}
		if (countSeat[0] > 100)
			checkSeat[0] = 1;
		if (checkSeat[0] == 1)
			/*cout << "personDetect1" << endl;
			else{
			}*/
			for (; contours1 != 0; contours1 = contours1->h_next)
			{
				rect = cvBoundingRect(contours1, 0); //extract bounding box for current contour

				//drawing rectangle
				cvRectangle(grayImage,
					cvPoint(60 + rect.x, 220 + rect.y),
					cvPoint(60 + rect.x + rect.width, 220 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);
				cvRectangle(frame,
					cvPoint(60 + rect.x, 220 + rect.y),
					cvPoint(60 + rect.x + rect.width, 220 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);

			}
		if (contours2 == 0)
		{
			countSeat[3] = 0;
			checkSeat[3] = 0;
		}
		else{
			countSeat[3]++;
		}
		if (countSeat[3] > 100)
			checkSeat[3] = 1;
		if (checkSeat[3] == 1)
			/*cout << "personDetect4" << endl;
			else{
			}*/
			for (; contours2 != 0; contours2 = contours2->h_next)
			{
				rect = cvBoundingRect(contours2, 0); //extract bounding box for current contour

				//drawing rectangle
				cvRectangle(grayImage,
					cvPoint(430 + rect.x, 100 + rect.y),
					cvPoint(430 + rect.x + rect.width, 100 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);
				cvRectangle(frame,
					cvPoint(430 + rect.x, 100 + rect.y),
					cvPoint(430 + rect.x + rect.width, 100 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);

			}
		if (contours3 == 0)
		{
			countSeat[2] = 0;
			checkSeat[2] = 0;
		}
		else{
			countSeat[2]++;
		}
		if (countSeat[2] > 100)
			checkSeat[2] = 1;
		if (checkSeat[2] == 1)
			/*cout << "personDetect3" << endl;
			else{
			}*/

			for (; contours3 != 0; contours3 = contours3->h_next)
			{
				rect = cvBoundingRect(contours3, 0); //extract bounding box for current contour

				//drawing rectangle
				cvRectangle(grayImage,
					cvPoint(185 + rect.x, 128 + rect.y),
					cvPoint(185 + rect.x + rect.width, 128 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);
				cvRectangle(frame,
					cvPoint(185 + rect.x, 128 + rect.y),
					cvPoint(185 + rect.x + rect.width, 128 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);

			}
		if (contours4 == 0)
		{
			countSeat[1] = 0;
			checkSeat[1] = 0;
		}
		else{
			countSeat[1]++;
		}
		if (countSeat[1] > 100)
			checkSeat[1] = 1;
		if (checkSeat[1] == 1)
			/*cout << "personDetect2" << endl;
			else{
			}*/
			for (; contours4 != 0; contours4 = contours4->h_next)
			{
				rect = cvBoundingRect(contours4, 0); //extract bounding box for current contour

				//drawing rectangle
				cvRectangle(grayImage,
					cvPoint(325 + rect.x, 325 + rect.y),
					cvPoint(325 + rect.x + rect.width, 325 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);
				cvRectangle(frame,
					cvPoint(325 + rect.x, 325 + rect.y),
					cvPoint(325 + rect.x + rect.width, 325 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);

			}
		if (contours5 == 0)
		{
			countSeat[4] = 0;
			checkSeat[4] = 0;
		}
		else{
			countSeat[4]++;
		}
		if (countSeat[4] > 100)
			checkSeat[4] = 1;
		if (checkSeat[4] == 1)
			/*cout << "personDetect5" << endl;
			else{
			}*/
			for (; contours5 != 0; contours5 = contours5->h_next)
			{
				rect = cvBoundingRect(contours5, 0); //extract bounding box for current contour

				//drawing rectangle
				cvRectangle(grayImage,
					cvPoint(290 + rect.x, 80 + rect.y),
					cvPoint(290 + rect.x + rect.width, 80 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);
				cvRectangle(frame,
					cvPoint(290 + rect.x, 80 + rect.y),
					cvPoint(290 + rect.x + rect.width, 80 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);
			}
		if (contours6 == 0)
		{
			countSeat[5] = 0;
			checkSeat[5] = 0;
		}
		else{
			countSeat[5]++;
		}
		if (countSeat[5] > 100)
			checkSeat[5] = 1;
		if (checkSeat[5] == 1)
			/*cout << "personDetect6" << endl;
			else{
			}*/
			for (; contours6 != 0; contours6 = contours6->h_next)
			{
				rect = cvBoundingRect(contours6, 0); //extract bounding box for current contour
				//drawing rectangle
				cvRectangle(grayImage,
					cvPoint(413 + rect.x, 30 + rect.y),
					cvPoint(413 + rect.x + rect.width, 30 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);
				cvRectangle(frame,
					cvPoint(413 + rect.x, 30 + rect.y),
					cvPoint(413 + rect.x + rect.width, 30 + rect.y + rect.height),
					cvScalar(0, 0, 255, 0),
					2, 8, 0);

			}


		//cout << "**************************" << endl;
		cvShowImage("grayImage", grayImage);

		char chKey = cvWaitKey(20);
		if (chKey == 27)    {    // Esc
			break;


		}
		end = clock();          // 시간설정
		curTime = (end - begin) / CLOCKS_PER_SEC;
		if (curTime % 1 == 0)///1초마다 data전달

		{
			if (save != curTime){
				cout << "수행시간 : " << ((end - begin) / CLOCKS_PER_SEC) << endl;
				passAns = to_string(checkSeat[0]) + to_string(checkSeat[1]) + to_string(checkSeat[2]) + to_string(checkSeat[3]) + to_string(checkSeat[4]) + to_string(checkSeat[5]);

				gettime(&sttime);
				if (sttime.ti_hour < 10){//10시 이전의 string 정리
					if (sttime.ti_min < 10) {//10분 이전의 string정리
						if (sttime.ti_sec < 10) {//10초 이전의 string정리
							sti = "0" + to_string(sttime.ti_hour);
							smi = "0" + to_string(sttime.ti_min);
							ssec = "0" + to_string(sttime.ti_sec);
						}
						else{
							sti = "0" + to_string(sttime.ti_hour);
							smi = "0" + to_string(sttime.ti_min);
							ssec = to_string(sttime.ti_sec);
						}
					}
					else{
						if (sttime.ti_sec < 10){
							sti = "0" + to_string(sttime.ti_hour);
							smi = to_string(sttime.ti_min);
							ssec = "0" + to_string(sttime.ti_sec);
						}
						else{
							sti = "0" + to_string(sttime.ti_hour);
							smi = to_string(sttime.ti_min);
							ssec = to_string(sttime.ti_sec);
						}
					}
				}
				else{
					if (sttime.ti_min < 10){
						if (sttime.ti_sec < 10){
							sti = to_string(sttime.ti_hour);
							smi = "0" + to_string(sttime.ti_min);
							ssec = "0" + to_string(sttime.ti_sec);
						}
						else{
							sti = to_string(sttime.ti_hour);
							smi = "0" + to_string(sttime.ti_min);
							ssec = to_string(sttime.ti_sec);
						}
					}
					else{
						if (sttime.ti_sec < 10){
							sti = to_string(sttime.ti_hour);
							smi = "0" + to_string(sttime.ti_min);
							ssec = to_string(sttime.ti_sec);
						}
						else{
							sti = to_string(sttime.ti_hour);
							smi = to_string(sttime.ti_min);
							ssec = to_string(sttime.ti_sec);
						}
					}
				}

				timeinfo = sti + smi + ssec;//시간정보

				if (checkSeat[0] == 1){
					if (saveSeatdata[0] == 0 || saveSeatdata[0] == 2){
						saveSeattime[0] = timeinfo;
					}
					saveSeatdata[0] = checkSeat[0];
				}
				else{
					saveSeatdata[0] = checkSeat[0];
					saveSeattime[0] = "0";
				}

				if (checkSeat[1] == 1){
					if (saveSeatdata[1] == 0 || saveSeatdata[1] == 2){
						saveSeattime[1] = timeinfo;
						cout << "personDetect2" << endl;
					}
					saveSeatdata[1] = checkSeat[1];
				}
				else{
					saveSeatdata[1] = checkSeat[1];
					saveSeattime[1] = "0";
				}

				if (checkSeat[2] == 1){
					if (saveSeatdata[2] == 0 || saveSeatdata[2] == 2){
						saveSeattime[2] = timeinfo;
						cout << "personDetect3" << endl;
					}
					saveSeatdata[2] = checkSeat[2];
				}
				else{
					saveSeatdata[2] = checkSeat[2];
					saveSeattime[2] = "0";
				}

				if (checkSeat[3] == 1){
					if (saveSeatdata[3] == 0 || saveSeatdata[3] == 2){
						saveSeattime[3] = timeinfo;
					}
					saveSeatdata[3] = checkSeat[3];
				}
				else{
					saveSeatdata[3] = checkSeat[3];
					saveSeattime[3] = "0";
				}

				if (checkSeat[4] == 1){
					if (saveSeatdata[4] == 0 || saveSeatdata[4] == 2){
						saveSeattime[4] = timeinfo;
						cout << "personDetect5" << endl;
					}
					saveSeatdata[4] = checkSeat[4];
				}
				else{
					saveSeatdata[4] = checkSeat[4];
					saveSeattime[4] = "0";
				}

				if (checkSeat[5] == 1){
					if (saveSeatdata[5] == 0 || saveSeatdata[5] == 2){
						saveSeattime[5] = timeinfo;
						cout << "personDetect6" << endl;
					}
					saveSeatdata[5] = checkSeat[5];
				}
				else{
					saveSeatdata[5] = checkSeat[5];
					saveSeattime[5] = "0";
				}

				stmt = dbConn->createStatement();
				try {
					stmt->execute("use spaceinfo");

					string sqlStr = "INSERT INTO inputdata VALUES('";
					sqlStr += timeinfo;
					sqlStr += "',";
					sqlStr += "'";
					sqlStr += passAns;
					sqlStr += "',";
					sqlStr += "'";
					sqlStr += saveSeattime[0];
					sqlStr += "',";
					sqlStr += "'";
					sqlStr += saveSeattime[1];
					sqlStr += "',";
					sqlStr += "'";
					sqlStr += saveSeattime[2];
					sqlStr += "',";
					sqlStr += "'";
					sqlStr += saveSeattime[3];
					sqlStr += "',";
					sqlStr += "'";
					sqlStr += saveSeattime[4];
					sqlStr += "',";
					sqlStr += "'";
					sqlStr += saveSeattime[5];
					sqlStr += "')";
					int affectedRows = stmt->executeUpdate(sqlStr);
					cout << "Done! Num. rows affected: " << affectedRows << endl;

				}
				catch (sql::SQLException e)
				{
					cout << "SQL error. Error message : " << e.what() << endl;
					system("pause");
					exit(1);
				}
				cout << "select num:  ";;

				cout << passAns << endl;
				save = curTime;

				String imageName, imageName1;
				imageName = "C:\\Users\\Tim\\Documents\\Visual Studio 2013\\Projects\\OPENDB\\ImageFile\\" + timeinfo + ".jpg";
				imageName1 = "C:\\Users\\Tim\\Documents\\Visual Studio 2013\\Projects\\OPENDB\\ImageFile\\save.jpg";
				cvSaveImage(imageName.c_str(), frame);
				cvSaveImage(imageName1.c_str(), frame);
				
			}
			
		}
		cvReleaseImage(&bkgImage1);
		cvReleaseImage(&grayImage1);
		cvReleaseImage(&diffImage1);

		cvReleaseImage(&bkgImage2);
		cvReleaseImage(&grayImage2);
		cvReleaseImage(&diffImage2);

		cvReleaseImage(&bkgImage3);
		cvReleaseImage(&grayImage3);
		cvReleaseImage(&diffImage3);

		cvReleaseImage(&bkgImage4);
		cvReleaseImage(&grayImage4);
		cvReleaseImage(&diffImage4);

		cvReleaseImage(&bkgImage5);
		cvReleaseImage(&grayImage5);
		cvReleaseImage(&diffImage5);

		cvReleaseImage(&bkgImage6);
		cvReleaseImage(&grayImage6);
		cvReleaseImage(&diffImage6);

		
	}
	
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
	/*cvDestroyAllWindows();
	cvReleaseImage(&temp);
	cvReleaseImage(&bkgImage);
	cvReleaseImage(&grayImage);
	cvReleaseImage(&diffImage);
	cvReleaseCapture(&capture);*/



	return 0;
}

void gettime(struct time *p) {
	char temp[30];
	char *cp;
	_strtime(temp); // temp char배열 안에 00:00:00(시:분:초)순의 문자열 상태로 저장 

	cp = strtok(temp, ":"); // temp배열의 첫번째 ':'까지 문자열를 끊어내어 cp에게 시에 해당하는 문자열의 시작주소를 리턴 

	p->ti_hour = atoi(cp); // 끊어낸 시에 해당하는 문자열을 정수형 데이터로 변환하여 p->ti_hour에 저장 

	cp = strtok(NULL, ":"); // temp배열의 두번째 ':'까지 문자열를 끊어내어 cp에게 분에 해당하는 문자열의 시작주소를 리턴 

	p->ti_min = atoi(cp); // 끊어낸 분에 해당하는 문자열을 정수형 데이터로 변환하여 p->ti_min에 저장 

	cp = strtok(NULL, ":"); // temp배열의 세번째 ':'까지 문자열를 끊어내려하나 ':'은 나타나지 않고 문자열이 
	// 끝나게 되므로 남은 문자열 전체를 리턴한다 즉, cp에게 초에 해당하는 문자열의 시작주소가 리턴한다. 

	p->ti_sec = atoi(cp); // 끊어낸 초에 해당하는 문자열을 정수형 데이터로 변환하여 p->ti_sec에 저장 
}