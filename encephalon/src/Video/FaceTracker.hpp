#ifndef INCLUDE_VIDEO_FACETRACKER_HPP_FILE
#define INCLUDE_VIDEO_FACETRACKER_HPP_FILE

#include <deque>
#include <string>
#include <vector>
#include <utility>
#include <cinttypes>
#include <opencv2/opencv.hpp>

#include "Util/Exception.hpp"
#include "Video/FaceDetector.hpp"
#include "Video/FaceRecognizerWrapper.hpp"


namespace Video
{

class FaceTracker
{
public:
  struct FaceInfo
  {
    cv::Point   center_;
    std::string label_;
    cv::Mat     face_;
  };
  // collection of recognized faces
  typedef std::vector<FaceInfo> Faces;

  struct FaceFrameInfo
  {
    FaceInfo info_;
    uint64_t seqNo_;
  };
  // sequence of face recognitions on the movie
  typedef std::deque<FaceFrameInfo> FaceSequence;
  // face images sequence and its description
  struct FaceSequenceInfo
  {
    enum class SequenceClass
    {
      NOT_YET_DETERMINED,
      RECOGNIZED,
      UNKNOWN
    };

    FaceSequence  faceSeq_;
    SequenceClass seqClass_;
    std::string   label_;       // meaningful only if seqClass_==RECOGNIZED
  };
  // historical data on face tracking
  typedef std::deque<FaceSequenceInfo> FaceHistory;

  // configuration paramters
  struct Parameters
  {
    double betweenFramesMovePercent_;       // [0..1] - how much can face move between images to be still classified as the same?
    size_t fullUnknownSeqLen_;              // sequence length long enought to be proposed for learning
    size_t maxUnknownSequenceLength_;       // sequence length to be used for learning
  };

  FaceTracker(FaceDetectorPtr det, FaceRecognizerWrapperPtr rec, Parameters params);

  Faces process(const cv::Mat& frame);

private:
  Faces processSingleFrame(const cv::Mat& frame) const;
  Faces postProcess(Faces faces);
  void updateRecognizer(const FaceSequence& seq);
  std::string labelCreator(void);

  // detection and recognition for single frame
  FaceDetectorPtr          det_;
  FaceRecognizerWrapperPtr rec_;
  // expansion over multiple frames
  uint64_t    frameNo_;
  Parameters  params_;
  FaceHistory hist_;
};

}

#endif
