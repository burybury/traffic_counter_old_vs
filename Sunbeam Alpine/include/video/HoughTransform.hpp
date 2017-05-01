//
// Created by fbiedrzy on 5/28/2016.
//

#ifndef HOUGHTRANSFORM_HPP
#define HOUGHTRANSFORM_HPP
#include <opencv2\world.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class HoughTransform
{
public:
    void perform(const cv::Mat& src);
};

#endif //HOUGHTRANSFORM_HPP