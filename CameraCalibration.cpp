//
//  main.cpp
//  lab2
//
//  Created by Nicola Brazzale on 02/04/2020.
//  Copyright © 2020 Nicola Brazzale. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, const char * argv[]) {
    
    cv::Mat input_img[58];
    char c[90];
    cv::Size patternsize(6,5);
    int CHESSBOARD[2]={6,5};
    vector<cv::Point2f> inner_cor;
    
    vector<cv::Point3f> corners;
    vector<vector<cv::Point3f>> points3d;
    vector<vector<cv::Point2f>> points2d;
    vector<cv::Point2f> proj_2d;
    bool success;
    
    for(int i=0; i<CHESSBOARD[1]; i++){
        for(int j=0; j<CHESSBOARD[0]; j++){
          corners.push_back(cv::Point3f(j,i,0));
        }
    }

     for(int i=1;i<=58;i++){
         if (i==37) {
             continue;
         }
         if(i<10){
             sprintf(c,"/Users/brazzalenicola/Desktop/Computer Vision/lab2/data/checkerboard_images/000%d_color.png",i);
         } else {
             sprintf(c,"/Users/brazzalenicola/Desktop/Computer Vision/lab2/data/checkerboard_images/00%d_color.png",i);
         }
         input_img[i-1] = cv::imread(c);
         success = findChessboardCorners(input_img[i-1], patternsize, inner_cor,cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE
         + cv::CALIB_CB_FAST_CHECK);
         if (success) {
             //cornerSubPix(input_img[i], inner_cor, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1));
             
             points2d.push_back(inner_cor);
             
             points3d.push_back(corners);
         }
     }

    cv::Mat cameraMatrix,distCoeffs,R,T;
    cv::calibrateCamera(points3d, points2d, cv::Size(input_img[1].rows,input_img[1].cols), cameraMatrix, distCoeffs, R, T);
    
    cout << "cameraMatrix : " << cameraMatrix << endl;
    cout << "distCoeffs : " << distCoeffs << endl;
    cout << "Rotation vector : " << R << endl;
    cout << "Translation vector : " << T << endl;
    
    double error = 0, tot_error=0, max = 0, min=100;
    int itM = 0, itm =0;
    //vector<cv::Point2f> proj_2d;
    
    for (int i=0; i<points3d.size(); i++) {
        cv::projectPoints(points3d[i], R.row(i), T.row(i), cameraMatrix, distCoeffs,proj_2d);
        error = cv::norm(points2d[i],proj_2d, cv::NORM_L2)/(proj_2d.size());
        if (error>=max) {
            max=error;
            itM = i;
        }
        if (error<=min) {
            min = error;
            itm = i;
        }
        tot_error = tot_error + error;
    }
    cout<<"Mean error "<<(tot_error/points3d.size())<<endl;
    cout<<"Maximum error "<<max<<" at iteration "<<itM<<endl;
    cout<<"minimum error "<<min<<" at iteration "<<itm<<endl;
    
    cv::Mat new_img = cv::imread("/Users/brazzalenicola/Desktop/Computer Vision/lab2/data/test_image.png");
    cv::Mat new_cameraMatrix, dst;
    
    cv::Mat map1, map2, R_matrix;
    cv::imshow("img1", new_img);
    Rodrigues(R.row(itm),R_matrix);
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs,R_matrix,new_cameraMatrix, cv::Size(new_img.rows,new_img.cols), CV_16SC2 ,map1, map2);
    cv::remap(new_img, dst, map1, map2, cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    
    cv::imshow("img1", new_img);
    cv::imshow("img2", dst);
    cv::waitKey();
    
    return 0;
}

