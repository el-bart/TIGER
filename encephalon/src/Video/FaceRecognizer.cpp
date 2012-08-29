#include <algorithm>
#include <cassert>

#include "Video/FaceRecognizer.hpp"


namespace Video
{

FaceRecognizer::FaceRecognizer(const TrainingSet& set, const double avgThRangeScale):
  avgThRangeScale_(avgThRangeScale),
  faceRecognizer_( makeFaceRecognizer() )
{
  if( set.entries().size() < 2 )
    throw Util::Exception{ UTIL_LOCSTRM << "training samples set needs to have at least 2 elements" };

  int                  nextFreeLabel = 0;
  std::vector<cv::Mat> faces;
  std::vector<int>     labels;
  std::vector<cv::Mat> testSet;
  // prepare space for the destination data
  faces.reserve( set.samples() );
  labels.reserve( set.samples() );
  labMap_.reserve( set.samples() );
  testSet.reserve( set.samples() );

  // prepare data in the format used by the learning algorithm
  for( const auto& e: set.entries() )
  {
    const std::string& name = e.first;
    const cv::Mat&     face = e.second;
    // find/create id to assign
    int id = -1;
    // initially this vector is ordered by string names
    const LabelMap::value_type searchValue(-1/*whatever*/, name);
    const auto                 swo = [](const LabelMap::value_type& lhs, const LabelMap::value_type& rhs) -> bool { return lhs.second < rhs.second; };
    const auto                 it  = std::lower_bound( labMap_.begin(), labMap_.end(), searchValue, swo );
    if( it==labMap_.end() || it->second!=name )
    {
      // assign new label
      id = nextFreeLabel;
      ++nextFreeLabel;
      labMap_.insert( it, LabelMap::value_type{id, name} );
      // first image from every class keep as a test one
      testSet.push_back(face);
      continue;
    }
    else
    {
      // use already assigned id
      assert( it->second==name );
      id = it->first;
    }
    // add to the containers
    faces.push_back(face);
    labels.push_back(id);
  }

  // learn data
  assert( faceRecognizer_.get() != nullptr );
  assert( faces.size() == labels.size() );
  //for(size_t i=0; i<faces.size(); ++i)
  faceRecognizer_->train(faces, labels);

  // set threshold according to what has been learned
  double thMin = 99999999999999;
  double thMax = 0;
  for(const auto& face: testSet)
  {
    int    label = -1;
    double dist  = -1;
    faceRecognizer_->predict(face, label, dist);
    if(dist<thMin)
      thMin = dist;
    if(dist>thMax)
      thMax = dist;
  }
  const double diff = thMax - thMin;
  threshold_ = thMin + diff * avgThRangeScale_;

  // final version needs to be sorted by int ids, for easier search
  {
    const auto swo = [](const LabelMap::value_type& lhs, const LabelMap::value_type& rhs) -> bool { return lhs.first < rhs.first; };
    std::sort( labMap_.begin(), labMap_.end(), swo );
  }
  assert( labMap_.size() == static_cast<size_t>(nextFreeLabel) );
}


const char* FaceRecognizer::recognize(const cv::Mat& face) const
{
  // sanity check
  if( face.type() != CV_8UC1 )
    throw Util::Exception( UTIL_LOCSTRM << "face image for recognition must be grayscale (CV_8UC1)" );

  // try recongnizing given image
  int    label;
  double dist;
  faceRecognizer_->predict(face, label, dist);
  if( dist > threshold_ )   // not simillar enough?
    return nullptr;

  // map found id to the name
  const LabelMap::value_type searchValue(label, std::string{}/*whatever*/);
  const auto                 swo = [](const LabelMap::value_type& lhs, const LabelMap::value_type& rhs) -> bool { return lhs.first < rhs.first; };
  const auto                 it  = std::lower_bound( labMap_.begin(), labMap_.end(), searchValue, swo );
  if( it == labMap_.end() )
    throw std::logic_error( (UTIL_LOCSTRM << "oops - required label id " << label << " not found!").str() );
  return it->second.c_str();
}


void FaceRecognizer::swap(FaceRecognizer& other)
{
  std::swap(avgThRangeScale_, other.avgThRangeScale_);
  std::swap(threshold_, other.threshold_);
  std::swap(faceRecognizer_, other.faceRecognizer_);
  std::swap(labMap_, other.labMap_);
}


FaceRecognizer::FaceRecognizerPtr FaceRecognizer::makeFaceRecognizer(void) const
{
  // we need to do mapping from cv::Ptr<> to std::unique_ptr<>
  cv::Ptr<cv::FaceRecognizer> tmp( cv::createLBPHFaceRecognizer() );
  FaceRecognizerPtr ptr(tmp.obj);
  tmp.obj = nullptr;
  return ptr;
}

}
