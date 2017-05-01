#include <blob\BlobTracker.h>

BlobTracker::BlobTracker() : firstTime(true), debugTrack(false), debugBlob(true),
showBlobMask(false), showOutput(false)
{
	Logger::AddMessage("BlobTracker()", __func__);
}

BlobTracker::~BlobTracker()
{
	std::cout << "BlobTracker)" << std::endl;
}

const cvb::CvTracks BlobTracker::getTracks()
{
	return tracks;
}

cv::Mat BlobTracker::process(const cv::Mat img_mask)
{
	if (img_mask.cols == 0)
	{
		Logger::AddMessage("Can't track blobs because mask is empty!", __func__, MsgType::ERROR);
		return img_mask;
	}
	cv::Mat img_output = cv::Mat::zeros(img_mask.size(), CV_8UC3);

	loadConfig();

	//if (firstTime)
	//	saveConfig();

	IplImage *frame = new IplImage(img_output);
	cvConvertScale(frame, frame, 1, 0);

	IplImage *segmentated = new IplImage(img_mask);

	IplConvKernel *morphKernel = cvCreateStructuringElementEx(5, 5, 1, 1, CV_SHAPE_RECT, NULL);
	cvMorphologyEx(segmentated, segmentated, NULL, morphKernel, CV_MOP_OPEN, 1);

	////    if (showBlobMask)
	//    cvShowImage("Blob Mask", segmentated);

	IplImage *labelImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1);

	cvb::CvBlobs blobs;
	cvb::cvLabel(segmentated, labelImg, blobs);
	int min_area = 350;
	int max_area = 1000000;
	_mc.Add("min_area", min_area, 10000);
	_mc.Add("max_area", max_area, 10000);
	//cvb::cvFilterByArea(blobs, 500, 1000000);
	cvb::cvFilterByArea(blobs, min_area, max_area);

	//cvb::cvRenderBlobs(labelImg, blobs, frame, frame, CV_BLOB_RENDER_BOUNDING_BOX);
	cvb::cvRenderBlobs(labelImg, blobs, frame, frame,
		//CV_BLOB_RENDER_BOUNDING_BOX
		CV_BLOB_RENDER_CENTROID

		| CV_BLOB_RENDER_COLOR
	);

	cvb::cvUpdateTracks(blobs, tracks, 10, 5);

	//cvb::cvRenderTracks(tracks, frame, frame);

	cv::Mat img_result = cv::cvarrToMat(frame);
	img_result.copyTo(img_output);

	cvReleaseImage(&labelImg);
	delete frame;
	delete segmentated;
	cvReleaseBlobs(blobs);
	cvReleaseStructuringElement(&morphKernel);

	firstTime = false;
	return img_output;
}

void BlobTracker::saveConfig()
{
	CvFileStorage *fs = cvOpenFileStorage("config/BlobTracker.xml", 0, CV_STORAGE_WRITE);

	//cvWriteInt(fs, "minArea", minArea);
	//cvWriteInt(fs, "maxArea", maxArea);

	cvWriteInt(fs, "debugTrack", debugTrack);
	cvWriteInt(fs, "debugBlob", debugBlob);
	cvWriteInt(fs, "showBlobMask", showBlobMask);
	cvWriteInt(fs, "showOutput", showOutput);

	cvReleaseFileStorage(&fs);
}

void BlobTracker::loadConfig()
{
	CvFileStorage *fs = cvOpenFileStorage("config/BlobTracker.xml", 0, CV_STORAGE_READ);

	//minArea = cvReadIntByName(fs, 0, "minArea", 500);
	//maxArea = cvReadIntByName(fs, 0, "maxArea", 20000);

	debugTrack = cvReadIntByName(fs, 0, "debugTrack", 1);
	debugBlob = cvReadIntByName(fs, 0, "debugBlob", 1);
	showBlobMask = cvReadIntByName(fs, 0, "showBlobMask", 1);
	showOutput = cvReadIntByName(fs, 0, "showOutput", 0);

	cvReleaseFileStorage(&fs);
}