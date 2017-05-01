#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <helper\Logger.hpp>
#include <video\bg-subtraction\MyBackgroundSubtractor.hpp>
#include <road\RoadDetector.h>
#include <road\RoadTransformator.hpp>

static const char *const MISSION_CONTROL_WINDOW_NAME = "Mission Control";
static const char *const DRAWING_MAT_WINDOW_NAME = "Drawable Window";

using namespace cv;
using namespace std;

std::string sourceFilename;

int main(int argc, char * argv[]);

void CreateMissionControl();
void ShowStartupInfo(VideoCapture);
int main(int argc, char *argv[])
{
	//sourceFilename = argv[1];
	sourceFilename = "C://Videos//regres.mp4";

	VideoCapture capture(sourceFilename); // open the video file for reading
	if (!capture.isOpened())  // if not success, exit program
	{
		Logger::AddMessage("Cannot open the video file: " + sourceFilename, __func__);
		return -1;
	}
	ShowStartupInfo(capture);

	//CreateMissionControl();


	Mat imgTmp;
	capture.read(imgTmp);

	MyBackgroundSubtractor bgSubtractor;
	//Drawer drawer;
	//ShapeBounder shapeBounder;
	//VehicleManager vehicleManager;
	//BlobTracker blobTracker;
	//HoughTransform houghTransform;
	//VideoStab videoStab;
	RoadDetector roadDetector;
	RoadTransformator roadTransformator;
	Size S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	//    namedWindow(DRAWING_MAT_WINDOW_NAME);
	//setMouseCallback(DRAWING_MAT_WINDOW_NAME, IOCallbacks::drawRectangleOnClick, static_cast<void *>(&drawer));
	Mat previous;
	bool isFirstRun = true;
	Mat frame;

	Multiplex  recMpx(2, 2, S, "rozne huje");
	recMpx.StartWritingVideo("C:\\Videos\\lotnia_out1.mp4");
	int key = 11;
	while (key != 'q' && capture.get(CV_CAP_PROP_POS_FRAMES) <= capture.get(CV_CAP_PROP_FRAME_COUNT))
	{
		capture.read(frame);
		if ((capture.get(CV_CAP_PROP_POS_FRAMES) == capture.get(CV_CAP_PROP_FRAME_COUNT)) ||
			(!isFirstRun && frame.cols == 0))
		{
			capture.release();
			capture.open(sourceFilename);
			isFirstRun = true;
			continue;
		}

		if (isFirstRun)
		{
			frame.copyTo(previous);
			isFirstRun = false;
			continue;
		}
		//imshow("RAW FRAME", frame);
		// 1's are cars
		//Mat bgMask = bgSubtractor.subtractBackground(frame);

		//        //TODO nie dziaa ;d Niech debugowanie wyświetla więcej ekranów
		//#ifdef _DEBUG // Show bg frame and bg mask
		//        Mat bgFrame;
		//        bgFrame = Mat::zeros(frame.size(), CV_8UC3);
		//        frame.copyTo(bgFrame, bgMask);
		//        imshow("Bg Mask", bgMask);
		//        imshow("Foreround Frame", backgroundFrame);
		//#endif
		//
		//imshow("Clear frame", frame);
		//Mat temp = videoStab.performAndReturn(frame, previous);
		//houghTransform.perform(temp);

		//videoStab.perform(frame, previous);
		
		Mat road_mask = roadDetector.perform(frame);
		Mat road_transformed = roadTransformator.perform(road_mask, frame,&recMpx);
		//Mat justBlobs = blobTracker.process(frame, bgMask);
		//cv::Mat frameWithBlobs = cv::Mat::zeros(frame.size(), CV_8UC3);
		//frameWithBlobs = frame + justBlobs;

		//imshow("Just Blobs", justBlobs);
		//vehicleManager.updateVehicles(blobTracker.getTracks());
		////        vehicleManager.process(justBlobs + frame);
		////        vehicleManager.process(justBlobs  + frame, blobTracker.getTracks());
		////        vehicleManager.drawCountingLine(frameWithBlobs);
		////        vehicleManager.countVehicles();
		////        Mat framewithData = vehicleManager.getSummaryView();

		//imshow("Frame with Blobs", frameWithBlobs);

		//cvWaitKey(0);
		//frame.copyTo(previous);
		key = waitKey(static_cast<int>(capture.get(CV_CAP_PROP_FPS)));
	}
	//recMpx.StopWritingVideo();
	return 0;
}
void ShowStartupInfo(VideoCapture cap)
{
	Logger::AddMessage("Frame per seconds: ", cap.get(CV_CAP_PROP_FPS), __func__);

}

void CreateMissionControl()
{
	int iLowH = 170;
	int iHighH = 179;

	int iLowS = 150;
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;

	namedWindow(MISSION_CONTROL_WINDOW_NAME, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	createTrackbar("LowH", MISSION_CONTROL_WINDOW_NAME, &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", MISSION_CONTROL_WINDOW_NAME, &iHighH, 179);

	createTrackbar("LowS", MISSION_CONTROL_WINDOW_NAME, &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", MISSION_CONTROL_WINDOW_NAME, &iHighS, 255);

	createTrackbar("LowV", MISSION_CONTROL_WINDOW_NAME, &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", MISSION_CONTROL_WINDOW_NAME, &iHighV, 255);
}