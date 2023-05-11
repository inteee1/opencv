#include <opencv2/opencv.hpp>
void arithemtic()
{
    cv::Mat src1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat src2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
    if(src1.empty() or src2.empty()) return;
    cv::imshow("SRC1", src1);
    cv::imshow("SRC2", src2);
    cv::Mat dst;
    cv::add(src1, src2, dst);
    cv::imshow("ADD", dst);
    cv::Mat dst2;
    cv::addWeighted(src1, 0.5, src2, 0.5, 0, dst2);
    cv::imshow("ADDWEIGHTED", dst2);
    cv::Mat dst3;
    cv::subtract(src1, src2, dst3);
    cv::imshow("SUBTRACT", dst3);
    cv::Mat dst4;
    cv::absdiff(src1, src2, dst4);
    cv::imshow("ABSDIFF", dst4);
    cv::waitKey();
    cv::destroyAllWindows();

}

void logical()
{
    cv::Mat src1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat src2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
    if(src1.empty() or src2.empty()) return;
    cv::imshow("SRC1", src1);
    cv::imshow("SRC2", src2);
    cv::Mat dst;
    cv::bitwise_and(src1, src2, dst);
    cv::imshow("BITWISE_AND", dst);
    cv::Mat dst2;
    cv::bitwise_or(src1, src2, dst2);
    cv::imshow("BITWISE_OR", dst2);
    cv::Mat dst3;
    cv::bitwise_xor(src1, src2, dst3);
    cv::imshow("BITWISE_XOR", dst3);
    cv::Mat dst4;
    cv::bitwise_not(src1, dst4);
    cv::imshow("BITWISE_NOT", dst4);
    cv::waitKey();
    cv::destroyAllWindows();

}


void filter_embossing()
{
    cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
    if(src.empty())
    {
        std::cerr << "File does not exist" << std::endl;
    }
    float filter_data[] = { -1.0f, -1.0f, 0.f, -1.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f};
    cv::Mat emboss(3, 3, CV_32FC1, filter_data);

    cv::Mat dst;
    cv::filter2D(src, dst, -1, emboss, cv::Point(-1, -1), 128);

    cv::imshow("SRC", src);
    cv::imshow("DST", dst);
    cv::waitKey();
    cv::destroyAllWindows();
}

