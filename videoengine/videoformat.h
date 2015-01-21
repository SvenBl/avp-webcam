#ifndef VIDEOFORMAT_H
#define VIDEOFORMAT_H
#include <opencv2/opencv.hpp>
#include <QDebug>
class VideoFormat
{
public:
    VideoFormat();
    void setFormat(cv::VideoCapture & videoCapture);
    void setType(int type);
    int frameWidth()const ;
    int frameHeight() const;
    int framesPerSecond()const;
    int frameCount()const;
    int type()const;
private:
    int _frameWidth;
    int _frameHeight;
    int _framesPerSecond;
    int _frameCount;
    int _type;
 };
QDebug operator<< ( QDebug os, const VideoFormat & videoFormat );

#endif // VIDEOFORMAT_H
