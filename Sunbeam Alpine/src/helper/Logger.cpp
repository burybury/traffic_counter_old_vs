//
// Created by fbiedrzy on 5/29/2016.
//

#include <iostream>
#include <opencv2/core/types.hpp>
#include <helper\Logger.hpp>
#include <ctime>

using u_int = unsigned int;

void Logger::AddMessage(std::string msg, const char *func_name, MsgType msgType)
{
	typeid(msgType).name();
	std::string tabs = "", msgTypeStr = "";

	switch (msgType)
	{
	case INFO:
		msgTypeStr = "INFO";
		break;
	case ERROR:
		msgTypeStr = "ERROR";
		break;
	case DEBUG:
		msgTypeStr = "DEBUG";
		break;
	}
	//if (MAX_CALLER_LENGTH >= strlen(caller))
	//    for (u_int i = 0; i <= ceil((MAX_CALLER_LENGTH - strlen(caller)) / 4); ++i)
	//    {
	//        tabs += "\t";
	//    }
	std::cout << "[" << msgTypeStr << "][" << currentDateTime() << "][" << func_name << tabs << "]:\t" << msg << std::endl;
}

void  Logger::AddMessage(std::string msg, cv::Size_<int> val, const char *caller)
{
	AddMessage(msg + "[H: " + std::to_string(val.height) + ", W: " + std::to_string(val.width) + "]", caller);
}

void Logger::AddMessage(std::string msg, double val, const char *caller)
{
	AddMessage(msg + std::to_string(val), caller);
}

void  Logger::AddMessage(std::string msg, int val, const char *caller)
{
	AddMessage(msg + std::to_string(val), caller);
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string Logger::currentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	localtime_s(&tstruct, &now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
//    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}

void Logger::AddMessage(std::string msg, const char *caller)
{
	AddMessage(msg, caller, MsgType::INFO);
}