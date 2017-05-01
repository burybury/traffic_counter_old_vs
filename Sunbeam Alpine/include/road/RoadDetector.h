#pragma once
#include <opencv2\world.hpp>
#include <math.h>
#include <opencv2\highgui.hpp>
#include <opencv2\video.hpp>

class RoadDetector
{
public:

	cv::Mat perform(const cv::Mat& src);

    RoadDetector();
    ~RoadDetector();

private:
    cv::Mat largest_contour_mask(const cv::Mat & src);
    cv::Mat morph(const cv::Mat& src);

    std::string MISSION_CONTROL_W_NAME = "Road Detector Mission Control";

    double resize_ratio = 0.3;

    char text[100];

    void showMissionControl();
    void AddSliders();

    int _CannyThresholdLow = 0;
    int _CannyRatio = 3;
    int _CannyKernelSize = 3;

    int slider_max = 100;
    int _sp = 0;
    int _sr = 15;
    int _maxLevel = 3;

    int _low = 42;
    int _high = 200;
    int _threshold1 = 77;
    int _threshold2 = 49;

    int _lowerb_h = 75;
    int _lowerb_s = 0;
    int _lowerb_v = 85;

    int _upperb_h = 190;
    int _upperb_s = 90;
    int _upperb_v = 255;
};
