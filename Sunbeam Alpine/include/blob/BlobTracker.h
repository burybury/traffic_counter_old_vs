#pragma once

#include <iostream>
#include <opencv2\world.hpp>
#include <opencv2\highgui.hpp>

#include <blob\blob.h>
#include <helper\MissionControl.h>
#include <helper\Logger.hpp>
class BlobTracker
{
private:
	bool firstTime;

	uint debugTrack;
	uint debugBlob;
	uint showBlobMask;
	uint showOutput;

	cvb::CvTracks tracks;

	void saveConfig();

	void loadConfig();

	MissionControl _mc = MissionControl("BlobTracker MC");
public:
	BlobTracker();

	~BlobTracker();

	cv::Mat process(const cv::Mat img_mask);

	const cvb::CvTracks getTracks();
};
