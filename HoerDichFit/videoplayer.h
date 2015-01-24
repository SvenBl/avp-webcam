#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QImage>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "videoengine.h"
#include "colorkeyerhsv.h"


namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();
    int timer;
    int currentSecond = 0;
    int oldPushups = 0;

private slots:
    void updateCalc(int pushups);
    void on_open_clicked();
    void on_start_toggled(bool checked);
    void updateParameters();
    void on_stop_clicked();
    void on_next_clicked();
    void on_previous_clicked();
    void on_volume_valueChanged(int value);
    void on_muteButton_toggled(bool checked);
    void on_toggleCameraButton_toggled(bool checked);
    void updatePosition(qint64 position);
    void setPosition(qint64 position);

    void on_startProgram_clicked();

private:
    Ui::VideoPlayer *ui;
    VideoEngine *videoThread;
    ColorKeyerHSV* colorKeyerHSV;
    QMediaPlayer *mediaPlayer;
    QMediaPlaylist *mediaPlaylist;
    void setThresholds();
};

#endif // VIDEOPLAYER_H
