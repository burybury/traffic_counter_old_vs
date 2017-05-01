#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/world.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include "RoadDrawer.h"
#include <functional>

#pragma once

using namespace cv;

class Road
{
public:
	enum : unsigned {

	};
	std::vector<Point2i> polygonPoints;
	const Point* elementPoints[1];
	double mAlpha = 0.5;
	double mBeta = (1.0 - mAlpha);
	Scalar mColor{ 255, 0, 0 };
	bool mSelected{ false };
public:
	Road()
	{
		polygonPoints.push_back(Point2i(100, 100));
		polygonPoints.push_back(Point2i(100, 400));
		polygonPoints.push_back(Point2i(400, 400));
		polygonPoints.push_back(Point2i(400, 100));
		
	};

	void draw(Mat& frame)
	{
		Mat frameOrignal;
		frame.copyTo(frameOrignal);
		elementPoints[0] = { &polygonPoints[0] };
		int numberOfPoints = (int)polygonPoints.size();
		fillPoly(frame, elementPoints, &numberOfPoints, 1, Scalar(255.0, 0.0, 0.0), LINE_4);		
		Mat newFrame;
		addWeighted(frameOrignal, mAlpha, frame, mBeta, 0.0, frame);
		
	}
	bool testIfClicked(Point clickPoint)
	{
		InputArray input(polygonPoints);
		double retVal = pointPolygonTest(input, clickPoint, false);
		if (retVal > 0)
		{
			std::cout << "road selected " << std::endl;
			return true;
		}
		return false;
	}
	~Road() = default;
};

