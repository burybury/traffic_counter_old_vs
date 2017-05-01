//
// Created by filip on 13.02.16.
//

#ifndef CORE_MOTIONESTIMATORL1BUILDER_HPP
#define CORE_MOTIONESTIMATORL1BUILDER_HPP

#include "IMotionEstimatorBuilder.hpp"

class MotionEstimatorL1Builder : public IMotionEstimatorBuilder
{
#define arg(name) cmd.get<string>(name)
#define argb(name) cmd.get<bool>(name)
#define argi(name) cmd.get<int>(name)
#define argf(name) cmd.get<float>(name)
#define argd(name) cmd.get<double>(name)
private:
    bool gpu;
    std::string prefix;
public:
    MotionEstimatorL1Builder(CommandLineParser &command) : IMotionEstimatorBuilder(command)
    { }

    MotionEstimatorL1Builder(CommandLineParser &command, bool use_gpu, const string &_prefix = "")
        : IMotionEstimatorBuilder(command), gpu(use_gpu), prefix(_prefix)
    { }

    Ptr<ImageMotionEstimatorBase> build();
};

#endif //CORE_MOTIONESTIMATORL1BUILDER_HPP