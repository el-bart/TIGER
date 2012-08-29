#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>

#include "Video/FaceRecognizerWrapper.hpp"

namespace fs = boost::filesystem;


namespace Video
{

FaceRecognizerWrapper::FaceRecognizerWrapper(FaceRecognizer::TrainingSet set, double avgThRangeScale, boost::filesystem::path workDir):
  workDir_( std::move(workDir) ),
  trainingSet_( std::move(set) ),
  avgThRangeScale_(avgThRangeScale),
  lastFileNumber_(0)
{
  updateImpl();
}


void FaceRecognizerWrapper::update(const Entries& newEntries)
{
  // prepare space for new entries
  trainingSet_.reserveAdditional( newEntries.size() );
  // move all entries to the training set
  for(const auto& e: newEntries)
  {
    trainingSet_.addElement(e.second, e.first); // add to training set
    saveAsFile(e);                              // and save to disk
  }
  // now update the recognizer
  updateImpl();
}


FaceRecognizer::TrainingSet FaceRecognizerWrapper::readTrainingSet(const boost::filesystem::path& workDir)
{
  Video::FaceRecognizer::TrainingSet trainSet;

  // iterate over the root directory's content
  const fs::directory_iterator dirEnd;
  for(fs::directory_iterator dirIt(workDir); dirIt!=dirEnd; ++dirIt)
  {
    const std::string label = dirIt->path().filename().string();
    if( !fs::is_directory( dirIt->path() ) )
      throw Util::Exception( UTIL_LOCSTRM << "'" << dirIt->path().string() << "' is not a directory" );

    // iterate over pictures
    for(fs::directory_iterator picIt(*dirIt); picIt!=dirEnd; ++picIt)
    {
      const fs::path file = picIt->path();
      if( !fs::is_regular_file(file) )
        throw Util::Exception( UTIL_LOCSTRM << "'" << file.string() << "' is not a regular file" );
      const cv::Mat pic = cv::imread( file.string(), 0 ); // image MUST BE GRAY SCALE!
      // add to training set
      trainSet.addElement(pic, label);
    }
  }

  // return final set
  return trainSet;
}


void FaceRecognizerWrapper::updateImpl(void)
{
  // train new classifier
  FaceRecognizerPtr tmp( new FaceRecognizer{trainingSet_, avgThRangeScale_} );
  // make it the default one
  swap(tmp, rec_);
  assert( rec_.get()!=nullptr );
}


void FaceRecognizerWrapper::saveAsFile(const Entries::value_type& entry)
{
  fs::path dir = workDir_ / entry.first;
  // ensure output direcotry exists
  if( !( fs::exists(dir) && fs::is_directory(dir) ) )
  {
    if( !fs::create_directory(dir) )
      throw Util::Exception( UTIL_LOCSTRM << "unable to create directory '" << dir.string() << "'" );
  }

  // find unique file name
  fs::path file;
  do
  {
    // generate next file name
    std::stringstream ss;
    ss << "face_img_" << std::setw(9) << std::setfill('0') << lastFileNumber_++ << ".jpg";
    file = dir / ss.str();
  }
  while( fs::exists(file) );

  // write image to a file
  cv::imwrite(file.string(), entry.second);
}

}
