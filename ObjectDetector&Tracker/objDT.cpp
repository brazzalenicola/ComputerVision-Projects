//
//  main.cpp
//  lab6
//
//  Created by Nicola Brazzale on 20/05/2020.
//  Copyright © 2020 Nicola Brazzale. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;

int main(){
    
    vector<cv::String> filename;
    vector<cv::Mat> images;
    cv::String path = "/Users/brazzalenicola/Desktop/Lab6/img/objects/obj*.png";
    cv::glob(path, filename);
    
    //examples images
    for(size_t i=0; i<filename.size();i++){
        images.push_back(cv::imread(filename[i]));
    }
    
    for(size_t i=0; i<images.size();i++){
        cv::resize(images[i], images[i], cv::Size(images[i].cols/2.3, images[i].rows/2.3));
    }
    
    cv::VideoCapture cap("/Users/brazzalenicola/Desktop/Lab6/img/video.mov");
    cv::Mat first_frame;
    cap.read(first_frame); //store the first frame
    
    auto sift = cv::xfeatures2d::SIFT::create(10000);
    //descr and keypoints for example images
    vector<cv::Mat> descr;
    vector<vector<cv::KeyPoint>> kps;
    //descriptor and keypoints for the first frame
    cv::Mat dcr_first;
    vector<cv::KeyPoint> kp_first;
    vector<cv::Mat> img_matches;
    
    
    sift->detect(images, kps);
    sift->compute(images, kps, descr);
    
    sift->detect(first_frame, kp_first);
    sift->compute(first_frame, kp_first, dcr_first);
    
    std::vector<cv::DMatch> mtc;
    std::vector<std::vector<cv::DMatch>> matches;
    cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2);
    
    //1.8
    float ratio = 3;
    
    //for colors
    vector<cv::Scalar> s;
    s.push_back(cv::Scalar(255,0,0));
    s.push_back(cv::Scalar(0,255,0));
    s.push_back(cv::Scalar(0,0,255));
    s.push_back(cv::Scalar(255,70,255));
    
    vector<vector<cv::Point2f>> points_temp, points_scene;
    points_temp.resize(images.size());
    points_scene.resize(images.size());
    
    vector<vector<cv::Point2f>> scene_corners(4);
    vector<vector<cv::Point2f>> obj_corners(4);
    
    for(size_t i=0; i<images.size();i++){
        matcher->match(descr[i], dcr_first, mtc);
        
        //find min distance of the matches
        sort(mtc.begin(), mtc.end());
        long min_dist = mtc[0].distance;
        auto it = mtc.begin();

        //keep only matches with distance less than min_dist * ratio
        for (auto j=mtc.begin(); j<mtc.end();j++){
            if ((*j).distance <= min_dist*ratio) {
                it = j;
            }
        }
        mtc.erase(it+1, mtc.end());
        
        for (auto m : mtc) {
            points_temp[i].emplace_back( kps[i][m.queryIdx].pt);
            points_scene[i].emplace_back( kp_first[m.trainIdx].pt);
        }
        
        cv::Mat imMatches;
        cv::drawMatches(images[i], kps[i], first_frame, kp_first, mtc, imMatches, s[i], s[i]);
        img_matches.push_back(imMatches);
        
        cv::Mat mask;
        cv::Mat h = findHomography(points_temp[i], points_scene[i], cv::RANSAC,3, mask);
        
        obj_corners[i].push_back(cv::Point(0,0));
        obj_corners[i].push_back(cv::Point( images[i].cols, 0));
        obj_corners[i].push_back(cv::Point( images[i].cols, images[i].rows));
        obj_corners[i].push_back(cv::Point( 0, images[i].rows));
        
        
        perspectiveTransform( obj_corners[i], scene_corners[i], h);
        
        line( imMatches, scene_corners[i][0] + cv::Point2f( images[i].cols, 0), scene_corners[i][1] + cv::Point2f( images[i].cols, 0), cv::Scalar(0, 255, 0), 4 );
        line( imMatches, scene_corners[i][1] + cv::Point2f( images[i].cols, 0), scene_corners[i][2] + cv::Point2f( images[i].cols, 0), cv::Scalar( 0, 255, 0), 4 );
        line( imMatches, scene_corners[i][2] + cv::Point2f( images[i].cols, 0), scene_corners[i][3] + cv::Point2f( images[i].cols, 0), cv::Scalar( 0, 255, 0), 4 );
        line( imMatches, scene_corners[i][3] + cv::Point2f( images[i].cols, 0), scene_corners[i][0] + cv::Point2f( images[i].cols, 0), cv::Scalar( 0, 255, 0), 4 );

        imshow( "Matches&Rects", imMatches);
        //cv::waitKey(0);
    }
    

    if(cap.isOpened()){
        cv::Mat frame;
        cap>>frame;
        cv::Mat next;
        cap>>next;
        cv::Mat mask = cv::Mat::zeros(frame.size(), frame.type());

        
        while(!next.empty()) {
            // calculate optical flow
            vector<cv::Point2f> next_pts;
            vector<uchar> status;
            vector<float> err;
            cv::calcOpticalFlowPyrLK(frame, next, points_scene[0], next_pts, status, err, cv::Size(11,11));
            
            vector<cv::Point2f> newFeatures;
            newFeatures.reserve(points_scene[0].size());
            auto it = points_scene[0].begin();
            
            for(uint j = 0; j < next_pts.size(); j++){
                // Select good points
                if(status[j] == 1) {
                    newFeatures.push_back(next_pts[j]);
                    ++it;
                } else {
                    it = points_scene[0].erase(it);
                }
            }
            
            //find the new position of the vertices
            cv::Mat mask;
            cv::Mat h = findHomography(points_scene[0], newFeatures, mask, cv::RANSAC);
            
            points_scene[0] = newFeatures;
            vector<cv::Point2f> new_corners(4);
            cv::perspectiveTransform( scene_corners[0], new_corners, h);
            scene_corners[0] = new_corners;
            
            cv:: Mat outframe = next.clone();
            for (int j = 0; j < scene_corners[0].size(); ++j)
            {
                cv::drawMarker(outframe,scene_corners[0][j],s[j],cv::MARKER_CROSS,20,10);
            }
            
            std::vector<cv::Point> vertices;
            vertices.reserve(4);
            for (const auto vertex : scene_corners[0])
            {
                vertices.emplace_back(vertex);
            }
            cv::polylines(outframe, vertices, true, s[0], 10);

            cv::imshow("Video", outframe);
            cv::waitKey(5);
            
            frame = next.clone();
            cap>>next;
        }
        cv::waitKey(0);
    }
    
    return 0;
}
