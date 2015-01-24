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
    connect(videoThread, SIGNAL(sendProcessedImage(const QImage&)), ui->processedFrame , SLOT(setImage(const QImage&)));
    connect(videoThread, SIGNAL(sendCounter(int)), this , SLOT(updateCounter(int)));
    connect(videoThread, SIGNAL(sendCounter(int)), this , SLOT(updateCalc(int)));

    updateParameters();

    ui->open->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
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
    ui->start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void VideoPlayer::updateParameters(){
    // hue thresholds
    int hueValue = 0;
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

    // saturation thresholds
    int saturationThreshold = 207;
    colorKeyerHSV->setSaturationThreshold(saturationThreshold);

    // alpha
    float alpha = 100;
    colorKeyerHSV->setAlpha(alpha/100.f);

    // median
    colorKeyerHSV->setMedianEnable(false);

    // opening
    colorKeyerHSV->setOpeningEnable(false);

    // small regions masking
    colorKeyerHSV->setMaskSmallRegions(true);
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
    if(doCount){
        if(timer == 0){
            mediaPlayer->play();
            ui->start->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        }
        timer = timer + timePerPushup;
        ui->remainingTimeLabel->setNum(timer);
    }
}

void VideoPlayer::updateCounter(int pushups)
{
    if(doCount){
        newPushups = newPushups +1;
        ui->counterLabel->setNum(newPushups);
    }
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
    if (timer == 0){
        mediaPlayer->pause();
        ui->start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    ui->positionLabel->setText(duration.toString(tr("mm:ss")));
}

void VideoPlayer::setPosition(qint64 position)
{
    mediaPlayer->setPosition(position);
}

void VideoPlayer::on_initialize_clicked()
{
    if(initialized){
        doCount = false;
        ui->initialize->setText("Initialize");
        ui->initialTimeSpinBox->setEnabled(true);
        ui->timePerPushupSpinBox->setEnabled(true);
        ui->upperLineSpinbox->setEnabled(true);
        ui->bottomLineSpinbox->setEnabled(true);
        initialized = false;
    }
    else{
        doCount = true;
        timer = ui->initialTimeSpinBox->value();
        timePerPushup = ui->timePerPushupSpinBox->value();
        ui->remainingTimeLabel->setNum(timer);
        ui->initialTimeSpinBox->setEnabled(false);
        ui->timePerPushupSpinBox->setEnabled(false);
        ui->upperLineSpinbox->setEnabled(false);
        ui->bottomLineSpinbox->setEnabled(false);
        ui->initialize->setText("New settings");
        initialized = true;
    }

}

void VideoPlayer::on_upperLineSpinbox_valueChanged(int arg1)
{
    colorKeyerHSV->upperLine = arg1;
}

void VideoPlayer::on_bottomLineSpinbox_valueChanged(int arg1)
{
    colorKeyerHSV->bottomLine = arg1;
}
