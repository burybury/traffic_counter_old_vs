#pragma once

#include <iostream>
#include <string>
#include <opencv2\world.hpp>
#include <opencv2\highgui.hpp>

#include <blob\blob.h>
#include <vehicle\Vehicle.hpp>
#include <helper\Drawer.h>

class VehicleManager
{
	bool firstTime = true;
	std::map<unsigned int, Vehicle> vehicles;

	int countAB = 0;
	int countBA = 0;

	void process(cv::Mat frame);

	void process(cv::Mat frame, cvb::CvTracks tracks);

	void saveConfig();

	void loadConfig();

public:
	static Line line;

	void updateVehicles(const cvb::CvTracks map);

	bool drawCountingLine(const cv::Mat &img_output);

	void countVehicles();

	cv::Mat getSummaryView();

	void showInactiveVehicles();

	unsigned long oldCount;
};