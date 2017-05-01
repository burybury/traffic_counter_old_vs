#pragma once
// stdlib
#include <vector>
#include <string>

// opencv 
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\video.hpp>
// ubik
#include <helper\Multiplex.h>

using namespace cv;

class MaskExtractor {
	int slider_max = 100;
	int _sp = 150;
	int _sr = 150;
	int _maxLevel = 3;

	int _low = 42;
	int _high = 200;
	int _threshold1 = 77;
	int _threshold2 = 38;

	int _lowerb_h = 75;
	int _lowerb_s = 0;
	int _lowerb_v = 85;

	int _upperb_h = 190;
	int _upperb_s = 90;
	int _upperb_v = 255;
public:
	MaskExtractor();
	~MaskExtractor();

	const std::string MISSION_CONTROL_W_NAME = " mission control ";
	void HsvMask(
		_In_ const cv::Mat& BGR_source, 
		_Out_ Mat& and_CH1_CH2
	);
	void CreateMissionControl() {
		
		createTrackbar("threshold1", MISSION_CONTROL_W_NAME, &_threshold1, 255);
		createTrackbar("threshold2", MISSION_CONTROL_W_NAME, &_threshold2, 255);
		
	}


	cv::Mat morphAndRetrieveRoadMask(const cv::Mat & src);

	cv::Mat largest_contour_mask(const cv::Mat& src);
};
