#ifndef BUSYDIALOG_H
#define BUSYDIALOG_H

#include "AnimatedLabel.h"

#include <QFrame>
#include <QLabel>
#include <QMovie>
#include <QIcon>
#include <QTimer>

class BusyDialog : public QFrame
{
    Q_OBJECT
public:
    explicit BusyDialog(const QString &text, QWidget *parent = 0);
    void messageSet(const QString &text);

Q_SIGNALS:
    void canceledBusy();

public Q_SLOTS:
    void start();
    void stop();

private:
    QLabel* message;
    QMovie* movie;
    QTimer timer;

};

#endif // BUSYDIALOG_H
