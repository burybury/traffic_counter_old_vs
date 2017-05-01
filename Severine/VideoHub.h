//stl
#include <vector>
//cv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//local
#include "Camera.h"

using namespace cv;

#pragma once
class VideoHub
{
private:
	std::vector<Camera> mCameras;
	// old
	std::vector<std::string> mSources;
	std::vector<VideoCapture> mCaptures;
	std::vector<std::pair<double, double>> capturesInfo;
	std::vector<Mat> mFrameContainer;
	void createFramesContainer();
	bool checkCapture(VideoCapture &capture);
public:
	VideoHub();
	void addCamera(Camera& cam);
	std::vector<Camera>& getCameras();

	void printCaptureInfo(VideoCapture& capture);
	std::vector<Mat>& readFramesFromAllCaptures(unsigned int frameSkip);
	bool grabFrames(std::vector <unsigned int> numberOfFrames);
	std::vector<Mat>& retrevieFrames();
	void processVideos();
	void addSources(std::vector <std::string> sources);
	void openCaputres();
	std::vector <std::string>& getSources();
	std::vector<Mat>& getFrameContainer();
	bool rewindAllCameras(double numberOfFrames);
	~VideoHub();
};

