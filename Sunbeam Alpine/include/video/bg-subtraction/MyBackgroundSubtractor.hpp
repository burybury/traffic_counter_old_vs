//
// Created by filip on 13.02.16.
//

#ifndef CORE_BACKGROUNDSUBTRACTOR_HPP
#define CORE_BACKGROUNDSUBTRACTOR_HPP

#include <opencv2/video/background_segm.hpp>
#include <helper\MissionControl.h>

class MyBackgroundSubtractor
{
public:
	void help();

	MyBackgroundSubtractor();

	cv::Mat subtractBackground(const cv::Mat& frame);

private:
	cv::Ptr<cv::BackgroundSubtractor> pMOG2;
	MissionControl _mc = MissionControl("Background Subtractor Controller");
	void fillHolesInBinaryMask(cv::Mat &mat);
};

#endif //CORE_BACKGROUNDSUBTRACTOR_HPP