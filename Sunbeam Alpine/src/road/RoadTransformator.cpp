#include <road\RoadTransformator.hpp>
#include <tuple>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <math.h>
#include <math/polyfit.hpp>

using namespace cv;
//void findRoadCorners(vector< cv::Point>& contour, int height, cv::Mat& imageToDraw, const cv::Mat& mask);

//void findRoadCorners(contours[largestIndex], mask.rows, masked, mask);

inline bool isInRange(int y, int thresh, int center) {
	if (y <= center + thresh && y >= center - thresh) {
		return true;
	}
	else {
		return false;
	}
}

int getXfromPolynomals(int y, int z, std::vector<float>& p1, std::vector<float>& p2, int transformedRoadSize) {
	// =(1-A2)*p1(y)+A2*p2(y) 
	float wage = (float(z)) / transformedRoadSize;
	float valueP1 = p1[y] * (1 - wage);
	float valueP2 = p2[y] * (wage);
	float xCoord = valueP1 + valueP2;
	return int(xCoord);

}

void roadBorderRegression(std::vector<Point2i>& contourPoints, cv::Size size, const cv::Mat &coloured,Multiplex* auxMpx = nullptr) {

	const uint upperThreshold = 20;
	const uint lowerThreshold = 30;
	//sort by height
	std::sort(contourPoints.begin(), contourPoints.end(),
		// Lambda expression begins
		[](Point2i a, Point2i b) {
		return (a.y < b.y);
	} // end of lambda expression
	);

	uint thresh = 0;
	std::vector<Point2i>::iterator iterator = contourPoints.begin();
	std::vector<Point2i> leftBorderRegressionData;
	std::vector<Point2i> rightBorderRegressionData;
	for (uint y = upperThreshold; y < size.height - lowerThreshold; y += 5) {
		//Point2i* currentPoint = iterator._Ptr;
		int leftExtremeValue = size.width;
		int rightExtremeValue = 0;
		Point2i* leftMost = nullptr, *rightMost = nullptr;
		// go to minimum
		iterator = contourPoints.begin();
		while (iterator != contourPoints.end()) {
			if (isInRange(iterator._Ptr->y, thresh, y)) {
				if (iterator._Ptr->x < leftExtremeValue) {
					leftExtremeValue = iterator._Ptr->x;
					leftMost = iterator._Ptr;
				}

				if (iterator._Ptr->x > rightExtremeValue) {
					rightExtremeValue = iterator._Ptr->x;
					rightMost = iterator._Ptr;
				}
				iterator++;

			}
			else if (iterator._Ptr->y < (y - thresh)) {
				iterator++;
			}
			else if (iterator._Ptr->y > (y + thresh)) {
				break;
			}

		}
		if (leftMost) {
			leftBorderRegressionData.push_back(*leftMost);

		}
		if (rightMost) {
			rightBorderRegressionData.push_back(*rightMost);
		}
	}

	double resize_ratio = 0.3;

	Mat view = Mat::zeros(size, CV_8UC3);
	//resize(coloured, view, cv::Size(), resize_ratio, resize_ratio);
	std::vector<float> leftBorderOX, leftBorderOY;
	std::vector<float> rightBorderOX, rightBorderOY;


	for each(Point2i pt in leftBorderRegressionData) {
		Vec3b* bgrPixel = &view.at<Vec3b>(pt);

		bgrPixel[2] = 255;
		leftBorderOX.push_back(pt.y);
		leftBorderOY.push_back(pt.x);
	}

	for each(Point2i pt in rightBorderRegressionData) {
		Vec3b* bgrPixel = &view.at<Vec3b>(pt);

		bgrPixel[0] = 255;
		rightBorderOX.push_back(pt.y);
		rightBorderOY.push_back(pt.x);
	}


	std::vector<float> leftFunctioncoeff = polyfit(leftBorderOX, leftBorderOY, 2);
	std::vector<float> rightFunctioncoeff = polyfit(rightBorderOX, rightBorderOY, 2);

	std::vector<float> oXnew;
	for (int y = 20; y < size.height - 10; y++) {
		oXnew.push_back(y);
	}



	std::vector<float>leftOYnew = polyval(leftFunctioncoeff, oXnew);
	std::vector<float>rightOYnew = polyval(rightFunctioncoeff, oXnew);

	std::vector<float>::iterator itLY = leftOYnew.begin();
	std::vector<float>::iterator itRY = rightOYnew.begin();
	std::vector<float>::iterator itX = oXnew.begin();
	while (itLY != leftOYnew.end() || itX != oXnew.end() || itRY != rightOYnew.end()) {
		if ((*itX._Ptr > 0 && *itX._Ptr < size.height)) {
			if ((*itLY._Ptr > 0 && *itLY._Ptr < size.width))
			{
				Vec3b* bgrPxl = &view.at<Vec3b>(*itX._Ptr, *itLY._Ptr);
				bgrPxl[0] = 255;
				bgrPxl[1] = 255;
				bgrPxl[2] = 255;
			}

			if (*itRY._Ptr > 0 && *itRY._Ptr < size.width) {
				Vec3b* bgrPxl = &view.at<Vec3b>(*itX._Ptr, *itRY._Ptr);
				bgrPxl[0] = 255;
				bgrPxl[1] = 255;
				bgrPxl[2] = 255;
			}

		}
		itLY++;
		itRY++;
		itX++;
	}


	imshow("left border view ", view);
	if (auxMpx) {
		auxMpx->Add(view, "road border regression");
	}

	Mat transformedRoad = Mat::zeros(view.size(), CV_8UC3);
	const uint transfromedRoadSize = 150;
	const uint leftBorder = 150;



	for (int y = 20; y < size.height - 10; y++) {
		for (uint x = leftBorder; x < leftBorder + transfromedRoadSize; x++) {
			Vec3b* bgrPxl = &transformedRoad.at<Vec3b>(y, x);
			int xCoord = getXfromPolynomals(y - 20, (x - leftBorder), leftOYnew, rightOYnew, transfromedRoadSize);
			if (xCoord > 0) {
				*bgrPxl = coloured.at<Vec3b>(y, xCoord);
			}

		}
	}

	imshow("female trouble", transformedRoad);
	if (auxMpx) {
		auxMpx->Add(transformedRoad, "transformed Road");
	}

	// now polynomal 


}


