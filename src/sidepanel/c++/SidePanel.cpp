#include "SidePanel.h"

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

SidePanel::SidePanel(QObject *parent) : QObject(parent) {
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
    // Hide old widget
    if (m_widget)
        m_widget->hide();


    m_widget = widget;

    if (autohide)
        m_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    else
        m_widget->setWindowFlags(Qt::FramelessWindowHint);

    // Correct position
    m_widget->adjustSize();
    const QRect screen = QApplication::desktop()->screenGeometry();
    if (panel)
        m_widget->setGeometry(QRect(screen.width() - m_widget->width(), 0,
            m_widget->width(), QApplication::desktop()->screenGeometry().height() - panel->height()));
    else
        m_widget->setGeometry(QRect(screen.width() - m_widget->width(), 0,
            m_widget->width(), QApplication::desktop()->screenGeometry().height()));

    m_widget->show();
    m_widget->raise();
    m_widget->activateWindow();
}

void SidePanel::hideEvent(QHideEvent * event) {
    m_widget->hide();
    m_widget = NULL;
}

SidePanel::~SidePanel() {
}
