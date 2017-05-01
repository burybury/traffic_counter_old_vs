/*
Copyright (c) 2014, Nghia Ho
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <video\stabilization\VideoStab.hpp>

using namespace std;
using namespace cv;

// This video stablisation smooths the global trajectory using a sliding average window

// 1. Get previous to current frame transformation (dx, dy, da) for all frames
// 2. Accumulate the transformations to get the image trajectory
// 3. Smooth out the trajectory using an averaging window
// 4. Generate new set of previous to current transform, such that the trajectory ends up being the same as the smoothed trajectory
// 5. Apply the new transformation to the video

VideoStab::VideoStab()
{
}
cv::Mat VideoStab::perform(const Mat & currentFrame, const Mat& previousFrame)
{
	//ofstream out_transform("prev_to_cur_transformation.txt");
	//ofstream out_trajectory("trajectory.txt");
	//ofstream out_smoothed_trajectory("smoothed_trajectory.txt");
	//ofstream out_new_transform("new_prev_to_cur_transformation.txt");
	Mat cur_grey;
	Mat prev_grey;
	pyrMeanShiftFiltering(currentFrame, currentFrame, 20, 45, 3);
	pyrMeanShiftFiltering(previousFrame, previousFrame, 20, 45, 3);
	cvtColor(previousFrame, prev_grey, COLOR_BGR2GRAY);

	// Step 1 - Get previous to current frame transformation (dx, dy, da) for all frames
	// Step 1 - Get previous to current frame transformation (dx, dy, da) for only 2 frames
	TransformParam prev_to_cur_transform; // previous to current
	cvtColor(currentFrame, cur_grey, COLOR_BGR2GRAY);

	// vector from prev to cur
	vector <Point2f> prev_corner, cur_corner;
	vector <Point2f> prev_corner2, cur_corner2;
	vector <uchar> status;
	vector <float> err;

	goodFeaturesToTrack(prev_grey, prev_corner, 200, 0.01, 30);
	//Mat corners;
	//previousFrame.copyTo(corners);
	//for (auto i = prev_corner.begin(); i != prev_corner.end(); i++)
	//{
	//    circle(corners, *i, 5, CV_RGB(255, 0, 0), -1, 8, 0);
	//}
	//imshow("corners", corners);
	calcOpticalFlowPyrLK(prev_grey, cur_grey, prev_corner, cur_corner, status, err);

	// weed out bad matches
	for (size_t i = 0; i < status.size(); i++) {
		if (status[i]) {
			prev_corner2.push_back(prev_corner[i]);
			cur_corner2.push_back(cur_corner[i]);
		}
	}
	//Mat corners2;
	//currentFrame.copyTo(corners2);
	//for (auto i = cur_corner2.begin(); i != cur_corner2.end(); i++)
	//{
	//    circle(corners2, *i, 5, CV_RGB(255, 0, 0), -1, 8, 0);
	//}
	//imshow("corners2", corners2);
	// translation + rotation only
	Mat T = estimateRigidTransform(prev_corner2, cur_corner2, false); // false = rigid transform, no scaling/shearing

																	  // in rare cases no transform is found. We'll just use the last known good transform.
	if (T.data == NULL) {
		// breakpoint reasons
	}
	// decompose T
	double dx = T.at<double>(0, 2);
	double dy = T.at<double>(1, 2);
	double da = atan2(T.at<double>(1, 0), T.at<double>(0, 0));

	prev_to_cur_transform = TransformParam(dx, dy, da);

	T = Mat(2, 3, CV_64F);

	int vert_border = HORIZONTAL_BORDER_CROP * previousFrame.rows / previousFrame.cols; // get the aspect ratio correct

	T.at<double>(0, 0) = cos(prev_to_cur_transform.da);
	T.at<double>(0, 1) = -sin(prev_to_cur_transform.da);
	T.at<double>(1, 0) = sin(prev_to_cur_transform.da);
	T.at<double>(1, 1) = cos(prev_to_cur_transform.da);

	T.at<double>(0, 2) = prev_to_cur_transform.dx;
	T.at<double>(1, 2) = prev_to_cur_transform.dy;

	Mat cur2;

	warpAffine(currentFrame, cur2, T, currentFrame.size());

	cur2 = cur2(Range(vert_border, cur2.rows - vert_border), Range(HORIZONTAL_BORDER_CROP, cur2.cols - HORIZONTAL_BORDER_CROP));

	//// Resize cur2 back to cur size, for better side by side comparison
	//resize(cur2, cur2, currentFrame.size());

	//// Now draw the original and stabilised side by side for coolness
	//Mat canvas = Mat::zeros(currentFrame.rows, currentFrame.cols * 2 + 10, currentFrame.type());

	//currentFrame.copyTo(canvas(Range::all(), Range(0, cur2.cols)));
	//cur2.copyTo(canvas(Range::all(), Range(cur2.cols + 10, cur2.cols * 2 + 10)));

	//// If too big to fit on the screen, then scale it down by 3, hopefully it'll fit :)
	//if (canvas.cols > 1500) {
	//	resize(canvas, canvas, Size(canvas.cols / 3, canvas.rows / 3));
	//}
	////waitKey(30);
	return cur2;
}
VideoStab::~VideoStab()
{
}