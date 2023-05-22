#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void on_threshold(int pos, void* userdata);

void using_threshold()
{
    Mat src = imread("sudoku.jpg", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    namedWindow("SRC");
    namedWindow("DST");
    imshow("SRC", src);
    auto on_thread = [](int pos, void* userdata)
    {
        Mat src = *static_cast<Mat*>(userdata);
        Mat dst;
        threshold(src, dst, pos, 255, THRESH_BINARY);
        imshow("DST", dst);

    };
   
    createTrackbar("Threshold", "DST", 0, 255, on_thread, (void*)&src);
    setTrackbarPos("Threshold", "DST", 128);
    waitKey();
    destroyAllWindows();
}

static void on_trackbar(int pos, void* userdata)
{
    Mat src = *(static_cast<Mat*>(userdata));
    int block_size = pos;
    if(pos % 2 == 0) --block_size;
    if(block_size < 3) block_size = 3;
    Mat dst;
    adaptiveThreshold(src, dst, 255.0 , ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, block_size, 5.0/* 이것을 많이 사용한다.*/);
    imshow("DST", dst);
}
void adaptive_threshold()
{
    Mat src = imread("sudoku.jpg", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    imshow("SRC", src);
    namedWindow("DST");
    createTrackbar("BlockSize", "DST", 0, 200, on_trackbar, (void*)&src);
    setTrackbarPos("BlockSize", "DST", 11);
    waitKey();
    destroyAllWindows();
}


void erode_dilate()
{
    Mat src = imread("milkdrop.bmp", IMREAD_GRAYSCALE);
    if(src.empty()) return;

    Mat bin;
    threshold(src, bin, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);

    Mat dst1, dst2;
    erode(bin, dst1, Mat());
    dilate(bin, dst2, Mat());

    imshow("SRC", src);
    imshow("BIN", bin);
    imshow("ERODE", dst1);
    imshow("DILATE", dst2);
    waitKey();
    destroyAllWindows();

}

void labeling_baseic()
{
    uchar data[] = {
        0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 1, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 1, 1, 0,
        0, 0, 0, 1, 0, 0, 1, 0,
        0, 0, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    Mat src = Mat(8, 8, CV_8UC1, data) * 255;
    Mat labels;
    int cnt = connectedComponents(src, labels);
    cout << "src: \n" << src << endl;
    cout << "labels: \n" << labels << endl;
    cout << "number of labels: " << cnt << endl;
}

void labeling_stats()
{
    Mat src = imread("keyboard.bmp", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    Mat binary_image;
    threshold(src, binary_image, 0.0, 255.0, THRESH_BINARY | THRESH_OTSU);
    Mat labels;
    Mat stats;
    Mat centroids;
    int numberOfLabels = connectedComponentsWithStats(binary_image, labels, stats, centroids);
    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);
    for (int i = 1; i < numberOfLabels; ++i)
    {
        int *ptr = stats.ptr<int>(i);
        if (ptr[4] < 20) continue;
        rectangle(dst, Rect(ptr[0], ptr[1], ptr[2], ptr[3]), Scalar(0, 255, 255), 2);
    }
    imshow("SRC", src);
    imshow("DST", dst);
    waitKey();
    destroyAllWindows();


}

void contours_hier()
{
    Mat src = imread("contours.bmp", IMREAD_GRAYSCALE);
    if(src.empty()) return;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);
    for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
    {
        Scalar c(rand() & 255, rand() & 255, rand() & 255);
        drawContours(dst, contours, idx, c, -1, LINE_8, hierarchy);
    }

    imshow("SRC", src);
    imshow("DST", dst);
    waitKey();
    destroyAllWindows();
}
static void setLabel(Mat& img, const vector<Point>&pts, const String& label)
{
    Rect rc = boundingRect(pts);
    rectangle(img, rc, Scalar(0, 0, 255), 1);
    putText(img, label, rc.tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));

}
void find_polyGon() 
{
    Mat src = imread("polygon.bmp", IMREAD_COLOR);
    if(src.empty()) return;
    Mat gray_scale;
    cvtColor(src, gray_scale, COLOR_BGR2GRAY);
    Mat binary_image;
    threshold(gray_scale, binary_image, 200/*threshold*/, 255/*maxvalue*/, THRESH_BINARY_INV | THRESH_OTSU);
    vector<vector<Point>> contours; // 외곽선들을 담기 위한 벡터 자료형
    findContours(binary_image, contours, RETR_EXTERNAL /*mode*/ , CHAIN_APPROX_NONE);
    for(auto&& pts: contours){
        if(contourArea(pts) < 400) { // pixel 400보다 작으면 제외 왜냐하면 노이즈를 객체로 인식할 수 있음.
            continue;
        }
        vector<Point> approximate;
        approxPolyDP(pts, approximate, arcLength(pts/*curve*/, true/*closed*/) * 0.02/* 보통 0.02%로 설정*/, true);
        int polygon_size = static_cast<int>(approximate.size());
        if(polygon_size == 3) {
            setLabel(src, pts, "Triangle");
        } else if( polygon_size == 4) {
            setLabel(src, pts, "Rectangle");
        } else if(polygon_size > 4) {
            double length = arcLength(pts, true);
            double area = contourArea(pts);
            double ration = 4.0 * CV_PI * area / (length * length);
            if (ration > 0.8) {
                setLabel(src, pts, "Circle");
            }
        }
    }
    imshow("Image", src);
    waitKey();
    destroyAllWindows();
}

void template_matching()
{
    Mat img = imread( "circuit.bmp", IMREAD_COLOR);
    Mat tempi = imread("crystal.bmp", IMREAD_COLOR);
    if(img.empty()) return;
    img = img + Scalar(50, 50, 50);
    Mat noise(img.size(), CV_32SC3);
    randn(noise, 0, 10);
    add(img, noise, img, Mat(), CV_8UC3);

    Mat res, res_norm;
    matchTemplate(img, tempi, res, TM_CCOEFF_NORMED);
    normalize(res, res_norm, 0, 255, NORM_MINMAX, CV_8U);

    double maxv {0.0};
    Point maxloc;
    minMaxLoc(res, 0, &maxv, 0, &maxloc);
    cout << "maxv: " << maxv << endl;

    rectangle(img, Rect(maxloc.x, maxloc.y, tempi.cols, tempi.rows), Scalar(0, 0, 255), 2);
    imshow("Tempi", tempi);
    imshow("res_norm",res_norm);
    imshow("img", img);
    waitKey();
    destroyAllWindows();



}

void detect_face()
{
    Mat src = imread("kids.png");
    if(src.empty()) return;
    CascadeClassifier classifier("haarcascade_frontalface_default.xml");
    if(classifier.empty()) return;
    vector<Rect> faces;
    classifier.detectMultiScale(src, faces);
    for(auto&& face: faces)
    {
        rectangle(src, face, Scalar(255, 0, 255), 2);
    }
    imshow("SRC", src);
    waitKey();
    destroyAllWindows();
}

void detect_walkers()
{
    VideoCapture capture("vtest.avi");
    if(!capture.isOpened()) {
        cerr << "VIDEO open failed" << endl;
        return;
    }
    HOGDescriptor hog;
    hog.setSVMDetector(hog.getDefaultPeopleDetector());
    Mat frame;
    while(true) {
        capture >> frame;
        if(frame.empty()) break;
        vector<Rect> detected;
        hog.detectMultiScale(frame, detected);
        for(auto&& rc : detected) {
            Scalar color = Scalar(rand() % 256, rand() % 256, rand() % 256);
            rectangle(frame, rc, color, 3);
        }
        imshow("Frame", frame);
        if(waitKey(10) == 27) break;
    }


}

