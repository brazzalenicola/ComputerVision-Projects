//
//  main.cpp
//  lab2
//
//  Created by Nicola Brazzale on 02/04/2020.
//  Copyright © 2020 Nicola Brazzale. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
cv::Mat cylindricalProj(const cv::Mat& image,const double angle);

int main(){
    
    vector<cv::String> filename;
    vector<cv::Mat> images;
    cv::String path = "/Users/brazzalenicola/Desktop/Lab5/img/i*.png";
    cv::glob(path, filename);
    
   
    for(size_t i = 0; i < filename.size(); ++i){
        cv::Mat tmp = cv::imread(filename[i]);
        images.push_back(cylindricalProj(tmp, 27.0));
    }
    
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create();
    vector<cv::Mat> descr;
    vector<vector<cv::KeyPoint>> kps;

    //DETECT & COMPUTE KEYPOINTS+DESCRIPTORS
    orb->detect(images, kps, cv::Mat());
    orb->compute(images, kps, descr);
    
    std::vector<cv::DMatch> mtc;
    std::vector<std::vector<cv::DMatch>> matches;
    cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
    std::vector<cv::Point2f> points1, points2;
    std::vector<int> shift;
    shift.resize(descr.size());
    shift[0]=0;
    
    long min_dist = INFINITY;
    float ratio = 4;
    
    for(size_t i = 0; i < descr.size()-1; ++i){
        matcher->match(descr[i], descr[i+1], mtc, cv::Mat());
        
        sort(mtc.begin(), mtc.end());
        min_dist = mtc[0].distance;
        //auto it = std::find(mtc.begin(), mtc.end(), min_dist*ratio);
        
        //initilization of iterator
        auto it = mtc.begin();
        
        //keep only matches with distance less than min_dist * ratio
        for (auto j=mtc.begin(); j<mtc.end();j++){
            if ((*j).distance == min_dist*ratio) {
                it = j;
            }
        }
        //int numGoodMatches = mtc.size() * 0.12f;
        mtc.erase(it+1, mtc.end());
        //matches.push_back(mtc);
    
        
        for (auto m : mtc) {
            points1.push_back( kps[i][m.queryIdx].pt);
            points2.push_back( kps[i+1][m.trainIdx].pt);
        }
        
        cv::Mat mask;
        cv::Mat h = findHomography(points1, points2, cv::RANSAC,5, mask);
        int count = 0;
        shift[i+1]=0;
        for(size_t j=0;j<points1.size();j++){
            if(mask.at<int>(j,1) == 1){
                shift[i+1] += std::abs(points2[j].x - points1[j].x);
                count++;
            }
        }
        cout<<count<<endl;
        
        /*if (i == 0){
            shift[i] = 0;
        } else {
            shift[i] = (shift[i] / count) + shift[i-1];
            cout<<shift[i]<<endl;
        }*/
        shift[i+1] = (shift[i+1] / count) + shift[i];

        
        cv::Mat imMatches;
        cv::drawMatches(images[i], kps[i], images[i+1], kps[i+1], mtc, imMatches);
        cv::namedWindow("Panoramic Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Panoramic Image", imMatches);
        //cv::waitKey(0);
    }
    
    int rows = images[0].rows;

    int cols = images[0].cols * images.size()-shift[images.size()-1];
    cout<<rows<<" "<<cols<<endl;
    
    cv::Mat fin(rows, cols, images[0].type());
    
    
    for (int i = 0; i < images.size(); i++) {
        images[i].copyTo(fin(cv::Rect(shift[i],0, images[0].cols, rows)));
    }
    cv::Mat res;
    cv::Size fsize(cols/2, rows/2);
    cv::resize(fin, res, fsize, 0, 0, cv::INTER_LINEAR);
    cv::imshow("Panoramic", res);
    cv::waitKey(0);
    
    return 0;
}

cv::Mat cylindricalProj(
    const cv::Mat& image,
    const double angle)
{
  cv::Mat tmp,result;
    cv::cvtColor(image, tmp, cv::COLOR_BGR2GRAY);
  result = tmp.clone();


  double alpha(angle / 180 * CV_PI);
  double d((image.cols / 2.0) / tan(alpha));
  double r(d/cos(alpha));
  double d_by_r(d / r);
  int half_height_image(image.rows / 2);
  int half_width_image(image.cols / 2);

  for (int x = - half_width_image + 1,
       x_end = half_width_image; x < x_end; ++x)
  {
    for(int y = - half_height_image + 1,
        y_end = half_height_image; y < y_end; ++y)
    {
      double x1(d * tan(x / r));
      double y1(y * d_by_r / cos(x / r));

      if (x1 < half_width_image &&
          x1 > - half_width_image + 1 &&
          y1 < half_height_image &&
          y1 > -half_height_image + 1)
      {
        result.at<uchar>(y + half_height_image, x + half_width_image)
            = tmp.at<uchar>(round(y1 + half_height_image),
                            round(x1 + half_width_image));
      }
    }
  }

  return result;
}
