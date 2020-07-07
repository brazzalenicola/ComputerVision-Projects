#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ccalib.hpp>
#include <opencv2/stitching.hpp>


class PanoramicImage{
public:
    PanoramicImage(); //constructor -> load images
    //cv:: Mat PanoramicUtils::cylindricalProj(const cv::Mat& image, const double angle);
    void feat_extract();
    void matches();
protected:
    cv::Mat input_image;
    
};
