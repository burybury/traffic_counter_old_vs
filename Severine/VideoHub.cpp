#include "VideoHub.h"
#include <stdexcept>
#include <iostream>

#include <helper\Logger.hpp>


void VideoHub::addCamera(Camera& cam)
{
	try
	{
		mCameras.push_back(cam);
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

std::vector<Camera>& VideoHub::getCameras()
{	
	return mCameras;
}



// pushback empty matrices as containers for frames
void VideoHub::createFramesContainer()
{
	if (mCaptures.size() > 0)
	{
		for (unsigned int i = 0; i < mCaptures.size(); i++)
		{
			mFrameContainer.push_back(Mat());
		}
	}
	else
	{
		throw std::runtime_error("Containers empty");
	}
}

bool VideoHub::checkCapture(VideoCapture & capture)
{
	if (!capture.isOpened())
	{
		throw std::runtime_error("Capture could not be opened \n");
		return false;
	}
	else
	{
		printCaptureInfo(capture);
		double fps = capture.get(CV_CAP_PROP_FPS);
		double total_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
		capturesInfo.push_back(std::make_pair(fps, total_frames));
		return true;
	}	
}

VideoHub::VideoHub()
{
}

void VideoHub::printCaptureInfo(VideoCapture& capture)
{
		Logger::AddMessage("Frame per seconds: ", capture.get(CV_CAP_PROP_FPS), __func__);
		Logger::AddMessage("Total frames: ", capture.get(CV_CAP_PROP_FRAME_COUNT), __func__);
		Logger::AddMessage(
			"\n		BGS Options\n\
		\n*** Default Package ***\n\
		0: FrameDifferenceBGS\n\
		1 : StaticFrameDifferenceBGS\n\
		2 : WeightedMovingMeanBGS\n\
		3 : WeightedMovingVarianceBGS\n\
		4 : MixtureOfGaussianV1BGS\n\
		5 : MixtureOfGaussianV2BGS\n\
		6 : AdaptiveBackgroundLearning\n\
		7 : AdaptiveSelectiveBackgroundLearning\n\
		8 : GMG\n\
		9 : KNNBGS\n", __func__);
}

// this is the main function that grabs frames from captures
std::vector<Mat>& VideoHub::readFramesFromAllCaptures(unsigned int frameSkip)
{
	if (!mFrameContainer.empty())
	{
		return mFrameContainer;
	}
	else
	{
		throw std::runtime_error("Frame container empty");
	}
}
// skips number of frames
bool VideoHub::grabFrames(std::vector<unsigned int> numberOfFrames)
{
	if (numberOfFrames.size() == mCaptures.size())
	{
		for (size_t idx = 0; idx < mCaptures.size(); idx++)
		{
			for (unsigned int n = 0; n < numberOfFrames.at(idx); n++)
			{
				bool success = mCaptures.at(idx).grab();
				if (!success)
				{					
					throw std::runtime_error("Frames could not be grabbed");
				}
			}
		}
	}
	else
	{
		throw std::runtime_error("Size of skip vector not equal to size of capture vector \n");
	}
	return true;
}

std::vector<Mat>& VideoHub::retrevieFrames()
{	
	
	for (size_t idx = 0; idx < mCaptures.size() ;idx++) 
	{
		bool frameGrabbed = mCaptures.at(idx).retrieve(mFrameContainer.at(idx));
		if (!frameGrabbed)
		{
			throw std::runtime_error("Capture end! Need to open next file\n");
		}
	}
}


void VideoHub::processVideos()
{
}

void VideoHub::addSources(std::vector<std::string> sources)
{
	mSources = sources;
}

void VideoHub::openCaputres()
{
	assert(!mSources.empty());
	for (auto &string : mSources)
	{
		VideoCapture capture;
		
		mCaptures.push_back(VideoCapture(string));
	}

	
	try
	{
		for (auto &capture : mCaptures)
		{
			checkCapture(capture);
		}
		createFramesContainer();
	}
	catch (std::runtime_error& e) 
	{
		throw e;
	}
}

std::vector <std::string>& VideoHub::getSources()
{
	return mSources;
}

std::vector<Mat>& VideoHub::getFrameContainer()
{
	return mFrameContainer;
	// TODO: insert return statement here
}

bool VideoHub::rewindAllCameras(double numberOfFrames)
{
	bool canRewindAll = true;
	for (auto& cam : mCameras)
	{
		if ((cam.getCurrentFrameNumber() - numberOfFrames) < 0)
		{
			canRewindAll = false;
		}
	}

	if (canRewindAll) {
		for (auto& cam : mCameras)
		{
			if (canRewindAll)
			{
				cam.setCurrentFrame(numberOfFrames);
			}
		}
		return true;
	}
	else
	{
		std::cout << "Could not rewind all cams \n" << std::endl;
		return false;
	}
}


VideoHub::~VideoHub()
{
}
