#include "videoplayer.h"
#include "ui_videoplayer.h"
#include <QFileDialog>
#include <QCheckBox>
#include <QDir>
#include <QTime>
#include <QTimer>
#include <QMediaPlayer>
#include <QMediaPlaylist>

// http://doc.qt.digia.com/qt-5.2/qtwinextras-musicplayer-musicplayer-cpp.html

int lastVolume = 0;

VideoPlayer::VideoPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoPlayer)
    , videoThread(new VideoEngine)
    , colorKeyerHSV(new ColorKeyerHSV()){


    mediaPlayer = new QMediaPlayer(this);
    // owned by PlaylistModel
    mediaPlaylist = new QMediaPlaylist(mediaPlayer);
    mediaPlayer->setPlaylist(mediaPlaylist);

    ui->setupUi(this);
    videoThread->setProcessor(colorKeyerHSV);
    videoThread->openCamera(0,0);  
    connect(videoThread, SIGNAL(sendInputImage(const QImage&)), ui->inputFrame, SLOT(setImage(const QImage&)));
    connect(videoThread, SIGNAL(sendProcessedImage(const QImage&)), ui->processedFrame , SLOT(setImage(const QImage&)));
    connect(videoThread, SIGNAL(sendCounter(int)), ui->counterLabel , SLOT(setNum(int)));
    connect(videoThread, SIGNAL(sendCounter(int)), this , SLOT(updateCalc(int)));

    updateParameters();

    ui->open->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->previous->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
    videoThread->start();
}

VideoPlayer::~VideoPlayer(){
    delete videoThread;
    delete colorKeyerHSV;
    delete ui;
}

void VideoPlayer::on_start_toggled(bool checked){
    if (ui->start->isChecked()) {
        if(!mediaPlaylist->isEmpty()){
            mediaPlayer->play();
            ui->start->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        }else {
            on_open_clicked();
        }
    }else {
        mediaPlayer->pause();
        ui->start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void VideoPlayer::on_open_clicked(){


    QStringList fileNames = QFileDialog::getOpenFileNames(this, ("Open Songs"), QDir::homePath());
    foreach (QString argument, fileNames) {
        QUrl url(argument);
        if (url.isValid()) {
            mediaPlaylist->addMedia(url);
        }
        QFileInfo file(argument);
        ui->listWidget->addItem(file.baseName());
    }
    ui->listWidget->setCurrentRow(mediaPlaylist->currentIndex() != -1? mediaPlaylist->currentIndex():0);
}

void VideoPlayer::updateParameters(){
    // hue thresholds
    int hueValue = ui->hueSlider->value();
    int hueTolerance = 30;
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

    // saturation thresholds
    int saturationThreshold = 207;
    colorKeyerHSV->setSaturationThreshold(saturationThreshold);

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

void VideoPlayer::on_stop_clicked(){
    mediaPlayer->stop();
}

void VideoPlayer::on_volume_valueChanged(int value){
    mediaPlayer->setVolume(value);
    if (value == 0){
        ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }else {
        ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
}

void VideoPlayer::on_next_clicked(){
    mediaPlaylist->next();
}

void VideoPlayer::on_previous_clicked(){
    mediaPlaylist->previous();
}

void VideoPlayer::on_muteButton_toggled(bool checked)
{
    if (ui->muteButton->isChecked()){
        lastVolume = ui->volume->value();
        ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        mediaPlayer->setVolume(0);
        ui->volume->setValue(0);
    }else {
        ui->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        ui->volume->setValue(lastVolume);
        mediaPlayer->setVolume(lastVolume);
    }
}


void VideoPlayer::updateCalc(int pushups)
{
    timer = timer + 10;
    qDebug() << timer;
}

void VideoPlayer::updatePosition(qint64 position)
{
    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));   
    if(duration.second() > currentSecond){
        timer = timer -1;
        currentSecond = duration.second();
        ui->remainingTimeLabel->setNum(timer);
    }
    if(duration.second() < currentSecond){
        currentSecond = duration.second();
        ui->remainingTimeLabel->setNum(timer);
    }
    /*if (timer < 1){
        mediaPlayer->pause();
    }else {
        mediaPlayer->play();
    }*/
    ui->positionLabel->setText(duration.toString(tr("mm:ss")));
}

void VideoPlayer::setPosition(qint64 position)
{
    mediaPlayer->setPosition(position);
}

void VideoPlayer::on_startProgram_clicked()
{
    timer = ui->spinBox_2->value();
    ui->remainingTimeLabel->setNum(timer);
}
