#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>
#include "videoformat.h"

class VideoProcessor
{
public:
    virtual ~VideoProcessor(){}
    virtual void startProcessing(const VideoFormat& format) = 0;
    virtual cv::Mat process(const cv::Mat&source) = 0;
};



#endif // VIDEOPROCESSOR_H
