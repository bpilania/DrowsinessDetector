//============================================================================
// Name        : DrowsinessDetector.cpp
// Author      : Sameer Jagdale
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "FaceFeatures.hpp"
#include "helper.hpp"
#include<fstream>
#define TEST
#define COMMENT "SAMEER:  drowsy eyes"
#define PIXELTHRESHOLD 100
#define NOEYESLIMIT 200
#define DISTANCE "10"
#define NOEYESFRAMES 10
void raiseAlarm();
bool checkEyes(vector<Rect> eyes);
void drawFaceRect(EyeDetector, Mat, Rect);
void drawEyesRect(EyeDetector detector, Mat frame, Rect facesI, Rect eyesJ);
int main() {
	cout << "Checkpoint 9" << endl;

	CameraHandler handle;
	EyeDetector detector;
	Mat eyeROI;
	cout << "Checkpoint 8" << endl;

	detector.setFaceCascade("haarcascade_frontalface_alt.xml");
	detector.setEyeCascade("haarcascade_eye_tree_eyeglasses.xml");
	handle.initCamera(-1);
#ifdef debug
	cout << "Checkpoint 7" << endl;
#endif
	cvNamedWindow("test");
	int numBlack = 0;
	int flag = 0;
	int count = 0;
	int noEyesCount;
#ifdef debug
	cout << "Checkpoint 6" << endl;
#endif
	while (true) {
#ifdef debug
		cout << "Checkpoint 5" << endl;
#endif
		Mat frame = handle.fetchFrame();
		Mat frame_gray;
		int faceIndx = 0;
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		vector<Rect> faces = detector.detectFaces(frame_gray);
		vector<Rect> eyes;
#ifdef debug
		cout << "Checkpoint 4" << endl;
#endif
		if (faces.size() > 0) {
			faceIndx = Helper::getMax(faces);
#ifdef debug
			cout << "Checkpoint 3.5" << endl;
#endif
			drawFaceRect(detector, frame, faces[faceIndx]);
#ifdef debug
			cout << "Checkpoint 3" << endl;
#endif
			eyes = detector.detectEyes(faces[faceIndx], frame_gray);

#ifdef debug
			cout << "size of eyes" << eyes.size() << endl;
#endif
			if (!checkEyes(eyes)) {
				noEyesCount++;
				if (noEyesCount >= NOEYESFRAMES) {

					raiseAlarm();
				}
			} else {
				noEyesCount = 0;
			}
#ifdef debug
			cout << "Checkpoint 1" << endl;
#endif
			for (uint j = 0; j < eyes.size(); j++) {
				if (j == 0 || abs(eyes[j].x - eyes[0].x) >= 100) {
					drawEyesRect(detector, frame, faces[faceIndx], eyes[j]);

					if (flag == 1 && eyes.size() > 0) {
#ifdef debug
						cout << "Entered in numblack" << endl;
#endif
						//numBlack += Helper::getBlackPixels(frame_gray(eyes[j]));
					}
#ifdef debug
					cout << "x coordinate" << eyes[j].x << endl;
#endif

				}

			}

			if (flag == 1) {
				count++;
				//cout << "black pixels" << numBlack / count << endl;
			}

#ifndef TEST

//			if (numBlack / count < PIXELTHRESHOLD) {
//				noEyesCount++;
//
//				if (noEyesCount == NOEYESLIMIT) {
//					raiseAlarm();
//					noEyesCount = 0;
//				}
//
//			}
#endif
			//cout << "black pixels" << numBlack / count << endl;

		} else {
			noEyesCount++;
		}
		imshow("test", frame);
		int c = waitKey(10);
		if ((char) c == 'c') {
			break;
		}
		if ((char) c == 'w') {
			Helper::writeFile("log.txt", COMMENT, (float) numBlack / count,
					faces[faceIndx].height, faces[faceIndx].width);
			numBlack = 0;
		}
		if ((char) c == 'r') {
			cout << "entered r" << endl;
			flag = 1;
		}
		if ((char) c == 's') {
			flag = 0;
		}

	}
	handle.releaseCamera();
	return 0;
}

void raiseAlarm() {
	cout << "ALARM!!! ALARM!!! ALARM !!!" << endl;
}
