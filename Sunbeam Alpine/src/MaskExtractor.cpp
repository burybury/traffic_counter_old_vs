#include <MaskExtractor.h>
#include <math.h>

MaskExtractor::MaskExtractor() {
	namedWindow(MISSION_CONTROL_W_NAME);
}

MaskExtractor::~MaskExtractor() {
	destroyWindow(MISSION_CONTROL_W_NAME);
}
using namespace std;
vector<Vec3b> bgcolors;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")";

		Mat* mat = static_cast<Mat*>(userdata);

		Vec3b color = mat->at<Vec3b>(Point(x, y));


		bgcolors.push_back(color);
		cout << " Color: " << (int)color[0] << ":" << (int)color[1] << ":" << (int)color[2] << endl;

	}
}
void
MaskExtractor::HsvMask(
	_In_ const cv::Mat& BGR_source,
	_Out_ Mat& and_CH1_CH2
)
{

	Size2i mpxSize(300, 300);
	Multiplex mpx(3, 3, mpxSize, "recoloured");

	//mpx.Add(HSV_source, "resized_hsv");

	// color segmentation
	Mat mean_shifted;
	//pyrMeanShiftFiltering(BGR_source, mean_shifted, _sp, _sr, _maxLevel);
//	Mat blurred;
//	bilateralFilter(BGR_source, blurred, 5, 2, 20);
//	imshow("bgr blur",blurred);
	//mpx.Add(mean_shifted, "mean_shifted");

	// split into channels

	Mat HSV_source;
	cv::cvtColor(BGR_source, HSV_source, cv::COLOR_BGR2HSV);
	//medianBlur(HSV_source, mean_shifted, 7);
	//BGR_source.copyTo(HSV_source);
	imshow("bgr", BGR_source);
	imshow("hsv ", HSV_source);
	//imshow("mean", mean_shifted);
	std::vector<Mat> channels(3);
	split(HSV_source, channels);
	const Mat ch1 = channels[0];
	const Mat ch2 = channels[1];
	const Mat ch3 = channels[2];
	mpx.Add(ch1, "ch1");
	mpx.Add(ch2, "ch2");
	mpx.Add(ch3, "ch3");


	// threshold 'not-road' as precise as possible
	Mat ch1_thresholded;
	Mat ch2_thresholded;
	threshold(ch1, ch1_thresholded, _threshold1, 255, 0);
	threshold(ch2, ch2_thresholded, _threshold2, 255, 1);
	mpx.Add(ch1_thresholded, "ch1 t");
	mpx.Add(ch2_thresholded, "ch2 t");
	// 'ch1 & ch2' for filtering
	Mat and_ch1_ch2;
	bitwise_and(ch1_thresholded, ch2_thresholded, and_ch1_ch2);
	//mpx.Add(and_CH1_CH2, "and_ch1_ch2");
	imshow("and_ch1_ch2", BGR_source);
	setMouseCallback("and_ch1_ch2", CallBackFunc, (void*)&BGR_source);
	Mat merged = Mat::zeros(BGR_source.size(), CV_8UC1);
	int treshold3 = 15;




















	for (int row = 0; row < BGR_source.rows; row++)
		for (int col = 0; col < BGR_source.cols; col++)
		{
			Vec3b color = BGR_source.at<Vec3b>(Point(col, row));
			for (auto i = 0; i != bgcolors.size(); i++)
			{
				if (std::abs(color[0] - bgcolors[i][0]) < treshold3)
					if (std::abs(color[1] - bgcolors[i][1]) < treshold3)
						if (std::abs(color[2] - bgcolors[i][2]) < treshold3)
							merged.at<uchar>(Point(col, row)) = 255;
			}
		}

	imshow("PLAEASE", merged);
	
	static Mat superliner;


	if (empty(superliner))
	{
		merged.copyTo(superliner);
	}
	else
		bitwise_or(merged, superliner, superliner);


	imshow("superliner", superliner);






	ch2_thresholded.copyTo(and_CH1_CH2);
	// remove bad pixels

	// mark largest contour as road
	//Mat road_mask = largest_contour_mask(morph_opened);
	//mpx.Add(road_mask, "road_mask");

	//CreateMissionControl();
	mpx.Show();

}

using namespace std;

cv::Mat MaskExtractor::largest_contour_mask(const cv::Mat& src)
{
	double area{ 0 }, largest{ 0 };
	double length{ 0 }, longest{ 0 };
	int largestIndex{ 0 }, longestIndex{ 0 };

	vector< vector< cv::Point> > contours;
	findContours(src, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	//Mat draw_contours = Mat::zeros(src.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); ++i)
	{
		area = contourArea(contours[i]);
		if (area > largest) {
			largest = area;
			largestIndex = i;
		}
		length = arcLength(contours[i], true);
		if (length > longest) {
			longest = length;
			longestIndex = i;
		}
		//// default all to grey
		//drawContours(draw_contours, contours, i, Scalar(100, 100, 100), 2, 8);
	}

	//// largest area is blue
	//drawContours(draw_contours, contours, largestIndex, Scalar(255, 0, 0), 2, 8);

	//// longest area is red
	//drawContours(draw_contours, contours, longestIndex, Scalar(0, 0, 255), 2, 8);

	/// largest is bound with green rect
	//Rect bounding_rect_largest = boundingRect(contours[largestIndex]);
	//cv::rectangle(draw_contours, bounding_rect_largest, Scalar(0, 255, 0), 2);

	Mat mask = Mat::zeros(src.size(), CV_8UC1);
	cv::drawContours(mask, contours, largestIndex, Scalar(255, 255, 255), -1);
	return mask;
}

cv::Mat MaskExtractor::morphAndRetrieveRoadMask(const cv::Mat & src)
{
	int morph_size = 2;
	Mat morphed;
	Mat element = getStructuringElement(MorphShapes::MORPH_CROSS, Size(morph_size, morph_size));
	Size tempSize = Size(src.cols, src.rows);
	Mat resized;
	resize(src, resized, Size(), 0.5, 0.5);
	rectangle(resized, Rect(0, 0, resized.cols, resized.rows*0.3), Scalar(0), -1);
	///Apply the 'opening' morphology operation
	morphologyEx(resized, morphed, MORPH_OPEN, element, cv::Point(-1, -1), 2, BORDER_CONSTANT);

	// 'close' artifacts on the road
	morph_size = 5;
	element = getStructuringElement(MorphShapes::MORPH_CROSS, Size(morph_size, morph_size));
	morphologyEx(morphed, morphed, MORPH_CLOSE, element, cv::Point(-1, -1), 2, BORDER_CONSTANT);

	threshold(morphed, morphed, 230, 255, THRESH_BINARY);
	//imshow("morphed", morphed);
	Mat roadMaskSmall;
	roadMaskSmall = largest_contour_mask(morphed);


	Mat maskBigSize;
	resize(roadMaskSmall, maskBigSize, tempSize);
	imshow("roadMaskSmall", maskBigSize);
	return maskBigSize;
}
