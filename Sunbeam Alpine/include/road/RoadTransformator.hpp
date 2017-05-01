#pragma once
#include <opencv2\world.hpp>
#include <helper\Multiplex.h>

class RoadTransformator
{
public:
	cv::Mat perform(const cv::Mat& src, const cv::Mat& coloured, Multiplex* mpx = nullptr);
};
