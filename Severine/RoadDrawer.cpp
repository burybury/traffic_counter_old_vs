#include "RoadDrawer.h"


RoadDrawer::RoadDrawer(Camera& cam) : mCam(cam) , newRoadBorders(false)
{
	try {
		cvNamedWindow(cam.getName().c_str());
		
				
	}
	catch (std::bad_alloc)
	{
		std::cout << "could not allocate memory for roaddrawer \n" << std::endl;
		return;
	}
}

void
RoadDrawer::setCallbackForWindow(RoadDrawer* rd)
{
	setMouseCallback(mCam.getName().c_str(), CallBackFunc, static_cast<void*>(rd));
}


void 
RoadDrawer::addRoad()
{
	roads.push_back(Road());
	Road* road = &roads.back();
	for (auto& pt : road->polygonPoints)
	{
		ResizeRoadButton* resizeButton = new ResizeRoadButton{
			pt,
			[this](void* button,Point p) {
			RoadDrawer* resizeRoadButton = static_cast<RoadDrawer*>(button);
			std::cout << "resize selected " << std::endl;
			return true;
		}
		};
		buttons.push_back(resizeButton);
	}	
	newRoadBorders = true;
}

void
RoadDrawer::showWindow()
{
	Mat frame;
	mCam.getFrame().copyTo(frame);
	if (!frame.empty())
	{
		for (auto& road : roads)
		{
			road.draw(frame);
		}
		drawButtons(frame);
	}
	imshow(mCam.getName().c_str(),frame);
}




RoadDrawer::~RoadDrawer()
{
	for (auto* button : buttons) 
	{
		delete button;
	}
}

void 
RoadDrawer::CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	// ony mouseclick event 
	if (event != EVENT_LBUTTONDOWN && event != EVENT_RBUTTONDOWN)
	{
		return;
	}
	//static void(OpencvButton::*)(Point) callbackFunction;
	RoadDrawer* rd = static_cast<RoadDrawer*>(userdata);
	rd->drawButtons(rd->getCam().getFrame());
	
	for (std::vector<OpencvButton*>::reverse_iterator it = rd->getButtons().rbegin(); it != rd->getButtons().rend(); ++it)
	{

		if ((*it)->getRect().contains(Point2i(x, y))			)
		{

			if ((*it)->fun(rd, Point(x, y)))
			{
				rd->selectedButton = (*it);
			}
			else
			{
				rd->selectedButton = nullptr;
			}
			return;
		}
	}

	for (OpencvButton* button : rd->getButtons())
	{
		
	}

	if (rd->selectedButton != nullptr)
	{
		rd->selectedButton->callBack(Point(x, y));
		rd->selectedButton->setSelected(false);
		rd->selectedButton = nullptr;
	}

	std::cout << "nothing clicked \n" << std::endl;
	/**
	if (rd->getCurrentFunction() == ClickableButton::ButtonFunction::ADD_ROAD)
	{
		std::cout << "Adding road: ";
		if (event == EVENT_LBUTTONDOWN)
		

			std::cout << "Left button  :" << rd->getCam().getName() << "(" << x << ", " << y << ")" << std::endl;
		
		}
		
	}
	else if (rd->getCurrentFunction() == ClickableButton::ButtonFunction::SET_BORDER_CLICKED_BOTTOM)
	{
		std::cout << "Bottom border: ";
		if (event == EVENT_LBUTTONDOWN)
		{

			std::cout << "Left button  :" << rd->getCam().getName() << "(" << x << ", " << y << ")" << std::endl;
	
		}
		else if (event == EVENT_RBUTTONDOWN)
		{
			std::cout << "Right button  :" << rd->getCam().getName() << "(" << x << ", " << y << ")" << std::endl;
		
		}
	}
	else
	{
		std::cout << "No function selected \n" << std::endl;
	}
	**/
}

void RoadDrawer::makeButtons() {
	
	RoadDrawerButton* addRoadButton = new RoadDrawerButton{
		std::string{"add road"},
		Point2i(10,10),
		[this](void* rd,Point p) {
			RoadDrawer* roadDrawer = static_cast<RoadDrawer*>(rd);
			std::cout << "add road clicked " << std::endl;
			this->addRoad();
			return false;
		}
	
	};
	buttons.push_back(addRoadButton);

	RoadDrawerButton* newHomographyButton = new RoadDrawerButton{
		std::string{"new homography"},
		Point2i(10,50),
		[this](void* rd,Point p) {
		RoadDrawer* roadDrawer = static_cast<RoadDrawer*>(rd);
			std::cout << "new homography clicked " << std::endl;
			roadDrawer->setNewRoadBorder(true);
			return false;
		}
	};

	buttons.push_back(newHomographyButton);

	RoadDrawerButton* processBgsButton = new RoadDrawerButton{
		std::string{"bgs on"},
		Point2i(10,90),
		[this](void* rd,Point p) {
		RoadDrawer* roadDrawer = static_cast<RoadDrawer*>(rd);
		std::cout << "bgs process on" << std::endl;
		roadDrawer->getCam().setFinalRoad(true);
		return false;
		}
	};

	buttons.push_back(processBgsButton);

}


/***
bool 
RoadBorder::pointNotZero(Point2i & p) {
	if (p.x != 0 && p.y != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Mat& 
RoadBorder::draw(Mat& frame)
{
	drawTopBorder(frame);
	drawBottmBorder(frame);
	
	//	std::array<Point2i,4> polygonPoints;

	return frame;
}
bool 
RoadBorder::pointsNotZeroes() {
	if (pointNotZero(mLeft) && pointNotZero(mRight))
	{
		return true;
	}
	else
	{
		return false;
	}
}
**/