#include "SidePanel.h"
#include "ui_SidePanel.h"

#include "panel/IPanel.h"

#include <usModuleContext.h>
#include <usGetModuleContext.h>
#include <usServiceReference.h>
#include <usServiceProperties.h>

#include <QDebug>
#include <QRect>
#include <QWindow>
#include <QLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <qt5/QtCore/qnamespace.h>

SidePanel::SidePanel(QWidget *parent) :
QWidget(parent),
ui(new Ui::SidePanel) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);

    us::ModuleContext * context = us::GetModuleContext();
    us::ServiceReference<IPanel> ref =
            context->GetServiceReference<IPanel>();
    QWidget * panel;
    if (!ref) {
        qWarning() << "Unable to find the IPanel service.";
    } else {
        panel = dynamic_cast<QWidget*> (context->GetService(ref));
    }

//     Set position and size
    const QRect screen = QApplication::desktop()->screenGeometry();
    if (panel)
        setGeometry(QRect(screen.width() - screen.width() * 0.25, 0,
            width(), QApplication::desktop()->screenGeometry().height() - panel->height()));
    else
        setGeometry(QRect(screen.width() - screen.width() * 0.25, 0,
            width(), QApplication::desktop()->screenGeometry().height()));
}

void SidePanel::showWidget(QWidget *widget, const bool autohide) {
    clearWidget();
    m_autohide = autohide;

    layout()->addWidget(widget);
    m_widget = widget;
    
    updateGeometry();
    
    show();
    raise();
    activateWindow();
}

void SidePanel::clearWidget() {
    if (!m_widget.isNull()) {
        layout()->removeWidget(m_widget);
    }
}

SidePanel::~SidePanel() {
    delete ui;
}
