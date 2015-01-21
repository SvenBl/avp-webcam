#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H
#include <QLabel>
#include <QImage>

class VideoWidget: public QLabel
{
    Q_OBJECT
public:
    VideoWidget(QWidget*);

public slots:
    void setImage(const QImage&);
public:
    QString label;
};

#endif // VIDEOWIDGET_H
