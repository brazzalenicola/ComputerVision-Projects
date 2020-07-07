//
//  canny.cpp
//  lab4
//
//  Created by Nicola Brazzale on 28/04/2020.
//

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "canny.h"

Canny::Canny(cv::Mat input_img, int par1, int par2) {

    input_image = input_img;
    param1 = par1;
    param2 = par2;
    result_image = input_image.clone();
}

void Canny::doCanny() {

    cv::Canny(input_image, result_image, param1, param2);

}

// get output of the filter
cv::Mat Canny::getResult() {

    return result_image;
}
Hough::Hough(cv::Mat input_img, cv::Mat result, int par1){

    input_image = input_img;
    param1 = par1;
    result_image = result;
}

void Hough::doHough() {
    
    lines = std::vector<cv::Vec2f>();
    //HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
    cv::HoughLines(input_image, lines, 1, CV_PI/180, param1, 0 ,0);
    //lines.size()
    for( size_t i = 0; i < lines.size(); i++){
       float rho = lines[i][0], theta = lines[i][1];
       cv::Point pt1, pt2;
       double a = cos(theta), b = sin(theta);
       double x0 = a*rho, y0 = b*rho;
       pt1.x = cvRound(x0 + 1000*(-b));
       pt1.y = cvRound(y0 + 1000*(a));
       pt2.x = cvRound(x0 - 1000*(-b));
       pt2.y = cvRound(y0 - 1000*(a));
       line(result_image, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }

}

// get output of the filter
cv::Mat Hough::getResult() {

    return result_image;
}
