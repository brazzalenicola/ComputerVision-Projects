//
//  canny.h
//  lab4
//
//  Created by Nicola Brazzale on 28/04/2020.
//

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Canny{

// Methods

public:

    // constructor
    // input_img: image to be filtered
    // filter_size : size of the kernel/window of the filter
    Canny(cv::Mat input_img, int param1, int param2);

    // perform filtering (in base class do nothing, to be reimplemented in the derived filters)
    void doCanny();

    // get the output of the filter
    cv::Mat getResult();

// Data

protected:

    // input image
    cv::Mat input_image;

    // output image (filter result)
    cv::Mat result_image;

    // window size
    int param1;
    int param2;
};

class Hough{

// Methods

public:

    // constructor
    // input_img: image to be filtered
    // filter_size : size of the kernel/window of the filter
    Hough(cv::Mat input_img, cv::Mat res ,int param1);

    // perform filtering (in base class do nothing, to be reimplemented in the derived filters)
    void doHough();

    // get the output of the filter
    cv::Mat getResult();

// Data

protected:

    // input image
    cv::Mat input_image;

    // output image (filter result)
    std::vector<cv::Vec2f> lines;
    cv::Mat result_image;

    // window size
    int param1;
};
