#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vehicle/VehicleManager.hpp>
#include <IOCallbacks.hpp>
#include <helper\Logger.hpp>

static const char *const VEHICLE_COUNTING_WINDOW_NAME = "Vehicle Counting";
Line VehicleManager::line;
//
//VehiclePosition VehicleManager::getVehiclePosition(const CvPoint2D64f centroid, cv::Mat &img_output, bool draw = true)
//{
//    VehiclePosition vehiclePosition = VP_NONE;
//    cv::Point2i point = cv::Point(centroid.x, centroid.y);
//    std::string stateA = "(A) " + std::to_string(centroid.x) + ":" + std::to_string(centroid.y);
//    std::string stateB = "(B) " + std::to_string(centroid.x) + ":" + std::to_string(centroid.y);
//    if (line.orientation == Line::LO_HORIZONTAL)
//    {
//        if (centroid.x < line.start.x)
//        {
//
//            if (draw)
//                cv::putText(img_output, stateA, point, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
//            vehiclePosition = VP_A;
//        }
//
//        if (centroid.x > line.start.x)
//        {
//            if (draw)
//                cv::putText(img_output, stateB, point, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 255));
//            vehiclePosition = VP_B;
//        }
//    }
//
//    if (line.orientation == Line::LO_VERTICAL)
//    {
//        if (centroid.y > line.start.y)
//        {
//            if (draw)
//                cv::putText(img_output, stateA, point, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
//            vehiclePosition = VP_A;
//        }
//
//        if (centroid.y < line.start.y)
//        {
//            if (draw)
//                cv::putText(img_output, stateB, point, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 255));
//            vehiclePosition = VP_B;
//        }
//    }
//
//    return vehiclePosition;
//}

//void VehicleManager::process(cv::Mat frame, cvb::CvTracks tracks)
//{
//    cv::Mat img_output = cv::Mat::zeros(frame.size(), CV_8UC3);
//    frame.copyTo(img_output);
//
////    loadConfig();
//
//    drawCountingLine(img_output);
//    if (positions.size() > 0)
//        std::cout << "-------------MAP SHOW START----------" << std::endl;
//    //--------------------------------------------------------------------------
//    for (std::map<cvb::CvID, VehiclePosition>::iterator it = positions.begin(); it != positions.end(); it++)
//    {
//
//
//        cvb::CvID id = (*it).first;
//        VehiclePosition track = (*it).second;
//        std::cout << "id: " << id << std::endl;
//        std::cout << "track: " << track << std::endl;
//
//    }
//    if (positions.size() > 0)
//        std::cout << "-------------MAP SHOW END----------" << std::endl;
//    for (std::map<unsigned int, Vehicle>::iterator it = vehicles.begin(); it != vehicles.end(); it++)
//    {
//        unsigned int id = (*it).first;
//        Vehicle vehicle = (*it).second;
//
//        CvPoint2D64f centroid = vehicle.track->centroid;
//
////        std::cout << "---------------------------------------------------------------" << std::endl;
////        std::cout << "0)id:" << id << " (" << centroid.x << "," << centroid.y << ")" << std::endl;
////        std::cout << "track->active:" << track->active << std::endl;
////        std::cout << "track->inactive:" << track->inactive << std::endl;
////        std::cout << "track->lifetime:" << track->lifetime << std::endl;
//
////        if (track->inactive == 0)
//        if (0)
//        {
//            if (positions.count(id) > 0)
//            {
//                std::map<cvb::CvID, VehiclePosition>::iterator it2 = positions.find(id);
//                VehiclePosition old_position = it2->second;
//
//                VehiclePosition current_position = getVehiclePosition(centroid, img_output);
//
//                if (current_position != old_position)
//                {
//                    if (old_position == VP_A && current_position == VP_B)
//                        countAB++;
//
//                    if (old_position == VP_B && current_position == VP_A)
//                        countBA++;
//
//                    positions.erase(positions.find(id));
//                }
//            }
//            else
//            {
//                VehiclePosition vehiclePosition = getVehiclePosition(centroid, img_output);
//
//                if (vehiclePosition != VP_NONE)
//                    positions.insert(std::pair<cvb::CvID, VehiclePosition>(id, vehiclePosition));
//
//            }
//        }
//        else
//        {
//            if (positions.count(id) > 0)
//                positions.erase(positions.find(id));
//        }
//        cvWaitKey(0);
//    }
//
//    std::string countABstr = "A->B: " + std::to_string(countAB);
//    std::string countBAstr = "B->A: " + std::to_string(countBA);
//
//    cv::putText(img_output, countABstr, cv::Point(10, frame.size().height - 20), cv::FONT_HERSHEY_PLAIN,
//                1,
//                cv::Scalar(255, 255, 255));
//    cv::putText(img_output, countBAstr, cv::Point(10, frame.size().height - 8), cv::FONT_HERSHEY_PLAIN,
//                1,
//                cv::Scalar(255, 255, 255));
//
//    firstTime = false;
//    cv::imshow(VEHICLE_COUNTING_WINDOW_NAME, img_output
//    );
//}

