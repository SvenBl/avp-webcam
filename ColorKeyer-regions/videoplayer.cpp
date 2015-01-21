#include <QCheckBox>
#include "videoplayer.h"
#include "ui_videoplayer.h"
#include <QFileDialog>
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>


VideoPlayer::VideoPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoPlayer)
    , videoThread(new VideoEngine)
    , colorKeyerHSV(new ColorKeyerHSV())
{

    mediaPlayer = new QMediaPlayer(this);
    // owned by PlaylistModel
    mediaPlaylist = new QMediaPlaylist();
    mediaPlayer->setPlaylist(mediaPlaylist);

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


void VideoPlayer::on_recordButton_clicked()
{
    videoThread->start();
}
void VideoPlayer::on_videoPauseButton_clicked()
{
    videoThread->stop();
}

void VideoPlayer::on_start_clicked()
{
    if(!mediaPlaylist->isEmpty()){
        mediaPlayer->play();
    }else {
        on_open_clicked();
    }
}
void VideoPlayer::on_open_clicked()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    //mediaPlaylist->addMedia(QUrl::fromLocalFile(fileName));

    QStringList fileNames = QFileDialog::getOpenFileNames(this, ("Open Songs"), QDir::homePath());
    foreach (QString argument, fileNames) {
        QUrl url(argument);
        if (url.isValid()) {
            mediaPlaylist->addMedia(url);
        }
        QFileInfo file(argument);
        ui->listWidget->addItem(file.baseName());
    }
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
    float alpha = 100;
    colorKeyerHSV->setAlpha(alpha/100.f);

    // median
    colorKeyerHSV->setMedianEnable(ui->checkBoxMedian->checkState() == Qt::Checked);

    // opening
    colorKeyerHSV->setOpeningEnable(ui->checkBoxOpening->checkState() == Qt::Checked);

    // small regions masking
    colorKeyerHSV->setMaskSmallRegions(ui->checkBoSxSmallRegions->checkState() == Qt::Checked);



}

void VideoPlayer::on_stop_clicked()
{
    mediaPlayer->stop();
}

void VideoPlayer::on_pause_clicked()
{
    mediaPlayer->pause();
}
void VideoPlayer::on_volume_valueChanged(int value)
{
    mediaPlayer->setVolume(value);
}

void VideoPlayer::on_next_clicked()
{
    mediaPlaylist->next();
}

void VideoPlayer::on_previous_clicked()
{
    mediaPlaylist->previous();
}
