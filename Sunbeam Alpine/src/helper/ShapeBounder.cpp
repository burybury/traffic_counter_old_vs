//
// Created by fbiedrzy on 5/29/2016.
//

#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <helper\ShapeBounder.hpp>

using namespace cv;

cv::Mat ShapeBounder::getMatWithBoundRect(cv::Mat bgMask)
{
	Mat matWithRect = Mat::zeros(bgMask.size(), CV_8UC3);
	findContours(bgMask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	std::vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect(Mat(contours[i]));
	}
	if (!contours.empty() && !hierarchy.empty())
	{
		for (int i = 0; i < contours.size(); i++)
		{
			//            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
			//                    drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
			int rectThreshold = 10;
			if (boundRect[i].height < rectThreshold ||
				boundRect[i].width < rectThreshold)
				continue;
			//            std::cout << "Bound rectangle: " << boundRect[i].size() << std::endl;
			//            // Logger::AddMessage("Bound rectangle: ", boundRect[i].size());

			Scalar color = Scalar(255, 255, 0);
			rectangle(matWithRect, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		}
	}
	return matWithRect;
}

Mat ShapeBounder::getConvexHull()
{
	std::vector<std::vector<Point>> hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], true);
	}
	//TODO: dupa tutaj
	return Mat();
}