//
// Created by filip on 11.06.16.
//

#ifndef CORE_LINE_HPP
#define CORE_LINE_HPP

#include <opencv2/core/types.hpp>

class Line
{
public:
    enum Orientation
    {
        LO_HORIZONTAL = 1,
        LO_VERTICAL = 2
    };
    Orientation orientation;
    cv::Point2i start;
    cv::Point2i end, moving;
    bool IsDraw = false;
    bool DrawingInProgress = false;
};

#endif //CORE_LINE_HPP