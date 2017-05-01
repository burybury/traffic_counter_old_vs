/**
std::vector<Point2i> clickedPoints;
bool homographyFound = false;

class Road {
	Mat roadHomography;
	Mat warpedRoad = Mat(700, 500, CV_8UC3);
	std::vector<Point2i> staticPoints;
	string name;
	Scalar color;
	std::vector<Point2i> framePoints;
	bool verbose = true;
	std::ofstream* file = nullptr;
public:
	enum RoadType {
		twoPasmowa,
		jednoPasmowa
	}type;
	Road(vector<Point2i>& roadCoords, string roadName, RoadType type, ofstream* pFstream) : type(type), name(roadName), color(Scalar(255, 0, 0, 0.1)), framePoints(roadCoords), file(pFstream) {
		vector<Point2i> staticPoints;
		staticPoints.push_back(Point2i(100, 150));
		staticPoints.push_back(Point2i(300, 150));
		staticPoints.push_back(Point2i(300, 550));
		staticPoints.push_back(Point2i(100, 550));

		*file << "frames,delta T[s],delta Y[px], delta Y[m],speed[km/h],pas\n";

		roadHomography = findHomography(roadCoords, staticPoints, noArray());
	}
	void showRoad() {
		imshow(name, warpedRoad);
	}
	void process(Mat& frame) {
		warpPerspective(frame, warpedRoad, roadHomography, warpedRoad.size());
		//fillConvexPoly(warpedRoad, staticPoints, color,2);
		rectangle(warpedRoad, Rect(100, 150, 200, 400), Scalar(255, 0, 0), 2);

	}
	void drawRectangleOfCar(Point2f& coords) {

	}
	bool inRange(vector<Point2f>& newPts, cvb::CvTrack* track) {
		Point2f& pt = newPts.at(0);
		//	if (pt.x > 100 && pt.x < 300 && pt.y>150 && pt.y < 550) {
		if (pt.x > 60 && pt.x < 300 && pt.y>0 && pt.y < 700) {
			return true;
		}
		return false;
	}
	void checkIfInField(cvb::CvTrack* track) {
		vector<Point2f> srcPt{ Point2f(track->centroid.x,track->centroid.y) };
		vector<Point2f> newPts;
		perspectiveTransform(srcPt, newPts, roadHomography);
		if (inRange(newPts, track)) {
			//track->gatherPoints(newPts[0].y);


			if (newPts[0].y <= 550 && newPts[0].y > 150) {
				if (track->speedStarted == false) {
					track->startY = newPts[0].y;
					track->speedStarted = true;
				}
				else {
					track->countTime();
				}
				//track->countTime();
			}
			else if (newPts[0].y <= 150 && track->speedStarted) {
				cout << " number of fps " << track->numberOfFramesForDistance;
				track->speedStarted = false;
				track->endY = newPts[0].y;
				float delta = track->startY - track->endY;

				cout << " delta: " << delta << " meeters: " << delta / 50 << endl;
				float time = float(track->numberOfFramesForDistance) / 30.0;
				float speed = (delta / (33.333333*time)) * 36.0 / 10.0;
				cout << " speed:  " << speed << " km / h ";
				track->printData = true;
				if (file) {
					*file << track->numberOfFramesForDistance << "," << time << "," << delta << "," << delta / 33.33333 << "," << speed << ",";
					*file << track->roadAlign << "\n";
				}
			}

			rectangle(warpedRoad, Rect(newPts[0].x - 15, newPts[0].y - 15, 30, 30), color, 4);
			std::ostringstream ss;
			ss << track->area << " " << endl;
			ss << "y: " << newPts[0].y;
			std::string str = ss.str();
			putText(warpedRoad, str, Point2i(newPts[0]), CV_FONT_ITALIC, 1, Scalar(255, 255, 255));
			if (newPts[0].y < 450 && newPts[0].y > 0 && newPts[0].x > 50) {
				track->foundBefore = true;

				if (track->foundAfter == false) {
					if (type == RoadType::twoPasmowa) {
						if (track->centroidReadings.size() < 4) {
							track->centroidReadings.push_back(Point2i(newPts[0]));

						}
						else {
							unsigned int mediumX = 0;
							for each(Point2i pt in track->centroidReadings) {
								mediumX += pt.x;
							}
							mediumX /= track->centroidReadings.size();
							if (mediumX < 150) {
								track->roadAlign = cvb::CvTrack::left;
							}
							else {
								track->roadAlign = cvb::CvTrack::right;
							}

						}
					}



					if (track->areaReadings.size() < 4) {
						track->areaReadings.push_back(track->area);

					}
					else if (track->printData && newPts[0].y <= 100) {
						unsigned int fullArea = 0;
						for each(unsigned int area in track->areaReadings) {
							fullArea += area;
						}
						fullArea = fullArea / track->areaReadings.size();
						cout << "road: " << name << " -- found car, area: " << fullArea;
						if (type == RoadType::twoPasmowa) {
							if (track->roadAlign == cvb::CvTrack::left) {
								cout << " pas: lewy ";
							}
							else if (track->roadAlign == cvb::CvTrack::right) {
								cout << " pas: prawy ";
							}
							else {
								cout << " jakies spierdoolony case ";
							}
						}
						cout << "speed" << track->speed << endl;
						track->foundAfter = true;

					}
				}
			}
		}


	}

};



static Mat Homography1;
**/