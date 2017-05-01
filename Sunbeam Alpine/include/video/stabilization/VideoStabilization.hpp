//
// Created by filip on 13.02.16.
//
#include <string>
#include <iostream>
#include <stdexcept>
#include "opencv2/core.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videostab.hpp"

#ifndef CORE_VIDEOSTABILIZATION_HPP
#define CORE_VIDEOSTABILIZATION_HPP

void run();

cv::Mat stabilizeVideo(char *);
//void saveMotionsIfNecessary();

void printHelp();

cv::videostab::MotionModel motionModel(const std::string &str);

#endif //CORE_VIDEOSTABILIZATION_HPP