bool VehicleManager::drawCountingLine(const cv::Mat &img_output)
{
	if (!line.IsDraw && firstTime)
	{
		do
		{
			putText(img_output, "DRAW COUNTING LINE AND PRESS [ENTER]", cv::Point(10, 15), cv::FONT_HERSHEY_PLAIN, 1,
				cv::Scalar(0, 0, 255));

			cvSetMouseCallback(VEHICLE_COUNTING_WINDOW_NAME, IOCallbacks::drawLineOnClick,
				static_cast<void *>(&line));
			if (line.DrawingInProgress && !line.IsDraw)
			{
				cv::line(img_output, line.start, line.moving, CV_RGB(255, 0, 255));
			}
			imshow(VEHICLE_COUNTING_WINDOW_NAME, img_output);
			cvWaitKey(0);

			if (line.IsDraw)
			{
				std::cout << "Counting line defined (" << line.start.x << "," << line.start.y << "," << line.end.x <<
					"," << line.end.y << ")" << std::endl;
				break;
			}
			else
				std::cout << "Counting line undefined!" << std::endl;
		} while (1);
	}
	cvDestroyWindow(VEHICLE_COUNTING_WINDOW_NAME);
	bool isHorizontal = abs(line.start.x - line.end.x) < abs(line.start.y - line.end.y);

	std::string orientation;
	cv::Point start, end;

	if (isHorizontal)
	{
		line.orientation = Line::LO_HORIZONTAL;
		orientation = "Horizontal";
		start = cv::Point(line.start.x - 32, line.start.y);
		end = cv::Point(line.start.x + 12, line.end.y);
	}
	else
	{
		line.orientation = Line::LO_VERTICAL;
		orientation = "Vertical";
		start = cv::Point(line.start.x, line.start.y + 22);
		end = cv::Point(line.start.x, line.start.y - 12);
	}

	putText(img_output, orientation, cv::Point(10, 15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));

	putText(img_output, "(A)", start, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
	putText(img_output, "(B)", end, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));

	cv::line(img_output, line.start, line.end, cv::Scalar(0, 0, 255));

	return true;
}

void VehicleManager::saveConfig()
{
	CvFileStorage *fs = cvOpenFileStorage("config/VehicleManager.xml", 0, CV_STORAGE_WRITE);

	cvWriteInt(fs, "fav1_roi_defined", line.IsDraw);
	cvWriteInt(fs, "fav1_roi_x0", line.start.x);
	cvWriteInt(fs, "fav1_roi_y0", line.start.y);
	cvWriteInt(fs, "fav1_roi_x1", line.end.x);
	cvWriteInt(fs, "fav1_roi_y1", line.end.y);

	cvReleaseFileStorage(&fs);
}

