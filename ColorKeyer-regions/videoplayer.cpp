#include <QFileDialog>
#include <QCheckBox>
#include "videoplayer.h"
#include "ui_videoplayer.h"

VideoPlayer::VideoPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoPlayer)
    , videoThread(new VideoEngine)
    , colorKeyerHSV(new ColorKeyerHSV())
{
    ui->setupUi(this);
    videoThread->setProcessor(colorKeyerHSV);
    videoThread->openCamera(0,0);
    connect(videoThread, SIGNAL(sendInputImage(const QImage&)), ui->inputFrame, SLOT(setImage(const QImage&)));
    connect(videoThread, SIGNAL(sendProcessedImage(const QImage&)), ui->processedFrame , SLOT(setImage(const QImage&)));
    updateParameters();
}

VideoPlayer::~VideoPlayer()
{
    delete videoThread;
    delete colorKeyerHSV;
    delete ui;
}


void VideoPlayer::on_playButton_clicked()
{
    videoThread->start();
}


void VideoPlayer::updateParameters(){
    // hue thresholds
    int hueValue = ui->hueSlider->value();
    int hueTolerance = ui->hueTolerance->value();
    int hueLowerThreshold = hueValue - hueTolerance;
    if (hueLowerThreshold < 0){
        hueLowerThreshold += 360;
    }
    int hueUpperThreshold = hueValue + hueTolerance;
    if (hueUpperThreshold > 360){
        hueUpperThreshold -= 360;
    }
    colorKeyerHSV->setHueLowerThreshold(hueLowerThreshold);
    colorKeyerHSV->setHueUpperThreshold(hueUpperThreshold);
    ui->labelHue->setNum(hueValue);
    ui->labelHueTolerance->setNum(hueTolerance);

    // saturation thresholds
    int saturationThreshold = ui->saturationSlider->value();
    colorKeyerHSV->setSaturationThreshold(saturationThreshold);
    ui->labelSaturation->setNum(saturationThreshold);

    // alpha
    float alpha = ui->lowpassAlpha->value();
    colorKeyerHSV->setAlpha(alpha/100.f);
    ui->labelAlpha->setNum(alpha);

    // median
    colorKeyerHSV->setMedianEnable(ui->checkBoxMedian->checkState() == Qt::Checked);

    // opening
    colorKeyerHSV->setOpeningEnable(ui->checkBoxOpening->checkState() == Qt::Checked);

    // small regions masking
    colorKeyerHSV->setMaskSmallRegions(ui->checkBoSxSmallRegions->checkState() == Qt::Checked);



}
