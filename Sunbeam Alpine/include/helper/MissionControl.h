#pragma once
#include <string>
#include <map>
#include <opencv2\world.hpp>
#include <opencv2\world.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\video.hpp>

class MissionControl
{
public:

	MissionControl(std::string window_name = "Mission Control v.69");
	void Add(std::string name, double & value, int max_value);
	void Add(std::string name, int & value, int max_value);
private:
	std::string _window_name = "";
	std::map < std::string, std::pair<int, int>> values;

};