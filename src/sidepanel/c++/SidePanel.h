#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include "sidepanel/ISidePanel.h"

#include <QObject>
#include <QPointer>


class SidePanel: public QObject, public ISidePanel {
    Q_OBJECT
public:
    explicit SidePanel(QObject *parent = 0);
    void showWidget(QWidget *widget, const bool autohide);
    ~SidePanel();

protected:
    void hideEvent(QHideEvent * event);

private:
    QPointer<QWidget> m_widget;
    QPointer<QWidget> panel;
};

#endif // SIDEPANEL_H
