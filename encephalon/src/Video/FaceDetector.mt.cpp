#include <iomanip>
#include <iostream>

#include "Video/FrameGrabberV4L.hpp"
#include "Video/FaceDetector.hpp"
#include "Util/ClockTimer.hpp"

using namespace std;


void markFacesOnImage(const Video::FaceDetector::Faces& faces, cv::Mat& img)
{
  for(const auto& f: faces)
  {
    const cv::Scalar color = CV_RGB(0,255,0);
    cv::rectangle(img, f, color);
  }
}


int main(int argc, char** argv)
{
  if( argc != 1+2 )
  {
    cerr << argv[0] << " <video_device> <cascade_file>" << endl;
    return 1;
  }

  cout << "initializing face detector..." << endl;
  Video::FaceDetector faceDetector(argv[2], 1.0/6);

  cout << setprecision(3);

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
      cv::Mat                    frame = fg->grab();
      Video::FaceDetector::Faces faces = faceDetector.detect(frame);
      cerr << '.';
      markFacesOnImage(faces, frame);
      // output
      imshow( winName, frame );
      if( cv::waitKey(10) >= 0 )
      {
        cout << endl;
        cout << "breaking on user request";
        break;
      }
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
