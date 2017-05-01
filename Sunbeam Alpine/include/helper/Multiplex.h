#pragma once
#include <opencv2\world.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\video.hpp>
#include <IOCallbacks.hpp>
//std header
#include <iostream>
#include <tuple>
// ubik headeres
#include <helper\Logger.hpp>
class Multiplex
{

public:
	Multiplex(int  rows, int cols, cv::Size segment_size, std::string window_name = "Multiplex 3000");

	void Insert(const cv::Mat & frame, const char * title, int col, int row);
	void AddText(const cv::Mat & frame, const char * text, int pos_x, int pos_y, cv::Scalar color);
	void Add(const cv::Mat & frame, const char * title = "Miejsce na Twoja nazwe!");
	// like insert, but simply adds to next free slot
	void StartWritingVideo(std::string filePath);
	void StopWritingVideo();
	void Show();
	~Multiplex();

private:
	const char * _generic_title = "Miejsce na Twoja reklame!";
	std::string _window_name = "";
	const int _max_window_width = 1200;
	const int _max_window_height = 700;
	Multiplex(int rows, int cols, cv::Size segment_size);
	int _cols;
	int _rows;
	// by default all is free
	bool** _segments;
	//const static std::string DEF_WINDOW_NAME = "Multiplex 3000";
	cv::Mat canvas_to_show;
	int _horizontal_spacing = 50;
	int _vertical_spacing = 10;
	cv::VideoWriter outputVideo;
	int _segment_width;
	int _segment_height;
	const int _type = CV_8UC3;
	cv::Mat _canvas;
	std::vector<std::tuple<int, int, std::string>> segmentsWithNames;
	// Mat to show if Add() arg mat is not valid
	cv::Mat _invalid_mat;

	bool writingVideo = false;
};
