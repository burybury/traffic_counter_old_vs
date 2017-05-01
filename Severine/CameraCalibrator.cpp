#include "CameraCalibrator.h"

void CameraCalibrator::testCalibration()
{
	while (cvWaitKey(100) != 'q')
	{
		if (readChessboardCapture())
		{

			imshow("chessboard", currentFrame);
			if(!undistortedFrame.empty())
			{
			 imshow("undistorted ", undistortedFrame);
			}

			//fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix, coeff, currentFrame.size(),
			//	Matx33d::eye(), newCamMat, 1);
			//fisheye::initUndistortRectifyMap(cameraMatrix, coeff, Matx33d::eye(), newCamMat, currentFrame.size(),
			//	CV_16SC2, map1, map2);
			//cv::undistort(currentFrame, undistortedFrame, cameraMatrix, coeff);
			undistortFrame();
			
		}
	}
}

bool
CameraCalibrator::readChessboardCapture()
{
	
	if (chessboardCap.isOpened()) {
		chessboardCap.read(currentFrame);
		return true;
	}
	return false;
}

void
CameraCalibrator::undistortFrame()
{
	
	imshow("chessboard", currentFrame);
	
	//fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix, coeff, currentFrame.size(),
	//	Matx33d::eye(), newCamMat, 1);
	//fisheye::initUndistortRectifyMap(cameraMatrix, coeff, Matx33d::eye(), newCamMat, currentFrame.size(),
	//	CV_16SC2, map1, map2);
	//cv::undistort(currentFrame, undistortedFrame, cameraMatrix, coeff);
	
	/***
	Size new_size = Size();
	Size size = new_size.area() != 0 ? new_size : currentFrame.size();

    cv::Mat map1, map2;
    fisheye::initUndistortRectifyMap(k, d, cv::Matx33d::eye(), Mat::eye(3, 3, CV_64F), size, CV_16SC2, map1, map2 );
	
	Mat stackFrame;
    cv::remap(currentFrame, stackFrame, map1, map2,InterpolationFlags::INTER_NEAREST, BORDER_CONSTANT);
	**/
	cv::Matx33d newK = k;
	cv::fisheye::undistortImage(currentFrame, undistortedFrame, k, d, newK);

}

void CameraCalibrator::undistortFrame(Mat & in, Mat & out)
{
	cv::Matx33d newK = k;
	cv::fisheye::undistortImage(in, out, k, d, newK);
}

CameraCalibrator::CameraCalibrator(bool initForTest)
{
	cameraMatrix = Mat::zeros(3, 3, CV_64F);

	k = cv::Matx33d(953.19438949, 0, 612.47317476,
		0, 963.39806993, 463.61962376,
		0, 0, 1.0);
	
	d = Vec4d(-0.49549516, 0.38535273, 0.00340996, -0.16967512);
	
	if (initForTest)
	{
		if (!chessboardCap.open(chessboardPath))
		{
			throw std::runtime_error("could not open chessboard vi ");

		}
		else
		{
			std::cout << "chessboard capture frame width: "
				<< chessboardCap.get(CV_CAP_PROP_FRAME_WIDTH)
				<< " " << std::endl;
		}
	}
	
	

}


CameraCalibrator::~CameraCalibrator()
{
}
