#pragma once
#include <opencv2\world.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video.hpp>
class VideoStab
{
public:

	struct TransformParam
	{
		TransformParam() {}
		TransformParam(double _dx, double _dy, double _da) {
			dx = _dx;
			dy = _dy;
			da = _da;
		}

		double dx;
		double dy;
		double da; // angle
	};

	struct Trajectory
	{
		Trajectory() {}
		Trajectory(double _x, double _y, double _a) {
			x = _x;
			y = _y;
			a = _a;
		}

		double x;
		double y;
		double a; // angle
	};

	const int HORIZONTAL_BORDER_CROP = 0; // In pixels. Crops the border to reduce the black borders from stabilisation being too noticeable.

	VideoStab();
	cv::Mat perform(const cv::Mat & currentFrame, const cv::Mat & previousFrame);
	~VideoStab();
};