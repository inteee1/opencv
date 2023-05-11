#include <opencv2/opencv.hpp>
void on_mouse(int, int, int, int, void*);
void camera_in()
{
    cv::VideoCapture cap(-1);
    if(!cap.isOpened()) {
        std::cerr << "camera is not opened..." << std::endl;
        return;
    }
    std::cout << "Height : " << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT)) << std::endl;
    std::cout << "width : " << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH)) << std::endl;
    cv::Mat frame;
    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;
        cv::imshow("camera",~frame);
        if(cv::waitKey(10) == 27) break; // ESC key    
    }
    cap.release();
    cv::destroyAllWindows();
}

void video_in()
{
    cv::VideoCapture cap("stopwatch.avi");
    if(!cap.isOpened()) {
        std::cerr << "camera is not opened..." << std::endl;
        return;
    }
    std::cout << "Height : " << cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT)) << std::endl;
    std::cout << "Width : " << cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH)) << std::endl;
    std::cout << "Count : " << cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT)) << std::endl;
    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = cvRound(1000/fps);
    cv::Mat frame;
    cv::Mat inversed;
    while(true)
    {
        cap >> frame;
        inversed = ~frame;
        if(frame.empty()) break;
        cv::imshow("camera",frame);
        cv::imshow("inversed", inversed);
        if(cv::waitKey(delay) == 27) break; // ESC key    
    }
    //cap.release();
    cv::destroyAllWindows();
}

void video_out()
{
    cv::VideoCapture cap(-1);
    if(!cap.isOpened())
    {
        std::cerr << "camera is not opened..." << std::endl;
        return;
    }

    int h = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int w = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    std::cout << "Height : " << h << std::endl;
    std::cout << "Width : " << w << std::endl;
    std::cout << "Count : " << cvRound(cap.get(cv::CAP_PROP_FRAME_COUNT)) << std::endl;
    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = cvRound(1000/fps);
    cv::Mat frame;
    cv::Mat inversed;
    int fourcc = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
    cv::VideoWriter output("output.avi", fourcc, fps, cv::Size(w, h));
    if(!output.isOpened())
    {   
        std::cerr << "File is not opened" << std::endl;
        return;
    }
       
    while(true)
    {
        cap >> frame;
        if(frame.empty())
        {
            std::cerr << "Frame is not Opened" << std::endl;
            break;
        }
        inversed = ~frame;
        output << frame;

        cv::imshow("Camera",frame);
        cv::imshow("inversvoid on_mouse(int, int, int, int, void*)ed",inversed);
        if(cv::waitKey(delay) == 27) break;
    }
    cv::destroyAllWindows();
}

void drawLines()
{
    //canvas
    cv::Mat img(400, 400, CV_8UC3/* color */, cv::Scalar(255, 255, 255));
    cv::line(img, cv::Point(50,50), cv::Point(200,50), cv::Scalar(0,0,255), 10);
    cv::line(img, cv::Point(50,50), cv::Point(200,200), cv::Scalar(255, 0, 255), 10);
    cv::arrowedLine(img, cv::Point(50,200), cv::Point(150,200), cv::Scalar(0, 255, 0), 10);
    cv::drawMarker(img, cv::Point(30,350), cv::Scalar(0, 0, 255), cv::MARKER_CROSS);

    cv::imshow("CANVAS", img);
    cv::waitKey(0);
    cv::destroyAllWindows();


}

void drawPolys()
{
    cv::Mat img(400, 400, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::rectangle(img, cv::Rect(50, 50, 100, 50), cv::Scalar(299, 255, 207), 2);
    cv::rectangle(img, cv::Rect(50, 150, 100, 50), cv::Scalar(110, 138, 243), -1);
    cv::circle(img, cv::Point(300, 150), 30, cv::Scalar(25, 25, 121), -1, cv::LINE_AA);
    std::vector<cv::Point> pts;
    pts.push_back(cv::Point(250, 250));
    pts.push_back(cv::Point(300, 250));
    pts.push_back(cv::Point(300, 300));
    pts.push_back(cv::Point(350, 300));
    pts.push_back(cv::Point(350, 350));
    pts.push_back(cv::Point(250, 350));

    cv::polylines(img, pts, true, cv::Scalar(9, 67, 212, 2));
    cv::putText(img, "겅부겅부겅부겅부겅부", cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(204, 167, 251));
    cv::imshow("CANVAS", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void keyboard_event()
{
    cv::Mat img = cv::imread("lenna.bmp");
    if(img.empty()) return;
    cv::imshow("lenna", img);
    while(true) {
        int key_value = cv::waitKey(0);
        if(key_value == 'i' or key_value == 'I'){
                img = ~img;
                cv::imshow("Inv_lenna", img);
    } else {
            if(key_value == 27 or key_value == 'q' or key_value == 'Q') {
                    break;
            }

        }
    }
    cv::destroyAllWindows();

}

static cv::Mat img;
static cv::Point pt_old;
void mouse_event()
{
    img = cv::imread("lenna.bmp");
    if(img.empty()) return;
    cv::namedWindow("CANVAS");
    cv::setMouseCallback("CANVAS", on_mouse);
    cv::imshow("CANVAS", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void on_mouse(int event, int x, int y, int flags, void* )
{
    switch(event)
    {
        case cv::EVENT_LBUTTONDOWN:
                pt_old = cv::Point(x,y);
                std::cout << "EVENT_LBUTTON_DOWN" << x << ", " << y << std::endl;
                break;
        case cv::EVENT_LBUTTONUP:
                std::cout << "EVENT_LBUTTON_UP" << x << ", " << y << std::endl;
                break;
        case cv::EVENT_MOUSEMOVE:
                if(flags & cv::EVENT_FLAG_LBUTTON)
                {
                    cv::line(img, pt_old, cv::Point(x, y), cv::Scalar(202, 152, 73), 2);
                    cv::imshow("CANVAS",img);
                    pt_old = cv::Point(x,y);
                }
                break;
    }
}
