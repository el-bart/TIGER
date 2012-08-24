#ifndef INCLUDE_VIDEO_FACEDETECTOR_HPP_FILE
#define INCLUDE_VIDEO_FACEDETECTOR_HPP_FILE

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Util/Exception.hpp"


namespace Video
{

class FaceDetector
{
public:
  typedef std::vector<cv::Rect> Faces;

  FaceDetector(const std::string& cascadeFile, double scale);

  Faces detect(const cv::Mat& img);

private:
  Faces translatePosition(Faces faces) const;

  const double          scale_;
  cv::CascadeClassifier cc_;
  cv::Mat               gray_;
  cv::Mat               small_;
};

}

#endif
