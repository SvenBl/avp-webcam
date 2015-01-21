#include <opencv2/opencv.hpp>
#include <QImage>
#include <QMutexLocker>
#include <QDir>
#include <QDebug>
#include <string>
#include "videoengine.h"

// http://stackoverflow.com/questions/7801522/integrating-opencv-with-larger-programs

VideoEngine::VideoEngine()
    : stopped(false)
    , processor(0)
    , usingCamera(false)
{
}
VideoEngine::~VideoEngine(){
    stop();
    wait();
}
const VideoFormat& VideoEngine::videoFormat() const{
    return _videoFormat;
}
void VideoEngine::setProcessor(VideoProcessor *processor){
    this->processor = processor;
}

void VideoEngine::openFile(const QString& file){
    try {
        bool success = videoCapture.open(file.toStdString());
        if (!success){
            qDebug() << "Error: cannot open File: " << file;
        }
        else{
            _videoFormat.setFormat(videoCapture);
            qDebug() << "Successfully opened file: " << file;
            qDebug() << "Format: " << _videoFormat;
        }
    } catch (cv::Exception e) {
       qDebug() << e.err.c_str();
    }
}

void VideoEngine::openCamera(int device, int channel){
    cameraChannel = channel;
    try {
        bool success = videoCapture.open(device);
        if (!success){
            qDebug() << "Error: cannot open camera ";
        }
        else{
            _videoFormat.setFormat(videoCapture);
            qDebug() << "Successfully opened from camera";
            qDebug() << "Format: " << _videoFormat;
        }
    } catch (cv::Exception e) {
       qDebug() << e.err.c_str();
    }
}

void VideoEngine::stop()
{
    QMutexLocker locker(&mutex);
    stopped = true;
}

void VideoEngine::run()
{
    float fps = _videoFormat.framesPerSecond();
    if (fps == 0){
        fps = 30;
    }
    int milliSeconds = 1000/fps;
    int frameNumber = 0;
    while(!stopped)
    {
        cv::Mat cvFrame;
        if (false == videoCapture.grab()){
            qDebug() << "grab() failed";
            break;
        }
        if (false == videoCapture.retrieve(cvFrame, cameraChannel)){
            qDebug() << "retreive failed";
            break;
        }


        // retrieve Mat::type()
        frameNumber++;
        if (frameNumber == 1){
            _videoFormat.setType(cvFrame.type());
            if (processor != 0){
                processor->startProcessing(_videoFormat);
            }
        }

        // queue the image to the gui
        emit sendInputImage(cvMatToQImage(cvFrame));

        // Process Video Frame
        if (processor != 0){
            cvFrame = processor->process(cvFrame);
        }

        emit sendProcessedImage(cvMatToQImage(cvFrame));

        // check if stopped
        QMutexLocker locker(&mutex);
        if (stopped) {
            break;
        }

        if (usingCamera == false){
            msleep(milliSeconds);
        }
    }
    videoCapture.release();
}

int VideoEngine::framePosition(){
    return videoCapture.get(CV_CAP_PROP_POS_FRAMES);
}
