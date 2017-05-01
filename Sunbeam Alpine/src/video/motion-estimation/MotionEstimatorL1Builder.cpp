//
// Created by filip on 13.02.16.
//

//
// Created by filip on 13.02.16.
//
#include <video/stabilization/VideoStabilization.hpp>
#include <video/motion-estimation/MotionEstimatorL1Builder.hpp>

using namespace std;
using namespace cv;
using namespace cv::videostab;

Ptr<ImageMotionEstimatorBase> MotionEstimatorL1Builder::build()
{
	Ptr<MotionEstimatorL1> est = makePtr<MotionEstimatorL1>(motionModel(arg(prefix + "model")));

	Ptr<IOutlierRejector> outlierRejector = makePtr<NullOutlierRejector>();
	if (arg(prefix + "local-outlier-rejection") == "yes")
	{
		Ptr<TranslationBasedLocalOutlierRejector> tblor = makePtr<TranslationBasedLocalOutlierRejector>();
		RansacParams ransacParams = tblor->ransacParams();
		if (arg(prefix + "thresh") != "auto")
			ransacParams.thresh = argf(prefix + "thresh");
		tblor->setRansacParams(ransacParams);
		outlierRejector = tblor;
	}

#if defined(HAVE_OPENCV_CUDAIMGPROC) && defined(HAVE_OPENCV_CUDAOPTFLOW)
	if (gpu)
	{
		Ptr<KeypointBasedMotionEstimatorGpu> kbest = makePtr<KeypointBasedMotionEstimatorGpu>(est);
		kbest->setOutlierRejector(outlierRejector);
		return kbest;
	}
#endif

	Ptr<KeypointBasedMotionEstimator> kbest = makePtr<KeypointBasedMotionEstimator>(est);
	kbest->setDetector(GFTTDetector::create(argi(prefix + "nkps")));
	kbest->setOutlierRejector(outlierRejector);
	return kbest;
}