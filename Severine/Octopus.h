#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <helper/Logger.hpp>
#include <video/bg-subtraction/MyBackgroundSubtractor.hpp>
#include <road/RoadDetector.h>
#include <road/RoadTransformator.hpp>
#include <video/stabilization/VideoStab.hpp>
#include <blob/BlobTracker.h>

#include "VideoHub.h"
#include "RoadDrawer.h"
#include "MainProcessing.h"
#include "CameraCalibrator.h"
#include "Settings.h"
#include "Road.h"
#include "Homography.h"



#include <package_bgs/IBGS.h>
#include <package_bgs/WeightedMovingMeanBGS.h>
#include <package_bgs/FrameDifferenceBGS.h>
#include <package_bgs/StaticFrameDifferenceBGS.h>
#include <package_bgs/WeightedMovingMeanBGS.h>
#include <package_bgs/WeightedMovingVarianceBGS.h>
#include <package_bgs/MixtureOfGaussianV2BGS.h>
#include <package_bgs/AdaptiveBackgroundLearning.h>
#include <package_bgs/AdaptiveSelectiveBackgroundLearning.h>
#include <package_bgs\KNNBGS.h>
#include <package_bgs\db\IndependentMultimodalBGS.h>
#include <package_bgs\sjn\SJN_MultiCueBGS.h>
//#include <package_bgs/KNNBGS.h>
#include <fstream>
#include <functional>


class Octopus
{
private:
public:
	std::vector<RoadDrawer> roadDrawers;
	std::vector<IndependentMultimodalBGS*> bgSubtractors;
	std::vector<Mat> bgModels;
	std::vector<Mat> imgMasks;
	CameraCalibrator *calibrator{ nullptr };
	std::vector<Homography> homographs;
	bool initializeCameras();
	VideoHub videoHub{};
	Octopus();
	~Octopus();
};

