//
// Created by filip on 13.02.16.
//

#ifndef CORE_IMOTIONESTIMATORBUILDER_HPP
#define CORE_IMOTIONESTIMATORBUILDER_HPP

#include "opencv2/core.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videostab.hpp"

using namespace std;
using namespace cv;
using namespace cv::videostab;

class IMotionEstimatorBuilder
{
public:
    //    virtual ~IMotionEstimatorBuilder();

    virtual Ptr<ImageMotionEstimatorBase> build() = 0;

protected:
    IMotionEstimatorBuilder(CommandLineParser &command) : cmd(command)
    { }

    CommandLineParser cmd;
};

#endif //CORE_IMOTIONESTIMATORBUILDER_HPP