void bluring_mean()
{
    cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
    if(src.empty())
    {
        std::cerr << "File does not exist" << std::endl;
    }
    //float filter_data[] = { -1.0f, -1.0f, 0.f, -1.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f};
    //cv::Mat emboss(3, 3, CV_32FC1, filter_data);
    cv::imshow("SRC", src);
    cv::Mat dst;
    //cv::filter2D(src, dst, -1, emboss, cv::Point(-1, -1), 128);
    for( int ksize = 3; ksize <= 7; ksize += 2)
    {
        cv::blur(src, dst, cv::Size(ksize, ksize));
        cv::String desc = cv::format("Mean: %dx%d", ksize, ksize);
        cv::putText(dst, desc, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1 , cv::LINE_AA);
        cv::imshow("DST", dst);
        cv::waitKey();
    }
    
    
   
    cv::destroyAllWindows();
}
void bluring_gaussian()
{

    cv:: Mat src = cv::imread("아리.png", cv::IMREAD_GRAYSCALE);
    if(src.empty()) return;
    cv::imshow("SRC", src);
    cv::Mat dst;
    for(int sigma = 1; sigma <= 5; ++sigma)
    {
        cv::GaussianBlur(src, dst, cv::Size(), static_cast<double>(sigma));
        cv::String text = cv::format("Sigma = %d", sigma);
        cv::putText(dst, text, cv::Point(30,30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
        cv::imshow(text, dst);
        cv::waitKey();
    }
    cv::destroyAllWindows();
}

void unsharp_mask()
{
    cv::Mat src = cv::imread("가렌.png", cv::IMREAD_GRAYSCALE);
    if(src.empty()) return;
    cv::imshow("SRC", src);
    cv::Mat blurred;
    float alpha = 1.0f;
    for (int sigma = 1; sigma <= 5; sigma++)
    {
        
        cv::GaussianBlur(src, blurred, cv::Size(), sigma);
        cv::Mat dst = (1 + alpha) * src - (alpha * blurred);
        cv::String desc = cv::format("sigma : %d", sigma);
        cv::putText(dst, desc, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
        cv::imshow(desc, dst);
        cv::waitKey();
    }
}

void noise_gapussian()
{
   cv::Mat src = cv::imread("럭스.png", cv::IMREAD_GRAYSCALE);
   if(src.empty()) return;
   cv::imshow("src", src);
   cv::Mat dst;
   for(int stddev = 10; stddev <= 30; stddev += 10)
   {
        cv::Mat noise(src.size(), CV_32SC1);
        cv::randn(noise, 0 , stddev);
        cv::add(src, noise, dst, cv::Mat(), CV_8U);
        cv::String desc = cv::format("stddev %d", stddev);
        cv::putText(dst, desc, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
        cv::imshow(desc, dst);
        cv::waitKey();
   }
    cv::destroyAllWindows();
}

void filter_bilaterial()
{
    cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    if(src.empty()) return;
    cv::Mat dst1; //Gasussian Filter 
    cv::Mat dst2; //Bilateral Filter
    cv::Mat noise(src.size(), CV_32SC1);
    cv::randn(noise, 0, 5);
    cv::add(src, noise, src, cv::noArray(), CV_8U);
    cv::GaussianBlur(src, dst1, cv::Size(), 5);
    cv::bilateralFilter(src, dst2, -1, 10, 5);
    cv::imshow("Noise SRC", src);
    cv::imshow("Gaussian", dst1);
    cv::imshow("bilateral", dst2);
    cv::waitKey();
    cv::destroyAllWindows();
    
    
}

void filter_median()
{
    cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    int nums = (int)(src.total() * 0.1);
    for (int i = 0; i < nums; ++i)
    {
        int x = rand() % src.cols;
        int y = rand() % src.rows;
        src.at<uchar>(y, x) = (i % 2) * 255;
    }
    cv::Mat dst1;
    cv::GaussianBlur(src, dst1, cv::Size(), 1);

    cv::Mat dst2;
    cv::medianBlur(src, dst2, 3); // 3x3
    cv::imshow("Salt & Pepper SRC", src);
    cv::imshow("Gaussian", dst1);
    cv::imshow("Median", dst2);
    cv::waitKey();
    cv::destroyAllWindows();


}

void affine_transform()
{
    cv::Mat src = cv::imread("tekapo.bmp");
    if(src.empty()) return;
    cv::Point2f srcPts[3];
    cv::Point2f dstPts[3];

    srcPts[0] = cv::Point2f(0.0f, 0.0f);
    srcPts[1] = cv::Point2f(src.cols -1.0f, 0.0f);
    srcPts[2] = cv::Point2f(src.cols -1.0f, src.rows - 1.0f);
    dstPts[0] = cv::Point2f(50.0f, 50.0f);
    dstPts[1] = cv::Point2f(src.cols - 100.0f, 100.0f);
    dstPts[2] = cv::Point2f(src.cols - 50.0f, src.rows - 50.0f);

    cv::Mat M = cv::getAffineTransform(srcPts, dstPts);

    cv::Mat dst;
    cv::warpAffine(src, dst, M, cv::Size());

    cv::imshow("SRC", src);
    cv::imshow("DST", dst);
    cv::waitKey();
    cv::destroyAllWindows();

}

void affine_translation()
{
    cv::Mat src = cv::imread("tekapo.bmp");
    cv::Mat M = cv::Mat_<float>({2, 3}, {1.0, 0.0, 150.0, 0.0, 1.0, 100.0});
    cv::Mat dst;
    cv::warpAffine(src, dst, M, cv::Size());
    imshow("SRC", src);
    imshow("DST", dst);
    cv::waitKey();
    cv::destroyAllWindows();
}