#include "BusyDialog.h"
#include <QHBoxLayout>
#include <QImage>

BusyDialog::BusyDialog(const QString &text, QWidget *parent) :
    QFrame(parent)
{
    setObjectName("busy");

    setAutoFillBackground(true);


    AnimatedLabel *label = new AnimatedLabel();
    //QIcon icon = QIcon::fromTheme("process-working");
    //movie = new QMovie(":/images/busy.gif");
    //label->setMovie(movie);

    message = new QLabel(text);

    QHBoxLayout *lt = new QHBoxLayout;
    lt->addWidget(label);
    lt->addWidget(message,2);
    lt->addSpacing(5);
    lt->setMargin(2);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setLayout(lt);

    setStyleSheet("QFrame#busy{border-width: 1px; "
                  "border-style: solid; "
                  "border-color: lightgray;"
                  "margin-right: 3px;"
                  "border-radius: 5px;"
                  "background-color: white;}");

}

void BusyDialog::messageSet(const QString &text){
    message->setText(text);
}

void BusyDialog::start(){
    //movie->start();
    resize(sizeHint());
}

void BusyDialog::stop(){
    //movie->stop();
}







