#ifndef INCLUDE_VIDEO_FACERECOGNIZERWRAPPER_HPP_FILE
#define INCLUDE_VIDEO_FACERECOGNIZERWRAPPER_HPP_FILE

#include <memory>
#include <utility>
#include <cassert>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include "Util/Exception.hpp"
#include "Video/FaceRecognizer.hpp"


namespace Video
{

class FaceRecognizerWrapper: private boost::noncopyable
{
public:
  typedef FaceRecognizer::TrainingSet::Entries Entries;

  // avgThRangeScale - scale factor for similarity threshold scale, computed from average distance between elements
  //                   in the same class. if minimum distance between every element, within the set, is in the range
  //                   [300;500] than acceptance threshold will be equal to 300+(500-300)*avgThRangeScale. this value
  //                   should be at least 1.0. values between 2-6 are reasonable
  FaceRecognizerWrapper(double avgThRangeScale, double maxThreshold, boost::filesystem::path workDir, cv::Size size);

  const FaceRecognizer* operator->(void) const { assert( rec_.get()!=nullptr ); return rec_.get(); }

  void update(const Entries& newEntries);

  size_t samples(void) const { return trainingSet_.samples(); }

private:
  void updateImpl(void);
  FaceRecognizer::TrainingSet readTrainingSet(cv::Size size) const;
  void saveAsFile(const Entries::value_type& entry);

  const double                avgThRangeScale_;
  const double                maxThreshold_;
  FaceRecognizerPtr           rec_;
  size_t                      lastFileNumber_;
  boost::filesystem::path     workDir_;
  FaceRecognizer::TrainingSet trainingSet_;
};


typedef std::unique_ptr<FaceRecognizerWrapper> FaceRecognizerWrapperPtr;

}

#endif
