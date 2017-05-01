#include "Camera.h"
#include <helper/Logger.hpp>
#include <iostream>

const double CAMERA_FRAME_OVERLAP = 31.0;

Mat& Camera::getFrame()
{
	if (mNewFrameGrabbed) {
		bool success = mCapture.retrieve(mCurrentFrame);
		if (!success) {
			Logger::AddMessage(mName + " next video \n", __func__);
			if (OpenNextCapture())
			{
				setCurrentFrame(30.0);
				std::cout << " skipping frames \n " << std::endl;
				mCapture.grab();
				mCapture.retrieve(mCurrentFrame);
				if (cameraCalib != nullptr)
				{
					cameraCalib->undistortFrame(mCurrentFrame, mCurrentFrame);
				}
			}
			else
			{
				Logger::AddMessage(mName + " could not open next capture \n", __func__);
			}

		}
		else
		{

			if (cameraCalib != nullptr)
			{
				cameraCalib->undistortFrame(mCurrentFrame, mCurrentFrame);
			}

		}
		mNewFrameGrabbed = false;
	}

	return mCurrentFrame;
}

Mat& Camera::getCurrentRoiFrame()
{
	return mCurrentRoiFrame;
}

void Camera::cropFrame(Rect roi) {
	mCurrentRoiFrame = mCurrentFrame(roi);
}

bool Camera::OpenNextCapture()
{	
	if (mFilenameIndex < mFilenames.size())
	{
		std::string fileName = mFilenames.at(mFilenameIndex);
		mCapture.open(fileName);
		if (!mCapture.isOpened())
		{
			throw std::runtime_error("Could not open camera for flie: " + *mFilenameIterator);
			return false;
		}
		else
		{
			mFilenameIndex++;
			return true;
		}
	}
	else
	{
		throw std::runtime_error("Camera filenames list end " + mName);
		return false;
	}
}

double Camera::getCurrentFrameNumber()
{
	return mCapture.get(CV_CAP_PROP_POS_FRAMES);
}

bool Camera::setCurrentFrame(double frameNumber)
{
	return mCapture.set(CV_CAP_PROP_POS_FRAMES,frameNumber);
}

Camera::Camera(std::string name,std::vector<std::string> filenames,CameraPlacement camPlacement) : mName(name), mCamPlacement(camPlacement) , mNewFrameGrabbed(false), mFinalRoad(false)  {	
	if (filenames.size() > 0)
	{
		mFilenameIndex = 0;
		mFilenames = filenames;
	}
	else
	{
		throw std::runtime_error("Camera filenames list empty. cam: " + mName);
	}
}



Camera::~Camera()
{
	mCapture.release();
}

void Camera::setCameraCalibrator(CameraCalibrator * cam)
{
	cameraCalib = cam;
}
