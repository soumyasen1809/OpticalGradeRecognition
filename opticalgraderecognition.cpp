#include<iostream>
#include<string>
#include<vector>

#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>

cv::Mat convert_image(cv::Mat input_image){
    cv::Mat image_bin, image_gauss, image_canny;
    cv::cvtColor(input_image, image_bin, cv::COLOR_BGR2GRAY);   // Convert to gray image
    cv::GaussianBlur(image_bin, image_gauss, cv::Size(5,5), 30,0);   // Apply a Gaussian blur
    cv::Canny(image_gauss, image_canny, 10, 10);

    return image_canny;
}


void getContours(cv::Mat input_image, cv::Mat output_image){
    
    // Find and draw the contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(input_image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // cv::drawContours(image_resize, contours, -1, cv::Scalar(210,12,120), 2);    // Note contour id = -1 means all the contours are drawn

    // Find area of the contours
    for (auto i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);

        if (area >= 5) {        // Filter out if small circles (noise) has been detected
            cv::drawContours(output_image, contours, i, cv::Scalar(210,12,120), 2);
        }
        
    }

    // Using HoughCircles
    // std::vector<cv::Vec3f> circles;
    // cv::HoughCircles(image_canny, circles, cv::HOUGH_GRADIENT, 1, 2, 100, 50, 0, 0);
    // for (auto i = 0; i < circles.size(); i++) {
    //     cv::Point center = cv::Point(circles[i][0], circles[i][1]);
    //     cv::circle(image_resize, center, 1, cv::Scalar(0,0,10), 1);

    //     double radius = circles[i][2];
    //     cv::circle(image_resize, center, radius, cv::Scalar(100,20,120), 1);

    // }

}


int main(){
    std::string path = "../resources/omr_sheet1.png";
    cv::Mat image_color = cv::imread(path);   // Read the image (in RGB)
    std::array <cv::Mat,2> image_resize;
    image_resize[0] = image_color(cv::Rect(120,150,250,700));
    image_resize[1] = image_color(cv::Rect(470,150,250,700));

    for (auto i = 0; i < image_resize.size(); i++) {
        cv::Mat image_canny = convert_image(image_resize[i]);
        getContours(image_canny, image_resize[i]);

        cv::imshow("OMR_Sheet", image_resize[i]);
        cv::waitKey(0);
    }
    

    return 0;
}