void VehicleManager::loadConfig()
{
	CvFileStorage *fs = cvOpenFileStorage("config/VehicleManager.xml", 0, CV_STORAGE_READ);

	line.IsDraw = cvReadIntByName(fs, 0, "fav1_roi_defined", false) == 0 ? false : true;
	line.start.x = cvReadIntByName(fs, 0, "fav1_roi_x0", 0);
	line.start.y = cvReadIntByName(fs, 0, "fav1_roi_y0", 0);
	line.end.x = cvReadIntByName(fs, 0, "fav1_roi_x1", 0);
	line.end.y = cvReadIntByName(fs, 0, "fav1_roi_y1", 0);

	cvReleaseFileStorage(&fs);
}

void VehicleManager::updateVehicles(const cvb::CvTracks tracks)
{
	// mark all cars as missing, we will validate this statement in foreach tracks loop below
	for (auto it = vehicles.begin(); it != vehicles.end(); it++)
	{
		it->second.track.inactive++;
	}

	for (auto it = tracks.begin(); it != tracks.end(); it++)
	{
		cvb::CvID id = (*it).first;
		cvb::CvTrack *track = (*it).second;

		// ignore short-living blobs
		if (track->active >= 5)
		{
			// if no car in map, add it
			if (vehicles.count(id) == 0)
			{
				Vehicle vehicle(id, cvb::CvTrack(*track));
				vehicles.insert(std::pair<unsigned int, Vehicle>(vehicle.id, vehicle));
				Logger::AddMessage("New car detected! " + vehicle.getInfo(), __func__);
				continue;
			}
			// if found, refresh track info
			else
			{
				Vehicle *foundVehicle = &vehicles.find(id)->second;
				foundVehicle->updateTrack(cvb::CvTrack(*track));
			}
		}
	}
	auto it = vehicles.begin();
	while (it != vehicles.end())
	{
		Vehicle &v = it->second;
		if (v.track.inactive == 5)
		{
			Logger::AddMessage("Removing car! " + v.getInfo(), __func__);
			vehicles.erase(it++);
		}
		else
			++it;
	}
	//showInactiveVehicles();
}

cv::Mat VehicleManager::getSummaryView()
{
	return cv::Mat();
}

void VehicleManager::countVehicles()
{
	int oldBA = countBA;
	int oldAB = countAB;
	for (std::map<unsigned int, Vehicle>::iterator it = vehicles.begin(); it != vehicles.end(); it++)
	{
		unsigned int id = (*it).first;
		Vehicle vehicle = (*it).second;

		CvPoint2D64f centroid = vehicle.track.centroid;

		//        std::cout << "---------------------------------------------------------------" << std::endl;
		//        std::cout << "0)id:" << id << " (" << centroid.x << "," << centroid.y << ")" << std::endl;
		//        std::cout << "track->active:" << vehicle.track->active << std::endl;
		//        std::cout << "track->inactive:" << vehicle.track->inactive << std::endl;
		//        std::cout << "track->lifetime:" << vehicle.track->lifetime << std::endl;

		if (vehicle.track.inactive == 0)
		{
			if (vehicle.PositionChanged && vehicle.position == VehiclePosition::VP_A)
				countBA++;
			if (vehicle.PositionChanged && vehicle.position == VehiclePosition::VP_B)
				countAB++;
		}
	}

	if (oldBA != countBA)
		Logger::AddMessage("B->A: ", countBA, __func__);
	if (oldAB != countAB)
		Logger::AddMessage("A->B: ", countAB, __func__);
}

void VehicleManager::showInactiveVehicles()
{
	for (auto it = vehicles.begin(); it != vehicles.end(); it++)
	{
		Vehicle *vehicle = &it->second;
		if (vehicle->track.inactive > 0)
			Logger::AddMessage(vehicle->getInfo(), __func__);
	}
}