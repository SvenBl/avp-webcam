#include "videowidget.h"
#include <QPalette>
VideoWidget::VideoWidget(QWidget *parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    QPalette palette;
    palette.setColor(QPalette::Background,Qt::red);
    setPalette(palette);
}


void VideoWidget::setImage(const QImage& qImage){
    // scale pixmap to label size
    QPixmap pixmap = QPixmap::fromImage(qImage);
    pixmap = pixmap.scaled(this->size(), Qt::KeepAspectRatio);
    setPixmap(pixmap);
}
