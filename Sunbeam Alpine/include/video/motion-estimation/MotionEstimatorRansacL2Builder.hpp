//
// Created by filip on 13.02.16.
//

#ifndef CORE_MOTIONESTIMATORRANSACL2BUILDER_HPP
#define CORE_MOTIONESTIMATORRANSACL2BUILDER_HPP

#include "IMotionEstimatorBuilder.hpp"

class MotionEstimatorRansacL2Builder : public IMotionEstimatorBuilder
{
#define arg(name) cmd.get<string>(name)
#define argb(name) cmd.get<bool>(name)
#define argi(name) cmd.get<int>(name)
#define argf(name) cmd.get<float>(name)
#define argd(name) cmd.get<double>(name)
public:
    MotionEstimatorRansacL2Builder(CommandLineParser &command) : IMotionEstimatorBuilder(command)
    { }

    MotionEstimatorRansacL2Builder(CommandLineParser &command, bool use_gpu, const string &_prefix = "")
        : IMotionEstimatorBuilder(command), gpu(use_gpu), prefix(_prefix)
    { }

    Ptr<ImageMotionEstimatorBase> build();

private:
    bool gpu;
    string prefix;
};

#endif //CORE_MOTIONESTIMATORRANSACL2BUILDER_HPP