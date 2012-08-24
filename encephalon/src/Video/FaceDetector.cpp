#include "Video/FaceDetector.hpp"


namespace Video
{

FaceDetector::FaceDetector(const std::string& cascadeFile, double scale):
  scale_(scale)
{
  if( cc_.load(cascadeFile) )
    throw Util::Exception( UTIL_LOCSTRM << "unable to load cascade file for face detecion: '" << cascadeFile << "'" );
}


FaceDetector::Faces FaceDetector::detect(const cv::Mat& img)
{
  Faces faces;
  /*
  vector<Rect> faces;
  const static Scalar colors[] =  { CV_RGB(0,0,255),
    CV_RGB(0,128,255),
    CV_RGB(0,255,255),
    CV_RGB(0,255,0),
    CV_RGB(255,128,0),
    CV_RGB(255,255,0),
    CV_RGB(255,0,0),
    CV_RGB(255,0,255)} ;
  Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

  cvtColor( img, gray, CV_BGR2GRAY );
  resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
  equalizeHist( smallImg, smallImg );

  t = (double)cvGetTickCount();
  cascade.detectMultiScale( smallImg, faces,
      1.1, 2, 0
      //|CV_HAAR_FIND_BIGGEST_OBJECT
      //|CV_HAAR_DO_ROUGH_SEARCH
      |CV_HAAR_SCALE_IMAGE
      ,
      Size(30, 30) );
  */
  return faces;
}

}
