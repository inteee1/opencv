#include <opencv2/opencv.hpp>

void brightness1()
{
    cv::Mat src = cv::imread("lenna.bmp",cv::IMREAD_GRAYSCALE);
    cv::Mat dst = src - 100;
    cv::imshow("SRC", src);
    cv::imshow("dst", dst);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void brightness3()
{
    cv::Mat src = cv::imread("lenna.bmp",cv::IMREAD_GRAYSCALE);
    cv::Mat dst (src.rows, src.cols, src.type());
    // cv::Mat dst(512,512,CV_8UC1);
    for(int j = 0; j < src.rows; ++j)
    {
        for(int i = 0; i < src.cols; ++i)
        {
            dst.at<uchar>(j,i) = cv::saturate_cast<uchar>(src.at<uchar>(j, i) - 50);
        }

    }
    cv::imshow("SRC", src);
    cv::imshow("DST", dst);
    cv::waitKey(0);
    cv::destroyAllWindows();

}

static void on_brightness_changed(int pos, void* userdata)
{
    cv::Mat src = *(static_cast<cv::Mat*>(userdata));
    cv::Mat dst = src + pos;
    cv::imshow("CANVAS", dst);
}




void brightness4()
{
    cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    cv::namedWindow("CANVAS");
    cv::createTrackbar("Brightness", "CANVAS", 0, 100, on_brightness_changed, (void*)&src);
    cv::imshow("CANVAS", src);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void show_hist()
{
    cv::Mat src = cv::imread("hawkes.bmp", cv::IMREAD_GRAYSCALE);
    auto calcGrayHist = [](const cv::Mat& img)
    {
        cv::Mat hist;
        int channels[] = {0};
        int dims = 1;
        int histSize[] = {256};
        float graylevel[] = {0, 256};
        const float* ranges[] = {graylevel};
        cv::calcHist(&img, 1, channels, cv::noArray(), hist, dims, histSize, ranges);
        return hist;
    };
    cv::Mat hist1 = calcGrayHist(src);
    auto getGrayHistImage = [](const cv::Mat& hist)
    {
        double histMax;
        cv::minMaxLoc(hist, 0, &histMax);
        cv::Mat imgHist(100, 256, CV_8UC1, cv::Scalar(256));
        for (int i = 0; i < 256; ++i)
        {
            cv::line(imgHist, cv::Point(i, 100), cv::Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), cv::Scalar(0));
        }
        return imgHist;
    };
    cv::Mat hist_img = getGrayHistImage(hist1);
    cv::imshow("SRC", src);
    cv::imshow("HISTOGRAM", hist_img);
    cv::waitKey(0);
    cv::destroyAllWindows();

}

void histogram_streching()
{
    cv::Mat src = cv::imread("hawkes.bmp", cv::IMREAD_GRAYSCALE);
    double gmin = 0.0;
    double gmax = 0.0;
    cv::minMaxLoc(src, &gmin, &gmax);
    cv::Mat dst = ((src - gmin) / (gmax - gmin)) * 255;
    if(src.empty())
    {
        std::cerr << "File does not exist" << std::endl;
        return;
    }
    auto calcGrayHist = [](const cv::Mat& img)
    {
        cv::Mat hist;
        int channels[] = {0};
        int dims = 1;
        int histSize[] = {256};
        float graylevel[] = {0, 256};
        const float* ranges[] = {graylevel};
        cv::calcHist(&img, 1, channels, cv::noArray(), hist, dims, histSize, ranges);
        return hist;
    };
    cv::Mat hist1 = calcGrayHist(src);
    auto getGrayHistImage = [](const cv::Mat& hist)
    {
        double histMax;
        cv::minMaxLoc(hist, 0, &histMax);
        cv::Mat imgHist(100, 256, CV_8UC1, cv::Scalar(256));
        for (int i = 0; i < 256; ++i)
        {
            cv::line(imgHist, cv::Point(i, 100), cv::Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), cv::Scalar(0));
        }
        return imgHist;
    };
    cv::Mat hist_img = getGrayHistImage(hist1);
    cv::imshow("SRC", src);
    cv::imshow("HISTOGRAM", hist_img);
    cv::imshow("DST", dst);
    cv::imshow("DST_HISTOGRAM", getGrayHistImage(calcGrayHist(dst)));
    cv::waitKey(0);
    cv::destroyAllWindows();

}

void histogram_equlaization()
{
    cv::Mat src = cv::imread("hawkes.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat dst;
    if(src.empty())
    {
        std::cerr << "File does not exist" << std::endl;
        return;
    }
    
    auto calcGrayHist = [](const cv::Mat& img)
    {
        cv::Mat hist;
        int channels[] = {0};
        int dims = 1;
        int histSize[] = {256};
        float graylevel[] = {0, 256};
        const float* ranges[] = {graylevel};
        cv::calcHist(&img, 1, channels, cv::noArray(), hist, dims, histSize, ranges);
        return hist;
    };
    cv::Mat hist1 = calcGrayHist(src);
    auto getGrayHistImage = [](const cv::Mat& hist)
    {
        double histMax;
        cv::minMaxLoc(hist, 0, &histMax);
        cv::Mat imgHist(100, 256, CV_8UC1, cv::Scalar(256));
        for (int i = 0; i < 256; ++i)
        {
            cv::line(imgHist, cv::Point(i, 100), cv::Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), cv::Scalar(0));
        }
        return imgHist;
    };
    cv::Mat hist_img = getGrayHistImage(hist1);
    cv::imshow("SRC", src);
    cv::imshow("HISTOGRAM", hist_img);
    cv::equalizeHist(src, dst);
    cv::imshow("DST", dst);
    cv::imshow("DST_HISTOGRAM", getGrayHistImage(calcGrayHist(dst)));
    cv::waitKey(0);
    cv::destroyAllWindows();

}