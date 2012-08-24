#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "Video/FaceDetector.hpp"


namespace Video
{

FaceDetector::FaceDetector(const std::string& cascadeFile, double scale):
  scale_(scale)
{
  // sanity check
  if( scale_ <= 0.0 || 1.0 < scale_ )
    throw Util::Exception( UTIL_LOCSTRM << "scale factor must be in range (0;1], instead got " << scale_ );
  // load classifier
  if( !cc_.load(cascadeFile) )
    throw Util::Exception( UTIL_LOCSTRM << "unable to load cascade file for face detecion: '" << cascadeFile << "'" );
}


FaceDetector::Faces FaceDetector::detect(const cv::Mat& img)
{
  // convert to gray scale
  cvtColor( img, gray_, CV_BGR2GRAY );
  // scale image down
  const cv::Size smallSize( img.size().width*scale_, img.size().height*scale_ );
  resize( gray_, small_, smallSize, 0, 0, cv::INTER_LINEAR );
  // equalize histogram, for bettter detail detection
  equalizeHist( small_, small_ );

  // perform the detection
  Faces faces;
  auto mode = CV_HAAR_SCALE_IMAGE;
  //mode |= CV_HAAR_FIND_BIGGEST_OBJECT;
  //mode |= CV_HAAR_DO_ROUGH_SEARCH;
  const cv::Size pyrSize(30,30);
  cc_.detectMultiScale( small_, faces, 1.1, 2, mode, pyrSize );

  // return the result, scaled back to the initial image size
  return translatePosition( std::move(faces) );
}


FaceDetector::Faces FaceDetector::translatePosition(Faces faces) const
{
  const double rescale = 1/scale_;
  // process each rectangele, so that it will correspond back to the original image not the scaled-down one
  for(auto& f: faces)
  {
    f.x      *= rescale;
    f.y      *= rescale;
    f.width  *= rescale;
    f.height *= rescale;
  }
  // return output
  return faces;
}

}
