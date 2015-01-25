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
    int newPushups = 0;
    int timePerPushup = 10;
    bool doCount = false;
    bool initialized = false;


private slots:
    void updateCounter(int pushups);
    void updateCalc(int pushups);
    void on_open_clicked();
    void on_start_toggled(bool checked);
    void updateParameters();
    void on_next_clicked();
    void on_previous_clicked();
    void on_volume_valueChanged(int value);
    void on_muteButton_toggled(bool checked);
    void updatePosition(qint64 position);

    void on_initialize_clicked();

    void on_upperLineSpinbox_valueChanged(int arg1);

    void on_bottomLineSpinbox_valueChanged(int arg1);

private:
    Ui::VideoPlayer *ui;
    VideoEngine *videoThread;
    ColorKeyerHSV* colorKeyerHSV;
    QMediaPlayer *mediaPlayer;
    QMediaPlaylist *mediaPlaylist;
    void setThresholds();
};

#endif // VIDEOPLAYER_H
