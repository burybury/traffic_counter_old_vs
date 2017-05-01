//
// Created by filip on 01.06.16.
//

#include <vehicle\Vehicle.hpp>
#include <vehicle\VehicleManager.hpp>

Vehicle::Vehicle(unsigned int id, cvb::CvTrack track, VehiclePosition position)
{
    this->id = id;
    this->track = track;
    this->position = position;
}

Vehicle::Vehicle(unsigned int id, cvb::CvTrack track)
{
    this->id = id;
    this->track = track;
    this->position = VehiclePosition::VP_NONE;
}

void Vehicle::refreshPosition(Line line)
{
    if (line.orientation == Line::LO_HORIZONTAL)
    {
        if (track.centroid.x < line.start.x)
        {
            changePositionTo(VP_A);
        }

        if (track.centroid.x > line.start.x)
        {
            changePositionTo(VP_B);
        }
    }

    if (line.orientation == Line::LO_VERTICAL)
    {
        if (track.centroid.y > line.start.y)
        {
            changePositionTo(VP_A);
        }

        if (track.centroid.y < line.start.y)
        {
            changePositionTo(VP_B);
        }
    }
}

void Vehicle::updateTrack(cvb::CvTrack newTrack)
{
    track = newTrack;
    refreshPosition(VehicleManager::line);
}

void Vehicle::changePositionTo(VehiclePosition newPosition)
{
    if (this->position != newPosition)
    {
        this->PositionChanged = true;
        this->position = newPosition;
    }
    else
        this->PositionChanged = false;
}

std::string Vehicle::getInfo()
{
    return "ID: " + std::to_string(this->id) +
        "\tPOS: " + std::to_string(this->position) +
        "\tTrack Inactive: " + std::to_string(this->track.inactive);
}