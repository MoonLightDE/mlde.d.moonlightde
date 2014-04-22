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
#include <QVBoxLayout>

SidePanel::SidePanel(QWidget *parent) :
QWidget(parent), ui(new Ui::SidePanel()) {
    ui->setupUi(this);

    us::ModuleContext * context = us::GetModuleContext();
    us::ServiceReference<IPanel> ref =
            context->GetServiceReference<IPanel>();

    if (!ref) {
        qWarning() << "Unable to find the IPanel service.";
    } else {
        panel = dynamic_cast<QWidget*> (context->GetService(ref));
    }


}

void SidePanel::showWidget(QWidget *widget, const bool autohide) {
    // Hide to delete old widgets
    hide();
    setGeometry(0,0,0,0);

    ui->verticalLayout->addWidget(widget);
    m_widget = widget;

    if (autohide)
        setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    else
        setWindowFlags(Qt::FramelessWindowHint);

    // Correct position
    adjustSize();
    const QRect screen = QApplication::desktop()->screenGeometry();
    if (panel)
        setGeometry(QRect(screen.width() - m_widget->width() - ui->verticalLayout->margin(), 0,
            m_widget->width(), QApplication::desktop()->screenGeometry().height() - panel->height()));
    else
        setGeometry(QRect(screen.width() - m_widget->width(), 0,
            m_widget->width(), QApplication::desktop()->screenGeometry().height()));
    widget->adjustSize();

    show();
    raise();
    activateWindow();
}

void SidePanel::hideEvent(QHideEvent * event) {
    ui->verticalLayout->removeWidget(m_widget);
    delete m_widget;
}

SidePanel::~SidePanel() {
    delete ui;
}
