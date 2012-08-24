#ifndef INCLUDE_VIDEO_OPTICALFLOW_HPP_FILE
#define INCLUDE_VIDEO_OPTICALFLOW_HPP_FILE

#include <opencv2/opencv.hpp>


namespace Video
{

class OpticalFlow
{
public:
  class Flow
  {
  public:
    explicit Flow(cv::Mat& flow): flow_(flow) { /* TODO: check type */ }

    cv::Size size(void) const { return flow_.size(); }
    cv::Point2f at(const size_t y, const size_t x) const { return flow_.at<cv::Point2f>(y,x); }

  private:
    cv::Mat flow_;
  };


  OpticalFlow(const double scale):
    scale_(scale)
  { }

  Flow computeFlow(const cv::Mat& img);

private:
  const double scale_;
  cv::Mat      prev_;
  cv::Mat      now_;
  cv::Mat      nowGray_;
};

}

#endif
