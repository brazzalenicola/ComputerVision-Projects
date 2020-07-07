//
//  main.cpp
//  lab2
//
//  Created by Nicola Brazzale on 02/04/2020.
//  Copyright © 2020 Nicola Brazzale. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "canny.h"

#define CV_PI   3.1415926535897932384626433832795
using namespace std;

bool intersection(cv::Point2f o1, cv::Point2f p1, cv::Point2f o2, cv::Point2f p2,
                      cv::Point2f &r)
{
    cv::Point2f x = o2 - o1;
    cv::Point2f d1 = p1 - o1;
    cv::Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    r = o1 + d1 * t1;
    return true;
}
struct ImageWithParams {
    int par1;
    int par2;
    cv::Mat src_img, dest_img;
};

void Foo(int, void* userdata){
    
    ImageWithParams image = *(ImageWithParams*) userdata;
    Canny can(image.src_img, image.par1, image.par2);
    can.doCanny();
    image.dest_img = can.getResult();
    cv::imshow( "Canny image", image.dest_img);
    cout<<image.par2<<" "<<image.par1<<endl;
}

void Foo1(int, void* userdata){
    
    ImageWithParams image = *(ImageWithParams*) userdata;
    Hough hou(image.src_img, image.dest_img, image.par1);
    hou.doHough();
    image.dest_img = hou.getResult();
    cv::imshow( "Hough image", image.dest_img);
    cout<<image.par1<<endl;
}
void Foo2(int, void* userdata){
    ImageWithParams image = *(ImageWithParams*) userdata;
    vector<cv::Vec3f> circles;
    cv::HoughCircles(image.src_img, circles, cv::HOUGH_GRADIENT, 2, image.src_img.rows/16, image.par1, image.par2);
    for( size_t i = 0; i < circles.size(); i++ )
    {
         cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( image.dest_img, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
         // draw the circle outline
         circle( image.dest_img, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    }

    cv::imshow( "HoughCircle image", image.dest_img);
}

int main(int argc, const char * argv[]) {
    
    cv::Mat input_img= cv::imread("input.png");
    cv::Mat dst_c = input_img.clone();
    cv::Mat dst = input_img.clone();
    cv::Mat pro, gray;
    cv::namedWindow("Canny image");
    
    ImageWithParams canny, hough;
    cv::cvtColor(input_img, gray, cv::COLOR_BGR2GRAY);
    
    canny.src_img = gray;
    canny.dest_img = dst_c;
    canny.par1 = 0;
    canny.par2 = 0;
    
    // TRACKBAR USED TO SET THE CORRECT PARAMETERS
    /*cv::createTrackbar("Param1", "Canny image", &canny.par1, 1500, Foo,(void*) &canny);
    Foo(canny.par1, (void*) &canny);
    cv::createTrackbar("Param2", "Canny image", &canny.par2, 1500, Foo,(void*) &canny);
    Foo(canny.par2, (void*) &canny);*/
    
    cv::Mat final_canny = gray.clone();
    cv::Mat in_canny = gray.clone();
    
    cv::Canny(in_canny, final_canny, 425, 850,3);
    
    // TRACKBAR USED TO SET THE CORRECT PARAMETERS
    /*hough.src_img = final_canny;
    hough.dest_img = dst;
    hough.par1 = 150;
    
    cv::createTrackbar("Param1", "Hough image", &hough.par1, 600, Foo1,(void*) &hough);
    Foo1(hough.par1, (void*) &hough);*/

    
    vector<cv::Vec2f> lineS;
    cv::Mat fin = input_img.clone();
    vector<cv::Point2f> p1, p2;

    cv::HoughLines(final_canny, lineS, 1, CV_PI/60, 130);
    for( size_t i = 0; i< lineS.size(); i++){
        cv::Point2f pt1, pt2;
        float rho = lineS[i][0], theta = lineS[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        p1.push_back(pt1);
        p2.push_back(pt2);
       //line( fin, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
    }
    
    cv::Point2f inter, ix, iy;
    
    intersection(p1[0], p2[0], p1[1], p2[1], inter);
    intersection(p1[0], p2[0], cv::Point(fin.cols, fin.rows), cv::Point(0, fin.rows), ix);
    intersection(p1[1], p2[1],cv::Point(fin.cols, fin.rows), cv::Point(0, fin.rows), iy);
    
    cv::Point pts[3];
    pts[0] = inter;
    pts[1] = ix;
    pts[2] = iy;
    
    int npts[] = {3};
    const cv::Point* ppt[1] = { pts };
    
    cv::fillPoly(fin, ppt , npts, 1, cv::Scalar(0,0,255), 8);
    

    //CIRCLES DETECTION
    vector<cv::Vec3f> circles;
    cv::Canny(input_img.clone(), final_canny , 1309 , 649);
    cv::HoughCircles(final_canny, circles, cv::HOUGH_GRADIENT, 1, gray.rows, 200, 10);
    for( size_t i = 0; i < circles.size(); i++ )
    {
         cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         circle(fin, center, radius, cv::Scalar(0,255,0), -1, 8, 0 );
    }
    
    cv::imshow("Final Image", fin);

    
    cv::waitKey(0);
    return 0;
}