cv::Mat RoadTransformator::perform(const cv::Mat & source, const cv::Mat& coloured, Multiplex* recMpx)
{
	int leftmostUp = 0;
	int rightmostUp = 0;
	int rightmostDown = 0;
	int leftmostDown = 0;


	Mat src;
	resize(source, src, cv::Size(), 1.5, 1.5);

	Multiplex mpx(2, 2, src.size(), "RoadTransformator");
	mpx.Add(coloured, "src");
	if (recMpx) {
		recMpx->Add(coloured, "source");
	}
	// BURY DEMO


	std::vector < cv::Point> lowest;
	std::vector < cv::Point> highest;

	const int threshold = 10;

	int lowestVal = 0;
	int highestVal = src.rows - threshold;

	std::vector< std::vector< cv::Point> > contours;
	findContours(src, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat draw_cont(src);
	for (int i = 0; i < contours.size(); ++i)
	{
		drawContours(draw_cont, contours, i, Scalar(100, 100, 100), 2, 8);
	}
	mpx.Add(draw_cont, "all contours");
	if (recMpx) {
		recMpx->Add(draw_cont, "road contour ");
	}
	for each (cv::Point point2i in contours[0])
	{
		if (point2i.y < threshold) {
			if (point2i.y < lowestVal) {
				lowestVal = point2i.y;
			}
			lowest.push_back(point2i);
		}
		else if (point2i.y > (src.rows - threshold)) {
			if (point2i.y > highestVal) {
				highestVal = point2i.y;
			}
			highest.push_back(point2i);
		}
	}

	// -> road border
	Mat resizedColoured;
	resize(coloured, resizedColoured, src.size());
	roadBorderRegression(contours[0], src.size(), resizedColoured,recMpx);
	if (recMpx) {
		recMpx->Show();
	}
	/**
	std::vector<std::tuple<Point2i, Point2i>> lowestCandidates;

	for (uint y = lowestVal; y <= threshold; y++) {
		Point lowestLeftMost, lowestRightMost;
		uint leftmostVal = 400;
		uint rightmostVal = 0;
		bool leftFound = false;
		bool rightFound = false;
		for each (Point point2i in lowest) {
			if (point2i.y == y) {
				if (point2i.x < leftmostVal) {
					leftmostVal = point2i.x;
					lowestLeftMost = point2i;
					leftFound = true;
				}
				else if (point2i.x > rightmostVal) {
					rightmostVal = point2i.x;
					lowestRightMost = point2i;
					rightFound = true;
				}
			}
		}
		if (rightFound && leftFound) {
			lowestCandidates.push_back(std::make_tuple(lowestLeftMost, lowestRightMost));
		}
	}

	std::vector<std::tuple<Point2i, Point2i>> highestCandidates;

	for (uint y = src.rows; y >= src.rows - threshold; y--) {
		Point lowestLeftMost, lowestRightMost;
		uint leftmostVal = 400;
		uint rightmostVal = 0;
		bool leftFound = false;
		bool rightFound = false;
		for each (Point point2i in highest) {
			if (point2i.y == y) {
				if (point2i.x < leftmostVal) {
					leftmostVal = point2i.x;
					lowestLeftMost = point2i;
					leftFound = true;
				}
				else if (point2i.x > rightmostVal) {
					rightmostVal = point2i.x;
					lowestRightMost = point2i;
					rightFound = true;
				}
			}
		}
		if (rightFound && leftFound) {
			highestCandidates.push_back(std::make_tuple(lowestLeftMost, lowestRightMost));
		}
	}
	**/
	/**
	if (lowestCandidates.size() > 0 && highestCandidates.size() > 0) {
		std::vector<Point2f> homographyPointsRoadFound;

		// Push top of the road
		Point2f left = std::get<0>(lowestCandidates[0]);
		Point2f right = std::get<1>(lowestCandidates[0]);
		line(src, left, right, cv::Scalar(0, 255, 0), 5);
		homographyPointsRoadFound.push_back(left);
		homographyPointsRoadFound.push_back(right);

		// middle
		//
		/*
		for each (std::tuple<Point2i, Point2i> pair in perspectiveCandidates) {
		Point2f left = std::get<0>(pair);
		Point2f right = std::get<1>(pair);
		homographyPointsRoadFound.push_back(left);
		homographyPointsRoadFound.push_back(right);
		}

		// bottom
		left = std::get<0>(highestCandidates[0]);
		right = std::get<1>(highestCandidates[0]);

		line(src, left, right, cv::Scalar(0, 255, 0), 5);
		homographyPointsRoadFound.push_back(left);
		homographyPointsRoadFound.push_back(right);

		std::vector<Point2f> canvasPoints;
		canvasPoints.push_back(Point2f(200, 0));
		canvasPoints.push_back(Point2f(300, 0));
		canvasPoints.push_back(Point2f(200, 500));
		canvasPoints.push_back(Point2f(300, 500));

		Mat canvas; // = Mat::zeros(500, 500, CV_32FC3);
					//	Mat H = getAffineTransform(canvasPoints, homographyPointsRoadFound);
		Mat H = getPerspectiveTransform(homographyPointsRoadFound, canvasPoints);

		if (canvasPoints.size() == homographyPointsRoadFound.size()) {
			//Mat H =	findHomography(canvasPoints, homographyPointsRoadFound, noArray(), RANSAC);
			warpPerspective(src, canvas, H, cv::Size(600, 600));
			//warpAffine(imageToDraw, canvas, H, );

			imshow("moje", canvas);

			mpx.Add(canvas, "done!");
		}
			}
			*/

	mpx.Show();
	return Mat();
}
