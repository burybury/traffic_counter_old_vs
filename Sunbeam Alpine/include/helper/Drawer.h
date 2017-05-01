//
// Created by fbiedrzy on 5/28/2016.
//

#ifndef CORE_DRAWER_H
#define CORE_DRAWER_H

#include <cstdint>
#include <opencv2/core/mat.hpp>

class Drawer
{
public:
    int x = 0;
    int y = 0;
    unsigned int channels = 0;

    //    Drawer(cv::Mat *drawableMat);

    cv::Mat PaintDots(cv::Mat mat);

    void setPoint(int x, int y);
};

#endif //CORE_DRAWER_H
