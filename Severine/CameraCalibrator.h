//cv
#include <opencv2\calib3d.hpp>
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
//stl
#include <iostream>
#include <stdexcept>

using namespace cv;

#pragma once
class CameraCalibrator
{
	VideoCapture chessboardCap;
	std::string chessboardPath{ "C:\\Videos\\calib\\jeden.mp4" };
	Mat currentFrame;
	Mat undistortedFrame;
	Mat coeff;
	Mat cameraMatrix;
	Matx33d k;
	Vec4d d;
public:	
	void testCalibration();
	bool readChessboardCapture();
	void undistortFrame();
	void undistortFrame(Mat& in, Mat& out);
	CameraCalibrator() = delete;
	CameraCalibrator(bool initForTest);
	CameraCalibrator(int a) {};
	~CameraCalibrator();
};

