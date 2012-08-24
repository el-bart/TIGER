#include <utility>

#include "Video/OpticalFlow.hpp"


namespace Video
{

OpticalFlow::Flow OpticalFlow::computeFlow(const cv::Mat& img)
{
  // output flow map
  cv::Mat flowMat;
  // convert to gray scale
  cvtColor(img, nowGray_, CV_BGR2GRAY);
  // scale image down
  const cv::Size smallSize( img.size().width*scale_, img.size().height*scale_ );
  resize( nowGray_, now_, smallSize, 0, 0, cv::INTER_LINEAR );
  // compute flow only if there is a previous image
  if( prev_.data )
    calcOpticalFlowFarneback(prev_, now_, flowMat, 0.5, 3, 15, 3, 5, 1.2, 0);
  // make current image the previous one
  std::swap(prev_, now_);
  // return the output type
  return Flow(flowMat);
}

}
