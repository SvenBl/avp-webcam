#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QImage>
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
    void on_playButton_clicked();

    void on_openVideoFileButton_clicked();

    void updateParameters();

private:
    Ui::VideoPlayer *ui;
    VideoEngine *videoThread;
    ColorKeyerHSV* colorKeyerHSV;
    void setThresholds();
};

#endif // VIDEOPLAYER_H
