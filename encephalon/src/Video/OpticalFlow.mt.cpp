#include <iomanip>
#include <iostream>

#include "Video/FrameGrabberV4L.hpp"
#include "Video/OpticalFlow.hpp"
#include "Util/ClockTimer.hpp"

using namespace std;


void drawOpticalFlow(const Video::OpticalFlow::Flow& flow, cv::Mat& img, const size_t step)
{
  const cv::Size size  = flow.size();
  const double   scale = img.size().width/double(size.width);

  // draw each arrow
  for(size_t y=0; y<size.height; y+=step)
    for(size_t x=0; x<size.width; x+=step)
    {
      const cv::Point2f dxy   = flow.at(y,x);
      const cv::Scalar  color = CV_RGB(0,255,0);
      const cv::Point   start(x*scale, y*scale);
      const cv::Point   stop( cvRound((x+dxy.x)*scale), cvRound((y+dxy.y)*scale) );
      // draw it
      line(img, start, stop, color);
      circle(img, start, 2, color, -1);

    }
  {
    //const cv::Scalar color = CV_RGB(0,255,0);
    //cv::rectangle(img, f, color);
  }
}


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    cerr << argv[0] << " <video_device>" << endl;
    return 1;
  }

  Video::OpticalFlow opticalFlow(0.5);

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
      cv::Mat                  frame = fg->grab();
      Video::OpticalFlow::Flow flow  = opticalFlow.computeFlow(frame);
      cerr << '.';
      drawOpticalFlow(flow, frame, 10);
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
