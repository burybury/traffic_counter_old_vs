#include <helper\MissionControl.h>
using namespace cv;
MissionControl::MissionControl(std::string window_name)
{
	_window_name = window_name;
}
void MissionControl::Add(std::string name, double& value, int max_value)
{
	int temp = value * 100;
	Add(name, temp, max_value);
	value = temp / 100.0;
}
void MissionControl::Add(std::string name, int& value, int max_value)
{
	if (nullptr == cvGetWindowHandle(_window_name.c_str()))
		namedWindow(_window_name, CV_WINDOW_AUTOSIZE); //create a window called "Control"
	auto iter = values.find(name);
	if (iter != values.end())
	{
		value = iter->second.first;
		return;
	}
	values[name] = std::make_pair(value, max_value);
	createTrackbar(name, _window_name, &values[name].first, values[name].second); //Hue (0 - 179)


}
