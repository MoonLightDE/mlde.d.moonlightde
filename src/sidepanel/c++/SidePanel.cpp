#include "SidePanel.h"
#include "ui_SidePanel.h"

#include <QRect>
#include <QLayout>
#include <QApplication>
#include <QDesktopWidget>

SidePanel::SidePanel(QWidget *parent) :
QWidget(parent),
ui(new Ui::SidePanel) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    setAttribute(Qt::WA_AlwaysShowToolTips);

    setFocusPolicy(Qt::ClickFocus);


    // Set position and size
    const QRect screen = QApplication::desktop()->screenGeometry();
    setGeometry(QRect(screen.width() - width(), 0, width(), maximumHeight()));
}

void SidePanel::showWidget(QWidget *widget, const bool autohide) {
    m_autohide = autohide;

    layout()->addWidget(widget);
    m_widget = widget;
    show();
}

void SidePanel::clearWidget() {
    if (!m_widget.isNull())
        layout()->removeWidget(m_widget);
}

void SidePanel::focusOutEvent(QFocusEvent * event) {
    if (m_autohide) {
        clearWidget();
        hide();
    }
}

SidePanel::~SidePanel() {
    delete ui;
}
