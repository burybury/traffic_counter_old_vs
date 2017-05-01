//
// Created by fbiedrzy on 5/29/2016.
//

#ifndef CORE_LOGGER_HPP
#define CORE_LOGGER_HPP

static const int MAX_CALLER_LENGTH = 50;
enum MsgType
{
    INFO, ERROR, DEBUG
};
#include <string>
#include <opencv2/core/types.hpp>

class Logger
{
public:

    static const std::string currentDateTime();

    static void AddMessage(std::string msg, const char *caller);

    static void AddMessage(std::string msg, double val, const char *caller);

    static void AddMessage(std::string msg, int val, const char *caller);

    static void AddMessage(std::string msg, cv::Size_<int> val, const char *caller);

    static void AddMessage(std::string msg, const char *caller, MsgType msgType);
};

#endif //CORE_LOGGER_HPP