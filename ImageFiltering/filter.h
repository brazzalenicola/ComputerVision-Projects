#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Generic class implementing a filter with the input and output image data and the parameters
class Filter{

// Methods

public:

	// constructor 
	// input_img: image to be filtered
	// filter_size : size of the kernel/window of the filter
	Filter(cv::Mat input_img, int filter_size);

	// perform filtering (in base class do nothing, to be reimplemented in the derived filters)
	void doFilter();

	// get the output of the filter
	cv::Mat getResult();

	//set the window size (square window of dimensions size x size)
	void setSize(int size);
	
	//get the Window Size
	int getSize();

// Data

protected:

	// input image
	cv::Mat input_image;

	// output image (filter result)
	cv::Mat result_image;

	// window size
	int filter_size;



};

// Gaussian Filter
class GaussianFilter : public Filter  {

// write here your code
public:
    // constructor
    // input_img: image to be filtered
    // kernel_size : size of the kernel/window of the filter
    // sigma: sigmaX
    GaussianFilter(cv::Mat input_img, int kernel_size, int sigma);

    // place constructor
    void doFilter();
    // re-implement  doFilter()
    cv::Mat getResult();
    // additional parameter: standard deviation (sigma)
protected:
    cv::Mat input_image;
    cv::Mat filtered_img;
    int k_size, sig;
};

class MedianFilter : public Filter {

// write here your code
public:
    // place constructor
    MedianFilter(cv::Mat input_img, int kernel_size);
    
    // re-implement  doFilter()
    void doFilter();
    
    cv::Mat getResult();
    // no additional parameters
protected:
    cv::Mat input_image;
    cv::Mat filtered_img;
    int k_size;

};

class BilateralFilter : public Filter {

// write here your code
public:
    BilateralFilter(cv::Mat input_img, int sigma_color, int sigma_space);
    // place constructor
    
    // re-implement  doFilter()
    void doFilter();
    
    // additional parameters: sigma_range, sigma_space
    cv::Mat getResult();

protected:
    cv::Mat input_image;
    cv::Mat filtered_img;
    int sigma_c, sigma_s;

};
