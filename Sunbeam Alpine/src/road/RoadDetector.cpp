#include <road\RoadDetector.h>
#include <helper\Multiplex.h>
#include <iostream>
#include <opencv2\calib3d.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>
#include <tuple>

using namespace cv;
using namespace std;
cv::Mat RoadDetector::largest_contour_mask(const cv::Mat& src)
{
	double area{ 0 }, largest{ 0 };
	double length{ 0 }, longest{ 0 };
	int largestIndex{ 0 }, longestIndex{ 0 };

	vector< vector< cv::Point> > contours;
	findContours(src, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	//Mat draw_contours = Mat::zeros(src.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); ++i)
	{
		area = contourArea(contours[i]);
		if (area > largest) {
			largest = area;
			largestIndex = i;
		}
		length = arcLength(contours[i], true);
		if (length > longest) {
			longest = length;
			longestIndex = i;
		}
		//// default all to grey
		//drawContours(draw_contours, contours, i, Scalar(100, 100, 100), 2, 8);
	}

	//// largest area is blue
	//drawContours(draw_contours, contours, largestIndex, Scalar(255, 0, 0), 2, 8);

	//// longest area is red
	//drawContours(draw_contours, contours, longestIndex, Scalar(0, 0, 255), 2, 8);

	//// largest is bound with green rect
	//Rect bounding_rect_largest = boundingRect(contours[largestIndex]);
	//cv::rectangle(draw_contours, bounding_rect_largest, Scalar(0, 255, 0), 2);

	Mat mask = Mat::zeros(src.size(), CV_8UC1);
	cv::drawContours(mask, contours, largestIndex, Scalar(255, 255, 255), -1);
	return mask;
}

cv::Mat RoadDetector::morph(const cv::Mat & src)
{
	int morph_size = 1;
	Mat morphed;
	Mat element = getStructuringElement(MorphShapes::MORPH_RECT, Size(morph_size, morph_size));

	// Apply the 'opening' morphology operation
	morphologyEx(src, morphed, MORPH_OPEN, element, cv::Point(-1, -1), 2, BORDER_CONSTANT);

	// 'close' artifacts on the road
	morph_size = 3;
	element = getStructuringElement(MorphShapes::MORPH_RECT, Size(morph_size, morph_size));
	morphologyEx(morphed, morphed, MORPH_CLOSE, element, cv::Point(-1, -1), 2, BORDER_CONSTANT, Scalar(0, 0, 0));

	return morphed;
}

cv::Mat RoadDetector::perform(const cv::Mat & src)
{
	// resize for performance
	Mat resized;
	resize(src, resized, cv::Size(), resize_ratio, resize_ratio);

	Multiplex mpx(4, 4, resized.size(), "Perform");

	// Alternate 1-liner for whole code below, but it's slightly worse
	//Mat in_range;
	//inRange(resized_hsv, Scalar(_lowerb_h, _lowerb_s, _lowerb_v), Scalar(_upperb_h, _upperb_s, _upperb_v), in_range);
	//mpx.Add(in_range, "in_range");

	// convert to HSV
	Mat resized_hsv;
	cv::cvtColor(resized, resized_hsv, cv::COLOR_BGR2HSV);
	mpx.Add(resized_hsv, "resized_hsv");

	// color segmentation
	Mat mean_shifted;
	pyrMeanShiftFiltering(resized_hsv, mean_shifted, _sp, _sr, _maxLevel);
	mpx.Add(mean_shifted, "mean_shifted");

	// split into channels
	vector<Mat> channels(3);
	split(resized_hsv, channels);
	const Mat ch1 = channels[0];
	const Mat ch2 = channels[1];
	const Mat ch3 = channels[2];
	mpx.Add(ch1, "ch1");
	mpx.Add(ch2, "ch2");
	mpx.Add(ch3, "ch3");

	// threshold 'not-road' as precise as possible
	Mat ch1_thresholded;
	Mat ch2_thresholded;
	threshold(ch1, ch1_thresholded, _threshold1, 255, 0);
	threshold(ch2, ch2_thresholded, _threshold2, 255, 1);
	mpx.Add(ch1_thresholded, "ch1_thresholded");
	mpx.Add(ch2_thresholded, "ch2_thresholded");

	// 'ch1 & ch2' for filtering
	Mat and_ch1_ch2;
	bitwise_and(ch1_thresholded, ch2_thresholded, and_ch1_ch2);
	mpx.Add(and_ch1_ch2, "and_ch1_ch2");

	// remove bad pixels
	Mat morph_opened = morph(and_ch1_ch2);
	mpx.Add(morph_opened, "morph");

	// mark largest contour as road
	Mat road_mask = largest_contour_mask(morph_opened);
	mpx.Add(road_mask, "road_mask");

	//showMissionControl();
	mpx.Show();
	return road_mask;
}

void RoadDetector::showMissionControl()
{
	AddSliders();
}

void RoadDetector::AddSliders()
{
	//createTrackbar("sp", MISSION_CONTROL_W_NAME, &_sp, 255);
	//createTrackbar("sr", MISSION_CONTROL_W_NAME, &_sr, 255);
	//createTrackbar("maxLevel", MISSION_CONTROL_W_NAME, &_maxLevel, 8);
	//createTrackbar("low", MISSION_CONTROL_W_NAME, &_low, 255);
	//createTrackbar("high", MISSION_CONTROL_W_NAME, &_high, 255);



	createTrackbar("lowerb_h", MISSION_CONTROL_W_NAME, &_lowerb_h, 255);
	createTrackbar("lowerb_s", MISSION_CONTROL_W_NAME, &_lowerb_s, 255);
	createTrackbar("lowerb_v", MISSION_CONTROL_W_NAME, &_lowerb_v, 255);

	createTrackbar("upperb_h", MISSION_CONTROL_W_NAME, &_upperb_h, 255);
	createTrackbar("upperb_s", MISSION_CONTROL_W_NAME, &_upperb_s, 255);
	createTrackbar("upperb_v", MISSION_CONTROL_W_NAME, &_upperb_v, 255);
}

RoadDetector::RoadDetector()
{
	namedWindow(MISSION_CONTROL_W_NAME);
}

RoadDetector::~RoadDetector()
{
	destroyWindow(MISSION_CONTROL_W_NAME);
}