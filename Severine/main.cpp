#include "Octopus.h"


using namespace cv;
using namespace std;

static const char *const MISSION_CONTROL_WINDOW_NAME = "Main() Mission Control";
static const char *const DRAWING_MAT_WINDOW_NAME = "Drawable Window";



bool ChangeBGS(int& key, IBGS** bgs);
void PrintInfo(VideoCapture);
int main(int argc, char *argv[])
{

	Octo::Settings s{};
	Octopus octopus{};
	// initialize video hub && cameras
	octopus.initializeCameras();

	try
	{
		for (auto& cam : octopus.videoHub.getCameras())
		{
			cam.OpenNextCapture();
		}
	}
	catch (std::runtime_error& e)
	{
		cout << e.what() << endl;
		return -1;
	}

	// initialize road drawer 	


	if (s.enableRoadDrawers())
	{
		for (auto&cam : octopus.videoHub.getCameras())
		{
			octopus.roadDrawers.push_back(RoadDrawer(cam));

		}
		for (auto& rd : octopus.roadDrawers)
		{
			rd.setCallbackForWindow(&rd);
			rd.makeButtons();
		}
		// initialize road drawers 

	}

	if (s.mHomographyOn)
	{
		for (auto& rd : octopus.roadDrawers)
		{
			octopus.homographs.push_back(Homography(rd));
		}
	}
	/**
	octopus.roadDrawer.getRoadBorders().at(0)->setLowerBorder(s.cam1LowerBorder);
	octopus.roadDrawer.getRoadBorders().at(1)->setLowerBorder(s.cam2LowerBorder);
	octopus.roadDrawer.getRoadBorders().at(1)->setUpperBorder(s.cam2UpperBorder);
	octopus.roadDrawer.getRoadBorders().at(2)->setUpperBorder(s.cam3UpperBorder);
	**/
	// synchronize cameras
	//octopus.videoHub.getCameras().at(0).
	octopus.videoHub.getCameras().at(0).setCurrentFrame(0);
	octopus.videoHub.getCameras().at(1).setCurrentFrame(685);
	octopus.videoHub.getCameras().at(2).setCurrentFrame(3745);


	BlobTracker blobTracker;
	//MissionControl mc = MissionControl(MISSION_CONTROL_WINDOW_NAME);
	unsigned int frameskip = 0;
	Mat previous;
	bool isFirstRun = true;
	Mat frame;
	if (s.subtractBackground()) {
		for (int i = 0; i < octopus.videoHub.getCameras().size(); i++)
		{
			octopus.bgSubtractors.push_back(new IndependentMultimodalBGS(1.0));
			octopus.bgModels.push_back(Mat());
			octopus.imgMasks.push_back(Mat());
		}
	}

	if (s.compensateFisheye())
	{
		try {
			octopus.calibrator = new CameraCalibrator(s.testCalibration());
			if (s.testCalibration())
			{
				octopus.calibrator->testCalibration();
			}
			for (auto& cam : octopus.videoHub.getCameras())
			{
				cam.setCameraCalibrator(octopus.calibrator);
			}
		}
		catch (std::runtime_error e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	char key{ 0 };



	while (key != 'q') // && capture.get(CV_CAP_PROP_POS_FRAMES) <= total_frames) //cout << "Processing progress: " << capture.get(CV_CAP_PROP_POS_FRAMES) << "/" << total_frames << endl;
	{
		//CvSize size = cvSize(cams.at(0).getCapture().get(CAP_PROP_FRAME_WIDTH), cams.at(0).getCapture().get(CAP_PROP_FRAME_HEIGHT));
		CvSize size(400, 400);
		Multiplex mpx(3, 3, size, "Main() overview");
		for (auto& cam : octopus.videoHub.getCameras())
		{
			cam.grabNewFrame();
			Mat frame = cam.getFrame();
			mpx.Add(frame, cam.getName().c_str());

		}

		if (s.enableRoadDrawers())
		{
			for (auto& roadDrawer : octopus.roadDrawers)
			{
				roadDrawer.showWindow();
			}

		}

		if (s.mHomographyOn)
		{
			int i = 1;
			for (auto& homography : octopus.homographs)
			{

				Mat warped = homography.warpRoad();

				std::string buffer2;
				buffer2 += "Odksztalcenie dla K";
				buffer2 += to_string(i);

				mpx.Add(warped, buffer2.c_str());

				i++;
			}
		}

		if (s.subtractBackground()) {
			for (size_t idx = 0; idx < octopus.videoHub.getCameras().size(); idx++)
			{
				// process first time default bgs;

				Mat frame = octopus.videoHub.getCameras().at(idx).getCurrentRoiFrame();
				if (!frame.empty())
				{
					if (octopus.videoHub.getCameras().at(idx).getFinalRoad())
					{
						octopus.bgSubtractors.at(idx)->process(frame, octopus.imgMasks.at(idx), octopus.bgModels.at(idx));
						//std::string buffer;
						//buffer += "Mask ";
						//buffer += to_string(idx);
						//mpx.Add(octopus.imgMasks.at(idx), buffer.c_str());


						std::string buffer2;
						buffer2 += "Trackowanie dla K";
						buffer2 += to_string(idx);
						Mat justBlobs = blobTracker.process(octopus.imgMasks.at(idx));

						mpx.Add(justBlobs+frame, buffer2.c_str());
					}
				}
			}
		}



		mpx.Show();



		key = cvWaitKey(1);
		if (key == ' ')
		{
			for (auto& cam : octopus.videoHub.getCameras())
			{
				std::cout << cam.getName() << " paused at frame: " << cam.getCurrentFrameNumber() << " " << std::endl;
			}
			while (cvWaitKey(100) != ' ');
		}

	}


	return 0;
}

bool ChangeBGS(int& key, IBGS** bgs)
{
	if (key == -1) return false;

	delete *bgs;
	*bgs = nullptr;
	cvDestroyAllWindows();

	switch (key)
	{
		/*** Default Package ***/
	case '0': *bgs = new FrameDifferenceBGS; break;
	case '1': *bgs = new StaticFrameDifferenceBGS; break;
	case '2': *bgs = new WeightedMovingMeanBGS; break;
	case '3': *bgs = new WeightedMovingVarianceBGS; break;
	case '4': *bgs = new IndependentMultimodalBGS; break;
	case '5': *bgs = new MixtureOfGaussianV2BGS; break;
	case '6': *bgs = new AdaptiveBackgroundLearning; break;
	case '7': *bgs = new AdaptiveSelectiveBackgroundLearning; break;
	case '8': *bgs = new SJN_MultiCueBGS; break;// not in 3.1
	case '9': *bgs = new KNNBGS(); break;
	}

	if (*bgs == nullptr)
	{
		Logger::AddMessage("BGS ID not recognized!. Creating default.", __func__);
		*bgs = new FrameDifferenceBGS;

	}
	key = -1;
	return true;
}


/**
VideoCapture capture(sourceFilename);
capture.set(CAP_PROP_POS_MSEC, 935270.0);
double dupa = capture.get(CV_CAP_PROP_FOURCC);

int ex = static_cast<int>(dupa);
char EXT[] = { (char)(ex & 0XFF),(char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24),0 };
Size S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
//VideoWriter outputVideo(outputeFilename, VideoWriter::fourcc(EXT[0], EXT[1], EXT[2], EXT[3]), capture.get(CV_CAP_PROP_FPS), S, true);                                        // Open the output
**/
/**

vector<Point2i> staticRoad1;
staticRoad1.push_back(Point2i(332, 154));
staticRoad1.push_back(Point2i(393, 164));
staticRoad1.push_back(Point2i(247, 378));
staticRoad1.push_back(Point2i(94, 338));

vector<Point2i> staticRoad2;
staticRoad2.push_back(Point2i(254,144));
staticRoad2.push_back(Point2i(293, 152));
staticRoad2.push_back(Point2i(115, 256));
staticRoad2.push_back(Point2i(74, 235));

vector<Point2i> staticRoad2;
staticRoad2.push_back(Point2i(170, 118));
staticRoad2.push_back(Point2i(321, 137));
staticRoad2.push_back(Point2i(147, 240));
staticRoad2.push_back(Point2i(6, 195));

vector<Point2i> staticRoad2;
staticRoad2.push_back(Point2i(244, 132));
staticRoad2.push_back(Point2i(314, 140));
staticRoad2.push_back(Point2i(165, 228));
staticRoad2.push_back(Point2i(79, 204));


Road dojazdPrawy = Road(staticRoad1, "dojazd", Road::RoadType::jednoPasmowa,&fileObwo);
Road obwPrawyPas = Road(staticRoad2, "owodnica", Road::RoadType::twoPasmowa,&fileDojazd);
**/



//outputVideo.open(outputeFilename, ex, capture.get(CV_CAP_PROP_FPS), S, true);


/**
if (!outputVideo.isOpened())
{
cout << "Could not open the output video for write: " << outputeFilename << endl;
return -1;
}
**/



//double fps = capture.get(CV_CAP_PROP_FPS);
//int total_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
