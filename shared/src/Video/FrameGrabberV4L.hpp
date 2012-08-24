#ifndef INCLUDE_VIDEO_FRAMEGRABBERV4L_HPP_FILE
#define INCLUDE_VIDEO_FRAMEGRABBERV4L_HPP_FILE

#include <vector>
#include <libv4l2.h>

#include "Util/UniqueDescriptor.hpp"
#include "Util/MappedMem.hpp"
#include "Video/FrameGrabber.hpp"

namespace Video
{

class FrameGrabberV4L: public FrameGrabber
{
public:
  FrameGrabberV4L(std::string devPath, size_t width, size_t height);

private:
  typedef Util::MappedMem<void, v4l2_mmap, v4l2_munmap> MMem;

  virtual cv::Mat  grabImpl(double timeout);
  virtual cv::Size sizeImpl(void);

  Util::UniqueDescriptor openDevice(void) const;
  void init(size_t width, size_t height);
  void startCapture(void);
  cv::Mat toRGB(void* mem, size_t length) const;

  std::string            devPath_;
  Util::UniqueDescriptor dev_;

  unsigned width_;
  unsigned height_;
  unsigned bytesPerLine_;

  std::vector<MMem> buffers_;
};

}

#endif
