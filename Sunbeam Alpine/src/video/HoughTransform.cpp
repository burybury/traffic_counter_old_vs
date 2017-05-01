#include <video\HoughTransform.hpp>
using namespace cv;
void HoughTransform::perform(const cv::Mat& src)
{
    Mat dst, cdst;
    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);

    std::vector<Vec2f> lines;
    // detect lines
    HoughLines(dst, lines, 1, CV_PI / 180, 300, 0, 0);

    // draw lines
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
    }

    //imshow("source", src);
    imshow("hough transform", cdst);
}