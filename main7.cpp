#include <opencv2/opencv.hpp>
using namespace cv;
void affine_scale()
{
    Mat src = imread("rose.bmp");
    if(src.empty()) return;
    Mat dst1;
    Mat dst2;
    Mat dst3;
    Mat dst4;
    resize(src, dst1, Size(), 4, 4, INTER_NEAREST);
    resize(src, dst2, Size(1920, 1280));
    resize(src, dst3, Size(1920, 1280), 0, 0, INTER_CUBIC);
    resize(src, dst4, Size(1920, 1280), 0, 0, INTER_LANCZOS4);

    imshow("SRC", src);
    imshow("DST1", dst1(Rect(400, 500, 400, 400)));
    imshow("DST2", dst2(Rect(400, 500, 400, 400)));
    imshow("DST3", dst3(Rect(400, 500, 400, 400)));
    imshow("DST4", dst4(Rect(400, 500, 400, 400)));
    waitKey();
    destroyAllWindows();
}

void affine_rotation()
{
    Mat src = imread("tekapo.bmp");
    if(src.empty()) return;
    Point2f cp(src.cols / 2.f, src.rows / 2.f);
    Mat M = getRotationMatrix2D(cp, 90, 1);
    Mat dst;
    warpAffine(src, dst, M, Size());
    imshow("SRC", src);
    imshow("DST", dst);
    waitKey();
    destroyAllWindows();

}

void affine_filp()
{
    Mat src = imread("럭스.png");
    if(src.empty()) return;
    imshow("SRC", src);
    Mat dst;
    int flipCode[] = { 1/*좌우대칭*/, 0/*상하대칭*/, -1/*상하좌우대칭*/};
    for (int i = 0; i < 3; ++i)
    {
        flip(src, dst, flipCode[i]);
        String desc = format("flipCode: %d", flipCode[i]);
        putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 1, LINE_AA);
        imshow("DST", dst);
        waitKey();
    }
     destroyAllWindows();
}




Mat src;
Point2f savesrc[4], savedst[4];
void on_mouse2(int, int, int, int, void*);


void on_mouse2(int event, int x, int y, int flags, void* )
{
    static int cnt = 0;
    if(event == EVENT_LBUTTONDOWN ) {
        if(cnt < 4) {
            savesrc[cnt++] = Point2f(x, y);
            circle(src, Point2f(x, y), 5, Scalar(255, 255, 255), -1);
            imshow("SRC", src);

            if(cnt == 4) {
                int w = 200, h = 300;

                savedst[0] = Point2f(0, 0);
                savedst[1] = Point2f(w - 1, 0);
                savedst[2] = Point2f(w - 1 , h - 1);
                savedst[3] = Point2f(0, h - 1);

                Mat pers = getPerspectiveTransform(savesrc, savedst);

                Mat dst;
                warpPerspective(src, dst, pers, Size(w, h));
                imshow("DST", dst);

            }
        }  
    }
}

void perspective()
{
    src = imread("card.bmp");
    if(src.empty()) return;
    namedWindow("SRC");
    setMouseCallback("SRC", on_mouse2);
    imshow("SRC", src);
    waitKey();
    destroyAllWindows();

}

void perspective2()
{
    static Point2f dstQuard[4];
    static Point2f srcQuard[4];
    static Mat src = imread("card.bmp");
    static int count = 0;
    imshow("SRC", src);
    auto on_mouse3 = [](int event, int x, int y, int flags, void* userdata) 
    {
        if(event == EVENT_LBUTTONDOWN )
        {
            imshow("SRC", src);
            if(count < 4) {
                srcQuard[count++] = Point2f(x, y);
                circle(src, Point2f(x, y), 5, Scalar(0, 0, 255), -1);
                imshow("SRC", src);

                    if(count == 4) 
                    {
                        int w = 200, h = 300;

                        dstQuard[0] = Point2f(0, 0);
                        dstQuard[1] = Point2f(w - 1, 0);
                        dstQuard[2] = Point2f(w - 1 , h - 1);
                        dstQuard[3] = Point2f(0, h - 1);

                        Mat M = getPerspectiveTransform(srcQuard, dstQuard);

                        Mat dst;
                        warpPerspective(src, dst, M, Size(w, h));
                        imshow("DST", dst);

                } 
            }  
        }  
    };
    setMouseCallback("SRC", on_mouse3);
    imshow("SRC", src);
    waitKey();
    destroyAllWindows();


}



void sobel_edge()
{
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    Mat dx;
    Mat dy;
    Sobel(src, dx, CV_32FC1, 1, 0);
    Sobel(src, dy, CV_32FC1, 0, 1);

    imshow("SRC", src);
    imshow("dx", dx);
    imshow("dy", dy);
    Mat mag;
    
    magnitude(dx, dy, mag);
    mag.convertTo(mag, CV_8UC1); // CV_32FC1 -> cv_8UC1
    Mat edge = mag > 150;
    imshow ("MAG", mag);
    imshow("EDGE", edge);
   

    waitKey();
    destroyAllWindows();


}

void canny_edge()
{
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    Mat dst1;
    Mat dst2;

    Canny(src, dst1, 50, 100);
    Canny(src, dst2, 50, 150);
    imshow("SRC", src);
    imshow("DST1", dst1);
    imshow("DST2", dst2);
    waitKey();
    destroyAllWindows();

}


void hough_lines()
{
    Mat src = imread("building.jpg", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    Mat edge;
    Canny(src, edge, 50, 150);

    std::vector<Vec2f> lines;
    HoughLines(edge, lines, 1, CV_PI / 180, 250);
    Mat dst;
    cvtColor(edge, dst, COLOR_GRAY2BGR);
    for (size_t i = 0; i < lines.size(); ++i)
    {
        float r = lines[i][0];
        float t = lines[i][1];
        double cos_t = cos(t); 
        double sin_t = sin(t);
        double x0 = r * cos_t; 
        double y0 = r * sin_t;
        double alpha = 1000;

        Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
        Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));
        line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }
    imshow("SRC", src);
    imshow("dst", dst);
    waitKey();
    destroyAllWindows();


}

void hough_circles()
{
    Mat src = imread("coins3.png", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    std::vector<Vec3f> circles;
    HoughCircles(src, circles, HOUGH_GRADIENT, 1, 20, 300, 38);

    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);

    for (Vec3f c : circles) {
        Point center(cvRound(c[0]), cvRound(c[1]));
        int radius = cvRound(c[2]);
        circle(dst, center, radius, Scalar(0, 0, 255), 2, LINE_AA);
    }
    imshow("SRC", src);
    imshow("DST", dst);
    waitKey();
    destroyAllWindows();
}

void color_grayscale()
{

    Mat src = imread("butterfly.jpg");
    Mat dst;
    cvtColor(src, dst, COLOR_BGR2GRAY);
    imshow("SRC", src);
    imshow("DST", dst);
    waitKey();
    destroyAllWindows();

}

void color_split()
{
    Mat src = imread("candies.png"); //png -> 4channel
    std::vector<Mat> bgr_planes;
    split(src, bgr_planes);
    imshow("SRC", src);
    imshow("B_plane", bgr_planes[0]);
    imshow("G_plane", bgr_planes[1]);
    imshow("R_plane", bgr_planes[2]);
    waitKey();
    destroyAllWindows();
}