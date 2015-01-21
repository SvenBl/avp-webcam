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

private slots:
    void on_recordButton_clicked();
    void on_open_clicked();
    void on_start_clicked();
    void on_pause_clicked();
    void on_videoPauseButton_clicked();
    void updateParameters();
    void on_stop_clicked();
    void on_next_clicked();
    void on_previous_clicked();
    void on_volume_valueChanged(int value);

private:
    Ui::VideoPlayer *ui;
    VideoEngine *videoThread;
    ColorKeyerHSV* colorKeyerHSV;
    QMediaPlayer *mediaPlayer;
    QMediaPlaylist *mediaPlaylist;
    void setThresholds();
};

#endif // VIDEOPLAYER_H
