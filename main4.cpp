#pragma Once
#include <opencv2/opencv.hpp>

static void on_level_changed(int, void*);

void trackbar_event()
{
    cv::Mat canvas(800, 800, CV_8UC3);//grayscale
    cv::namedWindow("CANVAS");
    cv::createTrackbar("Level", "CANVAS", 0, 16, on_level_changed, (void*)&canvas);
    cv::imshow("CANVAS", canvas);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void on_level_changed(int pos, void* user_data)
{
    cv::Mat img = *(static_cast<cv::Mat*>(user_data));
    img.setTo(pos * 16);
    cv::imshow("CANVAS", img);
}

void mask_setTo()
{
    cv::Mat src = cv::imread("lenna.bmp");
    cv::Mat mask = cv::imread("mask_smile.bmp", cv::IMREAD_GRAYSCALE);
    if(src.empty() or mask.empty()) return;
    src.setTo(cv::Scalar(0, 0, 255), mask);
    cv::imshow("LENNA",src);
    cv::imshow("MASK",mask);
    cv::waitKey(0); 
    cv::destroyAllWindows();

}

void mask_copyTo()
{
    cv::Mat src = cv::imread("airplane.bmp");
    cv::Mat mask = cv::imread("mask_plane.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat dst = cv::imread("field.bmp");
    if(src.empty() or mask.empty() or dst.empty()) return;
    src.copyTo(dst, mask);
    cv::imshow("DST", dst);
    cv::imshow("SRC",src);
    cv::imshow("mask", mask);
    cv::waitKey(0);
    cv::destroyAllWindows();

}



