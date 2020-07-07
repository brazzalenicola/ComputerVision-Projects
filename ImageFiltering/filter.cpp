#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filter.h"

//using namespace cv;

	// constructor
	Filter::Filter(cv::Mat input_img, int size) {

		input_image = input_img;
		if (size % 2 == 0)
			size++;
		filter_size = size;
	}

	// for base class do nothing (in derived classes it performs the corresponding filter)
	void Filter::doFilter() {

		// it just returns a copy of the input image
		result_image = input_image.clone();

	}

	// get output of the filter
	cv::Mat Filter::getResult() {

		return result_image;
	}

	//set window size (it needs to be odd)
	void Filter::setSize(int size) {

		if (size % 2 == 0)
			size++;
		filter_size = size;
	}

	//get window size 
	int Filter::getSize() {

		return filter_size;
	}

GaussianFilter::GaussianFilter(cv::Mat input_img, int kernel_size, int sigma) : Filter(input_img, kernel_size) {
    input_image = input_img;
    if (kernel_size % 2 == 0)
        kernel_size++;
    k_size = kernel_size;
    sig = sigma;
}
void GaussianFilter::doFilter(){
    cv::GaussianBlur( input_image, filtered_img, cv::Size( k_size, k_size ), sig, 0 );
}

cv::Mat GaussianFilter::getResult() {

    return filtered_img;
}
MedianFilter::MedianFilter(cv::Mat input_img, int kernel_size) : Filter(input_img, kernel_size){
    input_image = input_img;
    if (kernel_size % 2 == 0)
        kernel_size++;
    k_size = kernel_size;
}

void MedianFilter::doFilter(){
    cv::medianBlur(input_image, filtered_img, k_size);
}
cv::Mat MedianFilter::getResult() {

    return filtered_img;
}
BilateralFilter::BilateralFilter(cv::Mat input_img, int sigma_color, int sigma_space) : Filter(input_img, 6*sigma_space){
    input_image = input_img;
    sigma_c = sigma_color;
    sigma_s = sigma_space;
}
void BilateralFilter::doFilter(){
    cv::bilateralFilter ( input_image, filtered_img, 6*sigma_s, sigma_c, sigma_s);
}

cv::Mat BilateralFilter::getResult() {

    return filtered_img;
}
