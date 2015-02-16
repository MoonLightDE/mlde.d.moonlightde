/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 * This file is part of Moonlight Desktop Environment.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#include "module_config.h"
#include "PanelImpl.h"

#include <qt5xdg/XdgMenu>

#include <usModule.h>
#include <usGetModuleContext.h>
#include <usServiceReference.h>
#include <usServiceException.h>

#include <KWindowSystem>

#include <QDebug>
#include <QMenu>
#include <QWidget>
#include <QWindow>
#include <QX11Info>
#include <QPointer>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>



#include <LXQt/XfitMan>
#include <X11/Xatom.h>


using namespace us;

PanelImpl::PanelImpl(QWidget *parent) :
QWidget(parent), m_Desktop(-1), m_WidgetTracker(this) {

    setWindowFlags(Qt::WindowDoesNotAcceptFocus);
    adjustSizeToScreen();
    setupWindowFlags();

    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(adjustSizeToScreen()));
}

PanelImpl::~PanelImpl() {
}

bool PanelImpl::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::Show:
            requestExclusiveScreenArea();
            break;
        case QEvent::ContextMenu:
            break;

        case QEvent::LayoutRequest:
            qDebug() << MODULE_NAME << ": desktop layout changed.";
            adjustSizeToScreen();
            break;

        case QEvent::WinIdChange:
            qDebug() << MODULE_NAME << ": Wid changed.";
            setupWindowFlags();
            adjustSizeToScreen();
        default:
            break;
    }
    return QWidget::event(event);
}

void PanelImpl::adjustSizeToScreen() {
    // TODO: Hard coded values
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    m_Geometry.setWidth(screenGeometry.width());
    m_Geometry.setHeight(36);
    m_Geometry.moveBottomLeft(screenGeometry.bottomLeft());

    QWidget::setGeometry(m_Geometry);

    qDebug() << MODULE_NAME << ": Adjusting size to: " << m_Geometry.size();
    qDebug() << MODULE_NAME << ": Adjusting position to: " << m_Geometry.topLeft();
    qDebug() << MODULE_NAME << ": resulting geometry: " << geometry();

    // Update reserved screen area on resize
    requestExclusiveScreenArea();
}

inline void PanelImpl::setupWindowFlags() {
    if (m_Desktop < 0)
        KWindowSystem::setOnAllDesktops(effectiveWinId(), true);
    else {
        KWindowSystem::setOnAllDesktops(effectiveWinId(), false);
        KWindowSystem::setOnDesktop(effectiveWinId(), m_Desktop);
    }

    KWindowSystem::setType(effectiveWinId(), NET::Dock);
}

void PanelImpl::requestExclusiveScreenArea() {
    // TODO: Look for an alternate implementation inside the KF5.
    Window wid = effectiveWinId();
    if (wid == 0 || !isVisible())
        return;

    LxQt::XfitMan xf = LxQt::xfitMan();
    const QRect wholeScreen = QApplication::desktop()->geometry();
    // NOTE: http://standards.freedesktop.org/wm-spec/wm-spec-latest.html
    // Quote from the EWMH spec: " Note that the strut is relative to the screen edge, and not the edge of the xinerama monitor."
    // So, we use the geometry of the whole screen to calculate the strut rather than using the geometry of individual monitors.
    // Though the spec only mention Xinerama and did not mention XRandR, the rule should still be applied.
    // At least openbox is implemented like this.

    xf.setStrut(wid, 0, 0, 0, wholeScreen.bottom() - m_Geometry.y(),
            /* Left   */ 0, 0,
            /* Right  */ 0, 0,
            /* Top    */ 0, 0,
            /* Bottom */ m_Geometry.left(), m_Geometry.right()
            );
}

void PanelImpl::addWidgetFactory(presentation_panel::WidgetFactory* widgetFactory) {
    if (widgetFactory == NULL) {
        qWarning() << MODULE_NAME << " : adding a NULL object widget factory.";
        return;
    }

    QString name = widgetFactory->name();
    if (m_Widgets.contains(name)) {
        qDebug() << MODULE_NAME << " : " << name << " already registered.";
        return;
    }

    QWidget * widget = widgetFactory->build(MODULE_NAME, this);
    if (widget == NULL) {
        qDebug() << MODULE_NAME << " : " << name << " widget wasn't built.";
        return;
    }

    m_Widgets.insert(name, widget);
    m_Factories.insert(name, widgetFactory);

    updateLayout();
}

void PanelImpl::removeWidgetFactory(presentation_panel::WidgetFactory* widgetFactory) {
    if (widgetFactory == NULL) {
        qWarning() << MODULE_NAME << ": traing to remove a NULL widget factory.";
        return;
    }
    QString name = widgetFactory->name();
    QPointer<QWidget> widget = m_Widgets.value(name);

    m_Widgets.remove(name);
    m_Factories.remove(name);

    if (!widget.isNull())
        delete widget;

    updateLayout();
}

void PanelImpl::updateLayout() {
    // Destroy layout;
    delete layout();

    QHBoxLayout * newLayout = new QHBoxLayout(this);

    newLayout->addWidget(m_Widgets.value(presentation_panel::MAINMENUBUTTON, NULL));
    newLayout->addWidget(m_Widgets.value(presentation_panel::USERTASKS, NULL));
    newLayout->addSpacing(0);
    newLayout->addWidget(m_Widgets.value(presentation_panel::INDICATORS, NULL));
    newLayout->addWidget(m_Widgets.value(presentation_panel::DATETIME, NULL));
    setLayout(newLayout);
}

int PanelImpl::desktop() {
    return m_Desktop;
}

void PanelImpl::setDesktop(int desktop) {
    KWindowSystem::setOnDesktop(windowHandle()->winId(), desktop);
    m_Desktop = desktop;
}

QRect PanelImpl::geometry() {
    return m_Geometry;
}

void PanelImpl::setGeometry(QRect geometry) {
    qWarning() << MODULE_NAME << __PRETTY_FUNCTION__ << " not soported.";
}



