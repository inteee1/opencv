#include <opencv2/opencv.hpp>

int main()
{
    std::cout << "Hello OpenCV\n" << CV_VERSION << std::endl;

    class cv::Mat image; // 객체
    image = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    if(image.empty()) {
            std::cout << "A file is not found " << std::endl;
            return 1;
    }
    cv::namedWindow("LENNA");
    cv::imshow("LENNA", image);
    cv::waitKey(0);
    cv::destroyWindow("LENNA");
        return 0;
}
