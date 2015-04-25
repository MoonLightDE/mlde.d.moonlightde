#ifndef ANIM_H
#define ANIM_H
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QLabel>
#include <QList>
#include <QPixmap>

class AnimatedLabel : public QLabel
{
    Q_OBJECT

public:
    AnimatedLabel(QWidget* parent = 0);

private Q_SLOTS:
    void changeImage();

private:
    QList<QPixmap> pixmaps;
    int currentPixmap;
    QTimer timer;
};

#endif // ANIM_H
