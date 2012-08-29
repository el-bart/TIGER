#ifndef INCLUDE_VIDEO_FACERECOGNIZER_HPP_FILE
#define INCLUDE_VIDEO_FACERECOGNIZER_HPP_FILE

#include <string>
#include <memory>
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>

#include "Util/Exception.hpp"


namespace Video
{

class FaceRecognizer
{
public:
  class TrainingSet
  {
  private:
    typedef std::pair<cv::string,cv::Mat> Entry;

  public:
    typedef std::vector<Entry> Entries;

    void addElement(const cv::Mat& face, std::string name)
    {
      if( face.type() != CV_8UC1 )
        throw Util::Exception( UTIL_LOCSTRM << "face image for learning must be grayscale (CV_8UC1)" );
      entries_.push_back( Entry{std::move(name), face} );
    }

    size_t samples(void) const { return entries_.size(); }

    const Entries& entries() const { return entries_; }

    void reserveAdditional(const size_t more) { entries_.reserve( entries_.capacity() + more ); }

  private:
    Entries entries_;
  };


  // avgThRangeScale - scale factor for similarity threshold scale, computed from average distance between elements
  //                   in the same class. if minimum distance between every element, within the set, is in the range
  //                   [300;500] than acceptance threshold will be equal to 300+(500-300)*avgThRangeScale. this value
  //                   should be at least 1.0. values between 2-6 are reasonable
  FaceRecognizer(const TrainingSet& set, double avgThRangeScale);

  const char* recognize(const cv::Mat& face) const;

  void swap(FaceRecognizer& other);

  double threshold(void) const { return threshold_; }

private:
  typedef std::unique_ptr<cv::FaceRecognizer> FaceRecognizerPtr;
  typedef std::pair<int,std::string>          LabelToName;
  typedef std::vector<LabelToName>            LabelMap;

  FaceRecognizerPtr makeFaceRecognizer(void) const;

  double            avgThRangeScale_;
  double            threshold_;
  FaceRecognizerPtr faceRecognizer_;
  LabelMap          labMap_;
};


typedef std::unique_ptr<FaceRecognizer> FaceRecognizerPtr;


inline void swap(FaceRecognizer& first, FaceRecognizer& second)
{
  first.swap(second);
}

}

#endif
