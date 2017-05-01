//
// Created by fbiedrzy on 5/28/2016.
//

#include <iostream>
#include <helper\Drawer.h>

cv::Mat Drawer::PaintDots(cv::Mat mat)
{
	if (x && y)
	{
		channels = mat.channels();
		std::cout << "Drawing: " << x << " " << y << ". Channels: " << channels << std::endl;
		for (long int c = y; c < y + 20; c++)
		{
			for (long int r = x; r < x + 20; r++)
			{
				uint8_t *pixelPtr = mat.data;
				unsigned long pixelPosition = (c * mat.cols * channels) + (r * channels);
				// bgrPixel.val[0] = 230;
				pixelPtr[pixelPosition] = 233;
				pixelPtr[pixelPosition + 1] = 100;
				pixelPtr[pixelPosition + 2] = 0;
			}
		}
	}
	return mat;
}

void Drawer::setPoint(int x, int y)
{
	this->x = x;
	this->y = y;
}