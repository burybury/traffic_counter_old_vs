#pragma once

#include "Camera.h"
#include "ClickableButton.h"
#include "Road.h"
#include "Button.h"
#include <iostream>
#include <functional>

using namespace cv;

class RoadDrawer;

class RoadDrawerButton : public OpencvButton
{
private:
	Point mTextPosition;
	std::string mName;
public:
	
	//std::function<void(RoadDrawer*, Point)> fun;
	RoadDrawerButton(
		std::string name,
		Point position,
		std::function<bool(void*, Point)> fun) :
		OpencvButton(position, fun), mName(name)
	{
		size = Size(160, 40);
		mButtonRect = Rect(position, size);
		unsigned int x = position.x;
		unsigned int y = position.y;
		x += 10;
		y += 35;
		mTextPosition = Point2i(x, y);
	}
	void callBack(Point) {
		std::cout << "empty callback " << std::endl;
	};
	void draw(Mat& frame)
	{
		if (selected)
		{
			rectangle(frame, mButtonRect, colorSelected,2);
		}
		else
		{
			rectangle(frame, mButtonRect, colorUnselected,2);
		}
		putText(frame, mName, mTextPosition, HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0),2);
	}
};

class ResizeRoadButton : public OpencvButton
{
private:
	Point2i& referencedPosition;
	//std::function<void(RoadDrawer*, Point)> fun;
	Size size{ 10,10 };
public:
	ResizeRoadButton(Point& referencedPosition,
		std::function<bool(void*, Point)> fun):
		OpencvButton(referencedPosition,fun),
		referencedPosition(referencedPosition)
	{
		
		size = Size(10, 10);
		mButtonRect = Rect(referencedPosition, size);
	}
	void draw(Mat& frame)
	{
		if (selected)
		{
			rectangle(frame, mButtonRect, colorSelected);
		}
		else
		{
			rectangle(frame, mButtonRect, colorUnselected);
		}
	}
	void callBack(Point pt)
	{
		referencedPosition.x = pt.x;
		referencedPosition.y = pt.y;
		mButtonRect = Rect(referencedPosition, size);
		selected = false;
		std::cout << "resize utton callback" << std::endl;
	}
};

class RoadDrawer
{
private:
	Camera& mCam;
	//std::vector<ClickableButton> buttons;
	OpencvButton* selectedButton = nullptr;
	std::vector<OpencvButton*> buttons;
	std::vector<Road> roads;
	cv::MouseCallback callBack;
	static void CallBackFunc(int event, int x, int y, int flags, void* userdata);	
	bool newRoadBorders;
	void drawButtons(Mat& frame)
	{
		for (auto& button : buttons)
		{
			button->draw(frame);
		}

	}
public:
	void setNewRoadBorder(bool state)
	{
		newRoadBorders = state;
	}
	bool newRoadBorder() { return newRoadBorders; };
	void makeButtons();
	void (OpencvButton::*selectedButtonCB)(Point point) = nullptr;
	RoadDrawer() = delete;
	RoadDrawer(const RoadDrawer&) = default;
	RoadDrawer(Camera& cam);
	void addRoad();
	Camera& getCam() { return mCam; };	
	void showWindow();
	void setCallbackForWindow(RoadDrawer* rd);
	std::vector<OpencvButton*>& getButtons() { return buttons; };	
	void setSelectedButton(OpencvButton* button)
	{
		selectedButton = button;
	}
	std::vector<Road>& getRoads() { return roads; };
	~RoadDrawer();
};

