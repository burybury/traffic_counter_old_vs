#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
#pragma once

namespace Octo
{
	class Settings
	{
	public:
		bool mTestCalibration{ false };
		bool mBackgroundSubtraction{ true };
		bool mCompensateFisheye{ true };
		bool blobTracking{ false };
		bool mDrawRoad{ true };
		bool mHomographyOn{ true };
	
		std::pair<Point2i, Point2i> cam1LowerBorder;
		std::pair<Point2i, Point2i> cam2UpperBorder;
		std::pair<Point2i, Point2i> cam2LowerBorder;
		std::pair<Point2i, Point2i> cam3UpperBorder;
		double forwardCam1{ 0 };
		double forwardCam2{ 0 };
		double forwardCam3{ 0 };
		double forwardCam4{ 0 };

		bool testCalibration() { return mTestCalibration; };
		bool subtractBackground() { return mBackgroundSubtraction; };
		bool compensateFisheye() { return mCompensateFisheye; };
		bool enableRoadDrawers(){ return mDrawRoad; };
		Settings();
		~Settings();
	};
}

