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
  class ImageNormalizer
  {
  public:
    explicit ImageNormalizer(const cv::Size size):
      size_(size)
    { }

    cv::Mat normalize(const cv::Mat& in) const;

  private:
    mutable cv::Mat tmp_[2];
    cv::Size        size_;
  };

  class TrainingSet
  {
  private:
    typedef std::pair<cv::string,cv::Mat> Entry;

  public:
    typedef std::vector<Entry> Entries;

    explicit TrainingSet(const cv::Size size):
      imgNorm_(size)
    { }

    void addElement(cv::Mat face, std::string name)
    {
      entries_.push_back( Entry{ std::move(name), imgNorm_.normalize(face) } );
    }

    void shuffle(void);

    size_t samples(void) const { return entries_.size(); }

    const Entries& entries() const { return entries_; }

    void reserveAdditional(const size_t more) { entries_.reserve( entries_.capacity() + more ); }

    const ImageNormalizer& normalizer(void) const { return imgNorm_; }

  private:
    Entries         entries_;
    ImageNormalizer imgNorm_;
  };


  // avgThRangeScale - scale factor for similarity threshold scale, computed from average distance between elements
  //                   in the same class. if minimum distance between every element, within the set, is in the range
  //                   [300;500] than acceptance threshold will be equal to 300+(500-300)*avgThRangeScale. this value
  //                   should be at least 1.0. values between 2-6 are reasonable
  FaceRecognizer(const TrainingSet& set, double avgThRangeScale, double maxThreshold);

  const char* recognize(const cv::Mat& faceIn) const;

  void swap(FaceRecognizer& other);

  double threshold(void) const { return threshold_; }

private:
  typedef std::unique_ptr<cv::FaceRecognizer> FaceRecognizerPtr;
  typedef std::pair<int,std::string>          LabelToName;
  typedef std::vector<LabelToName>            LabelMap;

  FaceRecognizerPtr makeFaceRecognizer(void) const;

  ImageNormalizer   imgNorm_;
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
