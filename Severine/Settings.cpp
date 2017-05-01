#include "Settings.h"


Octo::
Settings::Settings()
{
		// cam 1 border
	cam1LowerBorder.first = Point2i(488, 709);
	cam1LowerBorder.second = Point2i(900, 466);
	//cam 2 upper 
	cam2UpperBorder.first = Point2i(291, 193);
	cam2UpperBorder.second = Point2i(358, 177);
	//cam 2 lower
	cam2LowerBorder.first = Point2i(579, 683);
	cam2LowerBorder.second = Point2i(903, 507);
	//cam 3 uupper
	cam3UpperBorder.first = Point2i(490, 174);
	cam3UpperBorder.second = Point2i(552, 166);


	mTestCalibration = false;
	mBackgroundSubtraction = true;
	mCompensateFisheye = true;
	blobTracking = true;
	mDrawRoad = true;
	mHomographyOn = true;

}

Octo::
Settings::~Settings()
{

}