#include "Octopus.h"



Octopus::Octopus()
{
}


Octopus::~Octopus()
{
	for each(auto* bgs in bgSubtractors)
	{
		if (bgs != nullptr)
		{
			delete bgs;
		}
	}
	if (calibrator != nullptr)
	{
		delete calibrator;
	}
}

//double getFrameFromSeconds(double seconds) {
//
//}

bool
Octopus::initializeCameras()
{
	std::vector <std::string> sourcesCam1;
	sourcesCam1.push_back(std::string("C:\\Videos\\rzut_wiadukt\\gora_N\\2017_0127_112225.mp4"));
	//sourcesCam1.push_back(std::string("C:\\Videos\\kustry2\\GdanskLotnisko_przeplatanie_BOCZNE_720p_25.01.2017\\K1\\2017_0125_104350.mp4"));
	std::vector <std::string> sourcesCam2;
	sourcesCam2.push_back(std::string("C:\\Videos\\rzut_wiadukt\\boczna_S\\2017_0127_112200.mp4"));
	//sourcesCam2.push_back(std::string("C:\\Videos\\kustry2\\GdanskLotnisko_przeplatanie_BOCZNE_720p_25.01.2017\\K2\\2017_0125_104543.mp4"));
	std::vector <std::string> sourcesCam3;
	sourcesCam3.push_back(std::string("C:\\Videos\\rzut_wiadukt\\gora_S\\2017_0127_112021.mp4"));
	// fyr6il8
//	sourcesCam3.push_back(std::string("C:\\Videos\\kustry2\\GdanskLotnisko_przeplatanie_BOCZNE_720p_25.01.2017\\K3\\2017_0125_104533.mp4"));

	Camera cam1("K1", sourcesCam1, Camera::CameraPlacement::CAM_PLACEMENT_FRONT);
	Camera cam2("K2", sourcesCam2, Camera::CameraPlacement::CAM_PLACEMENT_MIDDLE);
	Camera cam3("K3", sourcesCam3, Camera::CameraPlacement::CAM_PLACEMENT_REAR);
	const double movieLengthInMinutes = 3;
	double movieLengthInSeconds = movieLengthInMinutes * 60;
	double movieLengthInFrames = movieLengthInSeconds * 30;
	
	


	videoHub.addCamera(cam1);
	videoHub.addCamera(cam2);
	videoHub.addCamera(cam3);
	return true;
}
