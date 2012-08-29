#include <utility>

#include "Video/FaceTracker.hpp"


namespace Video
{

FaceTracker::FaceTracker(FaceDetectorPtr det, FaceRecognizerWrapperPtr rec, Parameters params):
  det_( std::move(det) ),
  rec_( std::move(rec) ),
  frameNo_(0),
  params_(params)
{
  // check detector and recognizer
  if( det_.get()==nullptr || rec_.get()==nullptr )
    throw Util::Exception( UTIL_LOCSTRM << "face detector nor face recognizer cannot be null" );
  // check params
  {
    const double tmp = params_.betweenFramesMovePercent_;
    if( 0 <= tmp && tmp <= 1 )
      throw Util::Exception( UTIL_LOCSTRM << "percent of movement between frames must be in [0..1], but got " << tmp );
  }
}


FaceTracker::Faces FaceTracker::process(const cv::Mat& frame)
{
  ++frameNo_;
  Faces local = processSingleFrame(frame);
  return postProcess( std::move(local) );
}


FaceTracker::Faces FaceTracker::processSingleFrame(const cv::Mat& frame) const
{
  FaceDetector::Faces detected = det_->detect(frame);
  Faces               faces;
  for(const auto& faceRect: detected)
  {
    // TODO
  }
  // TODO
  return faces;
}


FaceTracker::Faces FaceTracker::postProcess(Faces faces)
{
  // TODO
  return faces;
}


void FaceTracker::updateRecognizer(const FaceSequence& seq)
{
  // TODO
}

}
