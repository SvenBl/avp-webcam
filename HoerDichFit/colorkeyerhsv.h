#ifndef COLORKEYERHSV_H
#define COLORKEYERHSV_H

#include "videoprocessor.h"

class ColorKeyerHSV : public VideoProcessor
{
public:
    ColorKeyerHSV();
    void startProcessing(const VideoFormat& format){}
    cv::Mat process(const cv::Mat&source);
    void setHueUpperThreshold(int);
    void setHueLowerThreshold(int);
    void setSaturationThreshold(int);
    void setAlpha(float alpha);
    void setMedianEnable(bool enable);
    void setOpeningEnable(bool enable);
    void setMaskSmallRegions(bool enable);
    int pushups = 0;
    int upperLine = 8;
    int bottomLine = 16;
    int getPushups();
    void checkPushupCounter(cv::Mat& mat, cv::Point center);

private:    
    void drawCross(cv::Mat& mat, cv::Point center, int length, cv::Scalar color);
    void drawLines(cv::Mat& mat, cv::Scalar color);
    void centerOfMass(cv::Mat& image);
    cv::Mat maskSmallRegions(cv::Mat& image);
    cv::Mat colorKeying(cv::Mat& hsvFrame);
private:
    cv::Point center;
    int hueUpperThreshold;
    int hueLowerThreshold;
    int saturationThreshold;
    float alpha;
    bool useMedian;
    bool useOpening;
    bool useMaskSmallRegions;
    bool start;
    bool wasDown;

};

#endif // COLORKEYERHSV_H
