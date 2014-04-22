#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include "sidepanel/ISidePanel.h"

#include <QWidget>
#include <QPointer>

namespace Ui {
    class SidePanel;
}

class SidePanel : public QWidget, public ISidePanel {
    Q_OBJECT
public:
    explicit SidePanel(QWidget *parent = 0);
    void showWidget(QWidget *widget, const bool autohide);
    ~SidePanel();
protected:
    void focusOutEvent(QFocusEvent * event);

private:
    void clearWidget();

    bool m_autohide;
    QPointer<QWidget> m_widget;
    Ui::SidePanel *ui;
};

#endif // SIDEPANEL_H
