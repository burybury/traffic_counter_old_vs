//
// Created by fbiedrzy on 5/28/2016.
//

#include <vehicle\VehicleManager.hpp>
#include <opencv2/world.hpp>
#include <IOCallbacks.hpp>

using namespace cv;
//void IOCallbacks::drawRectangleOnClick_MouseCB(int event, int x, int y, int flags, void *userdata = 0) {
//    Drawer *drawer = static_cast<Drawer *>(userdata);
//    //CV_DbgAssert(drawer != nullptr);
//    if (event == cv::EVENT_LBUTTONDOWN) {
//        drawer->setPoint(x, y);
//        std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
//    }
//    else if (event == cv::EVENT_RBUTTONDOWN) {
//        drawer->setPoint(x, y);
//        std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
//    }
//
//}

void IOCallbacks::drawRectangleOnClick(int event, int x, int y, int flags, void *userdata /* = 0*/)
{
	Drawer *drawer = static_cast<Drawer *>(userdata);
	//CV_DbgAssert(drawer != nullptr);
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		drawer->setPoint(x, y);
		std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		drawer->setPoint(x, y);
		std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
	}
}
void IOCallbacks::drawColourInfoOnClick(int event, int x, int y, int flags, void* userdata)
{
	//TODO
	//Mat& frame = *static_cast<Mat *>(userdata);
	//if (event == CV_EVENT_LBUTTONDOWN)
	//{
	//    Mat mat;
	//    cvtColor(frame, mat, CV_GRAY2BGR);
	//    Vec3b p = mat.at<Vec3b>(y, x);
	//    printf("R=%d, G=%d, B=%d", p[2], p[1], p[0]);
	//    //putText(globalTempMat, text, Point(5, 15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0));
	//}
	//else if (event == CV_EVENT_RBUTTONDOWN)
	//{
	//    Vec3b p = frame.at<Vec3b>(y, x);
	//    printf("H=%d, S=%d, V=%d", p[0], p[1], p[2]);
	//    //putText(globalTempMat, text, Point(5, 15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0));
	//}
}
void IOCallbacks::drawLineOnClick(int evt, int x, int y, int flags, void *userdata)
{
	Line *roi = static_cast<Line *>(userdata);

	if (evt == CV_EVENT_LBUTTONDOWN)
	{
		if (!roi->DrawingInProgress)
		{
			roi->start.x = x;
			roi->start.y = y;
			roi->DrawingInProgress = true;
		}
		else
		{
			roi->end.x = x;
			roi->end.y = y;
			roi->IsDraw = true;
			roi->DrawingInProgress = false;
		}
	}
	if (evt == CV_EVENT_MOUSEMOVE && roi->DrawingInProgress)
	{
		roi->moving.x = x;
		roi->moving.y = y;
	}
}