//
// Created by fbiedrzy on 5/29/2016.
//

#ifndef CORE_SHAPEBOUNDER_HPP
#define CORE_SHAPEBOUNDER_HPP

#include <opencv2/core/mat.hpp>

class ShapeBounder
{
    //TODO: RAII
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

public:
    cv::Mat getMatWithBoundRect(cv::Mat bgMask);

    cv::Mat getConvexHull();
};

#endif //CORE_SHAPEBOUNDER_HPP