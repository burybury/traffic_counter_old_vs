#include <opencv2\core.hpp>
#include "RoadDrawer.h"
#include "Camera.h"

#pragma once
class Homography
{
private:
	Mat mRoadHomography;
	Mat mWarpedRoad = Mat(700, 500, CV_8UC3);
	std::vector<Point2i> staticPoints;
	Mat mRoiFrame;
	Rect mRoiRect;
	RoadDrawer &rd;
	bool homographyFound{ false };
public:
	Homography() = delete;
	Homography(RoadDrawer& rd) : rd(rd)
	{
		staticPoints.push_back(Point2i(100, 150));
		staticPoints.push_back(Point2i(100, 550));
		staticPoints.push_back(Point2i(300, 550));
		staticPoints.push_back(Point2i(300, 100));

	}
	Mat warpRoad() {
		for (auto &road : rd.getRoads())
		{
			if (rd.newRoadBorder())
			{
				mRoadHomography = findHomography(road.polygonPoints, staticPoints, noArray());
				mRoiRect = boundingRect(road.polygonPoints);
				homographyFound = true;
				rd.setNewRoadBorder(false);

			}
			if (homographyFound) {
				warpPerspective(rd.getCam().getFrame(), mWarpedRoad, mRoadHomography, mWarpedRoad.size());
				//	mRoiFrame = rd.getCam().getFrame()(mRoiRect);
				rd.getCam().cropFrame(mRoiRect);
				//cv::imshow("warped", mWarpedRoad);
				//imshow("roi", rd.getCam().getCurrentRoiFrame());
			}
		}
		return mWarpedRoad;
	}
	~Homography();
};

