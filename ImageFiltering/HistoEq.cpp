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

using namespace std;

void showHistogram(std::vector<cv::Mat>& hists){
  // Min/Max computation
  double hmax[3] = {0,0,0};
  double min;
  cv::minMaxLoc(hists[0], &min, &hmax[0]);
  cv::minMaxLoc(hists[1], &min, &hmax[1]);
  cv::minMaxLoc(hists[2], &min, &hmax[2]);

  std::string wname[3] = { "blue", "green", "red" };
  cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
                           cv::Scalar(0,0,255) };

  std::vector<cv::Mat> canvas(hists.size());

  // Display each histogram in a canvas
  for (int i = 0, end = hists.size(); i < end; i++)
  {
    canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

    for (int j = 0, rows = canvas[i].rows; j < hists[0].rows-1; j++)
    {
      cv::line(
            canvas[i],
            cv::Point(j, rows),
            cv::Point(j, rows - (hists[i].at<float>(j) * rows/hmax[i])),
            hists.size() == 1 ? cv::Scalar(200,200,200) : colors[i],
            1, 8, 0
            );
    }

    cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
  }
}

int main(int argc, const char * argv[]) {
    
    cv::Mat input_img=cv::imread("image.jpg");
    cv::resize(input_img, input_img, cv::Size(input_img.cols/2.0, input_img.rows/2.0));
    
    float range[] = { 0, 256 };
    const float* histRange = { range };
    int histSize = 256;
    vector<cv::Mat> bgr_planes;
    vector<cv::Mat> hists, hists_eq;
    cv::split(input_img, bgr_planes);
    
    
    cv::Mat b_hist, g_hist, r_hist;
    cv::Mat b_hist_eq, g_hist_eq, r_hist_eq;
    
    cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, true, false);
    cv::calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, true, false);
    cv::calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, true, false);
    
    
    hists.push_back(b_hist);
    hists.push_back(g_hist);
    hists.push_back(r_hist);
    
    cv::equalizeHist(bgr_planes[0], bgr_planes[0]);
    cv::equalizeHist(bgr_planes[1], bgr_planes[1]);
    cv::equalizeHist(bgr_planes[2], bgr_planes[2]);
    
    cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist_eq, 1, &histSize, &histRange, true, false);
    cv::calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist_eq, 1, &histSize, &histRange, true, false);
    cv::calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist_eq, 1, &histSize, &histRange, true, false);

    hists_eq.push_back(b_hist_eq);
    hists_eq.push_back(g_hist_eq);
    hists_eq.push_back(r_hist_eq);
    
    cv::Mat input_img_eq;
    
    cv::merge(bgr_planes, input_img_eq);
    cv::imshow("Input Image",input_img);
    showHistogram(hists);
    cv::imshow("Equalized Image",input_img_eq);
    showHistogram(hists_eq);
    
    cv::Mat out_img;
    cv::cvtColor(input_img, out_img, cv::COLOR_BGR2HSV);
    vector<cv::Mat> hsv_planes;
    cv::split(out_img, hsv_planes);
    cv::Mat h_hist, s_hist, v_hist;
    vector<cv::Mat> hists_hsv;
    
    //Equalizing channel V
    cv::equalizeHist(hsv_planes[2], hsv_planes[2]);
    cv::merge(hsv_planes, out_img);
    cv::cvtColor(out_img, out_img, cv::COLOR_HSV2BGR);
    
    cv::calcHist( &hsv_planes[0], 1, 0, cv::Mat(), h_hist, 1, &histSize, &histRange, true, false);
    cv::calcHist( &hsv_planes[1], 1, 0, cv::Mat(), s_hist, 1, &histSize, &histRange, true, false);
    cv::calcHist( &hsv_planes[2], 1, 0, cv::Mat(), v_hist, 1, &histSize, &histRange, true, false);
    
    hists_hsv.push_back(h_hist);
    hists_hsv.push_back(s_hist);
    hists_hsv.push_back(v_hist);
    
    cv::imshow("Converted Image", out_img);
    showHistogram(hists_hsv);
    
    cv::waitKey(0);
    return 0;
}
