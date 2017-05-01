//
// Created by filip on 13.02.16.
//
#include <video/motion-estimation/MotionEstimatorRansacL2Builder.hpp>
#include <video/stabilization/VideoStabilization.hpp>

using namespace std;
using namespace cv;
using namespace cv::videostab;

Ptr<ImageMotionEstimatorBase> MotionEstimatorRansacL2Builder::build()
{
	Ptr<MotionEstimatorRansacL2> est = makePtr<MotionEstimatorRansacL2>(motionModel(arg(prefix + "model")));

	RansacParams ransac = est->ransacParams();
	if (arg(prefix + "subset") != "auto")
		ransac.size = argi(prefix + "subset");
	if (arg(prefix + "thresh") != "auto")
		ransac.thresh = argf(prefix + "thresh");
	ransac.eps = argf(prefix + "outlier-ratio");
	est->setRansacParams(ransac);

	est->setMinInlierRatio(argf(prefix + "min-inlier-ratio"));

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