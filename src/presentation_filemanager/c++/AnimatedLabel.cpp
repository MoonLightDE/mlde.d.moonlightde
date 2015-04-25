#include "AnimatedLabel.h"
#include <QApplication>
#include <QIcon>
#include <QImage>

AnimatedLabel::AnimatedLabel(QWidget *parent)
    : QLabel(parent), currentPixmap(0)
{
    QIcon icon = QIcon::fromTheme("process-working");
    QPixmap p = icon.pixmap(QSize());
    //p.copy()

    QImage img = p.toImage();

    int subImageHeight = img.width() / 7;

    for (int i = 0; i < 7; i++)
    {
        QImage subImage = img.copy(0, i * subImageHeight, img.width(), subImageHeight);
        pixmaps.push_back(QPixmap::fromImage(subImage));
    }

    connect(&timer, SIGNAL(timeout()), SLOT(changeImage()));
    timer.start(100);
    changeImage();
}

void AnimatedLabel::changeImage()
{
    if (currentPixmap >= pixmaps.length())
        currentPixmap = 0;

    setPixmap(pixmaps.at(currentPixmap));

    currentPixmap++;
}

