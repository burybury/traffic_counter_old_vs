#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// local
#include "CameraCalibrator.h"

#pragma once

using namespace cv;



class Camera
{
public:
	enum class CameraPlacement : unsigned
	{
		CAM_PLACEMENT_UNKNOWN = 0,
		CAM_PLACEMENT_FRONT = 1,
		CAM_PLACEMENT_MIDDLE = 2,
		CAM_PLACEMENT_REAR = 2
	};
private:
	CameraCalibrator* cameraCalib = nullptr;
	std::string mName;
	VideoCapture mCapture;
	std::vector<std::string> mFilenames;
	std::vector<std::string>::iterator mFilenameIterator;
	size_t mFilenameIndex;
	Mat mCurrentFrame;
	Mat mCurrentRoiFrame;
	CameraPlacement mCamPlacement;
	bool mNewFrameGrabbed;
	bool mFinalRoad;
public:
	void setCameraCalibrator(CameraCalibrator* cam);
	Camera() = delete;
	Camera(std::string name, std::vector<std::string> filenames,CameraPlacement camPlacement);
	Mat& getFrame();
	Mat& getCurrentRoiFrame();
	void cropFrame(Rect roi);
	bool OpenNextCapture();
	void grabNewFrame() {
		mNewFrameGrabbed = true;
		mCapture.grab();
	}
	void setFinalRoad(bool final) {
		mFinalRoad = true;
	}
	bool getFinalRoad() {
		return mFinalRoad;
	}
	double getCurrentFrameNumber();
	bool setCurrentFrame(double frameNumber);
	VideoCapture& getCapture() { return mCapture; }
	const std::string const getName() { return mName; }
	~Camera();

	bool operator==(const Camera& other)
	{
		if (mName == other.mName)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
};

