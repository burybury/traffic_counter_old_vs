//
// Created by filip on 01.06.16.
//

#ifndef CORE_VEHICLE_HPP
#define CORE_VEHICLE_HPP

#include <blob\blob.h>
#include <helper\Line.hpp>

enum VehiclePosition
{
	VP_NONE = 0,
	VP_A = 1,
	VP_B = 2
};

class Vehicle
{
public:

	bool PositionChanged = false;
	VehiclePosition position = VP_NONE;
	unsigned int id;
	cvb::CvTrack track;

	Vehicle(unsigned int id, cvb::CvTrack track, VehiclePosition position);

	Vehicle(unsigned int id, cvb::CvTrack track);

	void refreshPosition(Line line);

	void updateTrack(cvb::CvTrack newTrack);

	std::string getInfo();

private:
	void changePositionTo(VehiclePosition newPosition);
};

#endif //CORE_VEHICLE_HPP