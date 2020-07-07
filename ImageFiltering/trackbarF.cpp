//
//  es2.cpp
//  lab3
//
//  Created by Nicola Brazzale on 10/04/2020.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter.h"

struct ImageWithParams {
int param1;
int param2;
cv::Mat src_img, filtered_img;
};


using namespace std;
//first parameter is the trackbar position and the second parameter is the user data
void Foo(int,void* userdata){
    
    ImageWithParams imageP = *(ImageWithParams*) userdata;
    BilateralFilter bil(imageP.src_img, imageP.param1, imageP.param2);
    bil.doFilter();
    imageP.filtered_img = bil.getResult();
    
    cv::imshow( "Bilateral filtered image", imageP.filtered_img);
}

void Foo1(int,void* userdata){
    
    ImageWithParams imageP = *(ImageWithParams*) userdata;
    GaussianFilter gaf(imageP.src_img, imageP.param1, imageP.param2);
    gaf.doFilter();
    imageP.filtered_img = gaf.getResult();
    
    cv::imshow( "Gaussian filtered image", imageP.filtered_img);
}

void Foo2(int,void* userdata){
    
    ImageWithParams imageP = *(ImageWithParams*) userdata;
    MedianFilter mef(imageP.src_img, imageP.param1);
    mef.doFilter();
    imageP.filtered_img = mef.getResult();
    
    cv::imshow( "Median filtered image", imageP.filtered_img);
}

int main(int argc, const char * argv[]){
    
    cv::Mat original = cv::imread("image.jpg");
    cv::Mat bil = original.clone();
    cv::Mat gauss = original.clone();
    cv::Mat med = original.clone();
    
    ImageWithParams bilateral;
    ImageWithParams gaussian;
    ImageWithParams median;
    
    cv::namedWindow("Original image");
    cv::namedWindow("Bilateral filtered image",1);
    cv::namedWindow("Gaussian filtered image");
    cv::namedWindow("Median filtered image");
    
    int ker_size;
    
    //bilateral.param1 & bilateral.param2 contain the intial values of sigmaColor and sigmaSpace, are the value to be changed with the trackbar
    bilateral.param2 = 0;
    bilateral.param1 = 0;
    bilateral.filtered_img = bil;
    bilateral.src_img = original;

    cv::createTrackbar( "sigmaColor", "Bilateral filtered image", &bilateral.param1, 90, Foo, (void*) &bilateral);
    Foo(bilateral.param1, (void*) &bilateral);
    
    cv::createTrackbar( "spaceColor", "Bilateral filtered image", &bilateral.param2, 100, Foo, (void*) &bilateral);
    Foo(bilateral.param2, (void*) &bilateral);
    
    
    //gaussian.param1 & gaussian.param2 contain the intial values of sigmax and kernel size, are the value to be changed with the trackbar
    gaussian.param1 = 0;
    gaussian.param2 = 0;
    gaussian.filtered_img = gauss;
    gaussian.src_img = original;
    
    cv::createTrackbar("SigmaX", "Gaussian filtered image", &gaussian.param1, 100, Foo1, &gaussian);
    Foo1(gaussian.param1, (void*) &gaussian);
    
    cv::createTrackbar("KernelSize", "Gaussian filtered image", &gaussian.param2, 90, Foo1, &gaussian);
    Foo1(gaussian.param2, (void*) &gaussian);
    
    
    //median.param1 contains the intial value of kernel size, is the value to be changed with the trackbar
    median.param1 = 0;
    median.filtered_img = med;
    median.src_img = original;
    
    cv::createTrackbar("KernelSize", "Median filtered image", &median.param1, 31, Foo2, &median);
    Foo2(median.param1, (void *) &median);
    
    cv::imshow("Original image", original);
    cv::waitKey();
}

