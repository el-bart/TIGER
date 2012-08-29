#include <iomanip>
#include <iostream>
#include <cassert>
#include <boost/filesystem.hpp>

#include "Video/FrameGrabberV4L.hpp"
#include "Video/FaceDetector.hpp"
#include "Video/FaceRecognizer.hpp"
#include "Util/ClockTimer.hpp"

namespace fs = boost::filesystem;
using namespace std;


void markFacesOnImage(const Video::FaceDetector::Faces& faces, const Video::FaceRecognizer& fr, cv::Mat& img)
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
    // try detecting face
    cv::Mat faceGray;
    cvtColor( faceImg, faceGray, CV_BGR2GRAY );
    const char* name = fr.recognize(faceGray);
    if(name==nullptr)
      name = "< UNKNOWN >";
    // check the size of the text
    CvSize txtSize;
    cvGetTextSize(name, &font, &txtSize, nullptr);
    // print the name on the screen, in the middle of the box
    IplImage tmp = img;
    const auto point = cvPoint( f.tl().x+(f.size().width-txtSize.width)/2, f.tl().y+(f.size().height-txtSize.height)/2 );
    cvPutText( &tmp, name, point, &font, CV_RGB(0,255,0) );
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
    cerr << argv[0] << " <video_device> <face_detector_classifier_file> <training_set_dir>" << endl;
    cerr << "\t note that training set dir content has to be in form name1/pic1 name1/pic... name2/pic1 name2/pic2..." << endl;
    return 1;
  }

  cout << "reading training set directory..." << endl;
  Video::FaceRecognizer::TrainingSet trainSet = readTrainSet(argv[3]);
  cout << "got " << trainSet.samples() << " samples" << endl;
  cout << "learning face recognition... (may take a while)" << endl;
  const Util::ClockTimerRT faceRecLearnClk;
  Video::FaceRecognizer faceRecognizer(trainSet, 4.6);
  cout << "learned in " << faceRecLearnClk.elapsed() << "[s]; threshold is " << faceRecognizer.threshold() << endl;

  cout << "initializing face detector..." << endl;
  Video::FaceDetector faceDetector(argv[2], 1.0/6);

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
      markFacesOnImage(faces, faceRecognizer, frame);
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
