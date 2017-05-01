#include <helper\Multiplex.h>

using namespace cv;
#define CV_COLOR_BLUE Scalar(255,0,0)
#define CV_COLOR_RED Scalar(0,0,255)
#define CV_COLOR_BLACK Scalar(0,0,0)
#define CV_COLOR_WHITE Scalar(255,255,255)
#define CV_COLOR_GREEN Scalar(0,255,0)
Multiplex::Multiplex(int rows, int cols, cv::Size segment_size, std::string window_name)
{
	_window_name = window_name;
	_cols = cols;
	_rows = rows;
	_segments = new bool*[_rows];
	
	canvas_to_show = Mat::zeros(_max_window_width, _max_window_height, _type);
	for (int i = 0; i < _rows; i++)
	{
		_segments[i] = new bool[_cols];
		for (int ii = 0; ii < _cols; ii++)
			_segments[i][ii] = true;
	}

	int _canvas_width = (segment_size.width + _vertical_spacing)*_cols - _vertical_spacing;
	int _canvas_height = (segment_size.height + _horizontal_spacing)*_rows;
	_canvas = Mat::zeros(_canvas_height, _canvas_width, _type);
	_segment_height = segment_size.height;
	_segment_width = segment_size.width;
	//setMouseCallback(DEF_WINDOW_NAME, IOCallbacks::drawColourInfoOnClick);
}

void Multiplex::Insert(const Mat & frame, const char* title, int col, int row)
{
	_segments[col][row] = false;
	// chamski resize, powinno byæ z black border
	Mat segment(Mat::zeros(_segment_width, _segment_height, _type));

	resize(frame, segment, cv::Size(_segment_width, _segment_height));
	if (segment.type() != _canvas.type())
		cvtColor(segment, segment, CV_GRAY2RGB);
	Range col_range = Range(col*(_segment_width + _vertical_spacing), (col + 1)*_segment_width + col*_vertical_spacing);
	Range row_range = Range(row*(_segment_height + _horizontal_spacing) + _horizontal_spacing, _horizontal_spacing + (row + 1)*_segment_height + row*_horizontal_spacing);
	if (row_range.end > _canvas.rows || col_range.end > _canvas.cols)
		throw std::out_of_range("Calculated segment position in _cavas is out of range!");
	segment.copyTo(_canvas(row_range, col_range));

	int text_x = col*(_segment_width + _vertical_spacing);
	int text_y = 7 + _horizontal_spacing / 2 + row*(_segment_height + _horizontal_spacing);
	AddText(_canvas, title, text_x, text_y, CV_COLOR_WHITE);

}
void Multiplex::AddText(const Mat& frame, const char* text, int pos_x, int pos_y, Scalar color)
{
	char buffer[50];
	sprintf(buffer, text);
	putText(frame, buffer, cv::Point(pos_x, pos_y), CV_FONT_NORMAL, 1, color);
}

void Multiplex::Add(const cv::Mat & frame, const char* title)
{
	bool invalid_argument = false;

	if (frame.rows == 0 || frame.cols == 0)
	{

		invalid_argument = true;
		//throw std::invalid_argument("Can't add empty frame to mpx!");
		_invalid_mat = Mat::zeros(_segment_height, _segment_width, _type);
		AddText(_invalid_mat, "Rows OR cols are 0!", 0, 25, CV_COLOR_RED);
	}

	// check if existing 
	for each(std::tuple<int, int, std::string> pair in segmentsWithNames) {
		std::string name;
		int i;
		int ii;
		std::tie(i, ii, name) = pair;
		if (name.compare(title) == 0) {
			Insert(frame, title, i, ii);
			return;
		}		
	}
	


	for (int i = 0; i < _cols; i++)
		for (int ii = 0; ii < _rows; ii++)
			if (_segments[i][ii])
			{
				if (invalid_argument)
					Insert(_invalid_mat, title, i, ii);
				else
					Insert(frame, title, i, ii);
					segmentsWithNames.push_back(std::make_tuple(i, ii, String(title)));
				return;
			}
	throw std::out_of_range("Can't add more screens to multiplex!");
}
void Multiplex::StartWritingVideo(std::string filePath)
{

	if (!writingVideo) {		
		Size S = Size(_max_window_width, _max_window_height);
		int dupa /**xD**/ = 828601953;
		char EXT[] = { (char)(dupa & 0XFF),(char)((dupa & 0XFF00) >> 8),(char)((dupa & 0XFF0000) >> 16),(char)((dupa & 0XFF000000) >> 24),0 };
		outputVideo = VideoWriter(filePath, VideoWriter::fourcc(EXT[0], EXT[1], EXT[2], EXT[3]), 30, S, true);  
		//outputVideo.open(outputeFilename, ex, capture.get(CV_CAP_PROP_FPS), S, true);
		if (!outputVideo.isOpened())
		{
			std::cout << "Could not open the output video for write: " << filePath << std::endl;
			Logger::AddMessage("could not open \n", __func__);
			return ;
		}
		else {
			Logger::AddMessage("ouput stream opened \n", __func__);
			writingVideo = true;
		}
		
	}
	
}
void Multiplex::StopWritingVideo()
{
	Logger::AddMessage("releasing video \n", __func__);
	
	outputVideo.release();
	
}
void Multiplex::Show()
{

	int new_width = _max_window_width;
	int new_height = _max_window_height;
	if (_canvas.size().width > _max_window_width)
	{
		float ratio = 1 / (float)(_canvas.size().width / (float)_max_window_width);
		new_height *= ratio;
	}


	if (_canvas.size().height > _max_window_height)
	{
		float ratio = 1 / (float)(_canvas.size().height / (float)_max_window_height);
		new_width *= ratio;

	}
	
	resize(_canvas, canvas_to_show, cv::Size(_max_window_width, _max_window_height));
	imshow(_window_name, canvas_to_show);
	if (writingVideo) {
		outputVideo << canvas_to_show;
	}
}
Multiplex::~Multiplex()
{
	if (writingVideo) {
		//StopWritingVideo();
	}
}