#ifndef VIDEOENGINE_H
#define VIDEOENGINE_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include "videoprocessor.h"
#include "cvmattoqimage.h"
#include "videoformat.h"


class VideoEngine : public QThread
{
    Q_OBJECT
public:
    VideoEngine();
    ~VideoEngine();
    void openFile(const QString& file);
    void openCamera(int device = 0, int channel = 0);
    void setProcessor(VideoProcessor*);
    const VideoFormat& videoFormat() const;
    int framePosition();
    VideoProcessor* processor;
    int pushupsOld = 0;
protected:
    void run();
public slots:
    void stop();
signals:
    void sendInputImage(const QImage&);
    void sendProcessedImage(const QImage&);
    void sendCounter(int);
private:
    cv::VideoCapture videoCapture;
    VideoFormat _videoFormat;
    bool stopped;
    QMutex mutex;   
    bool usingCamera;
    int cameraChannel;
};


#endif // VIDEOENGINE_H
