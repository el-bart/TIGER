#include <iomanip>
#include <sstream>
#include <iostream>
#include <cassert>
#include <boost/filesystem.hpp>

#include "Video/FrameGrabberV4L.hpp"
#include "Video/FaceDetector.hpp"
#include "Video/FaceRecognizer.hpp"
#include "Util/ClockTimer.hpp"

namespace fs = boost::filesystem;
using namespace std;


size_t g_fileNo = 0;

void markAndSaveFaces(const Video::FaceDetector::Faces& faces, cv::Mat& img, const fs::path& dir)
{
  CvFont font;
  cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, 1.0);

  for(const auto& f: faces)
  {
    // mark face with a rectangle
    const cv::Scalar color = CV_RGB(0,255,0);
    cv::rectangle(img, f, color);
    // select face region from the image
    cv::Mat faceImg = img(f);
    // save image to a new file
    stringstream ss;
    ss << fixed << setw(10) << setfill('0') << g_fileNo << ".jpg";
    fs::path out = dir / ss.str();
    cv::imwrite( out.string(), faceImg );
    ++g_fileNo;
  }
}


Video::FaceRecognizer::TrainingSet readTrainSet(const fs::path& dir)
{
  Video::FaceRecognizer::TrainingSet trainSet;
  const fs::directory_iterator dirEnd;
  // iterate over the root directory's content
  for(fs::directory_iterator dirIt(dir); dirIt!=dirEnd; ++dirIt)
  {
    const string label = dirIt->path().filename().string();
    assert( fs::is_directory( dirIt->path() ) && "invalid direcotry strucutre" );
    // iterate over pictures
    for(fs::directory_iterator picIt(*dirIt); picIt!=dirEnd; ++picIt)
    {
      const fs::path file = picIt->path();
      assert( fs::is_regular_file(file) && "invalid direcotry strucutre" );
      const cv::Mat  pic  = cv::imread( file.string(), 0 ); // image MUST BE GRAY SCALE!
      // add to training set
      trainSet.addElement(pic, label);
    }
  }
  // return final set
  return trainSet;
}


int main(int argc, char** argv)
{
  cout << setprecision(3);

  if( argc != 1+3 )
  {
    cerr << argv[0] << " <video_device> <face_detector_classifier_file> <output_dir>" << endl;
    return 1;
  }

  cout << "initializing face detector..." << endl;
  Video::FaceDetector faceDetector(argv[2], 1.0/6);

  const fs::path outDir(argv[3]);

  cout << "initializing video device..." << endl;
  const Util::ClockTimerRT clkInit;
  Video::FrameGrabberPtr fg( new Video::FrameGrabberV4L(argv[1], 800, 600) );
  const std::string winName = "image from grabber";
  cv::namedWindow( winName, CV_WINDOW_AUTOSIZE|CV_WINDOW_KEEPRATIO );
  cout << "device initialized in " << clkInit.elapsed() << " second(s)" << endl;

  // check what is the grabbing resolution
  {
    const cv::Size size = fg->size();
    cout << "grabbing resolution set to " << size.width << "x" << size.height << endl;
  }

  cout << "grabbing frames - press any key to stop" << endl;
  const Util::ClockTimerRT clkFps;
  size_t                   i;
  for(i=0; true; ++i)
  {
    try
    {
      // check is exit is requested
      if( cv::waitKey(10) >= 0 )
      {
        cout << endl;
        cout << "breaking on user request";
        break;
      }
      // grab
      cv::Mat                    frame = fg->grab();
      Video::FaceDetector::Faces faces = faceDetector.detect(frame);
      markAndSaveFaces(faces, frame, outDir);
      if( faces.empty() )
        cerr << '.';
      else
        cerr << 'f';
      // output
      imshow( winName, frame );
    }
    catch(const std::exception& ex)
    {
      cerr << 'X' << endl;
      cerr << ex.what() << endl;
    }
  }
  const double fpsT = clkFps.elapsed();
  cout << endl;
  cout << "processed " << i << " frames in " << fpsT << " second(s) (" << i/clkFps.elapsed() << " fps on average)" << endl;
  cout << "all done! :)" << endl;

  return 0;
}
