/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *   Maciej Płaza <plaza.maciej@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "lxqttaskbar.h"
#include "lxqttaskbutton.h"

#include "core/ICore.h"

#include <qt5xdg/xdgicon.h>
#include <lxqt/lxqtgridlayout.h>
#include <lxqt/lxqtxfitman.h>

#include <usModuleContext.h>
#include <usGetModuleContext.h>
#include <usServiceReference.h>
#include <usServiceProperties.h>

#include <QApplication>
#include <QDebug>
#include <QToolButton>
#include <QSettings>
#include <QList>
#include <QDesktopWidget>
#include <QWheelEvent>
#include <QX11Info>

using namespace LxQt;

/************************************************

 ************************************************/
LxQtTaskBar::LxQtTaskBar(QWidget *parent) :
QFrame(parent),
mButtonStyle(Qt::ToolButtonTextBesideIcon),
mShowOnlyCurrentDesktopTasks(false),

mPlaceHolder(new LxQtTaskButton(0, this)) {
    // Lookup for settings services
    us::ModuleContext * context = us::GetModuleContext();

    us::ServiceReference<Core::ISettingsProfile> ref =
            context->GetServiceReference<Core::ISettingsProfile>();
    if (!ref) {
        qWarning() << "Unable to find the SettingsProfile service.";
        m_settings = new QSettings(qApp->applicationName(), "Panel/Clock");
    } else {
        Core::ISettingsProfile * settingsProfile = context->GetService(ref);
        m_settings = settingsProfile->getSettingsOf("Panel/Clock");
    }

    // Install event filter
    qApp->installNativeEventFilter(this);

    mLayout = new LxQt::GridLayout(this);
    setLayout(mLayout);
    mLayout->setMargin(0);
    realign();
    mLayout->addWidget(mPlaceHolder);

    mRootWindow = QX11Info::appRootWindow();

    settingsChanged();
}

/************************************************

 ************************************************/
LxQtTaskBar::~LxQtTaskBar() {
}

/************************************************

 ************************************************/
LxQtTaskButton* LxQtTaskBar::buttonByWindow(Window window) const {
    if (mButtonsHash.contains(window))
        return mButtonsHash.value(window);
    return 0;
}

/************************************************

 ************************************************/
bool LxQtTaskBar::windowOnActiveDesktop(Window window) const {
    if (!mShowOnlyCurrentDesktopTasks)
        return true;

    XfitMan xf = xfitMan();
    int desktop = xf.getWindowDesktop(window);
    if (desktop == -1) // Show on all desktops
        return true;

    if (desktop == xf.getActiveDesktop())
        return true;

    return false;
}

/************************************************

 ************************************************/
void LxQtTaskBar::refreshTaskList() {
    XfitMan xf = xfitMan();
    QList<Window> tmp = xf.getClientList();

    //qDebug() << "** Fill ********************************";
    //foreach (Window wnd, tmp)
    //    if (xf->acceptWindow(wnd)) qDebug() << XfitMan::debugWindow(wnd);
    //qDebug() << "****************************************";


    QMutableHashIterator<Window, LxQtTaskButton*> i(mButtonsHash);
    while (i.hasNext()) {
        i.next();
        int n = tmp.removeAll(i.key());

        if (!n) {
            delete i.value();
            i.remove();
        }
    }

    foreach(Window wnd, tmp) {
        if (xf.acceptWindow(wnd)) {
            LxQtTaskButton* btn = new LxQtTaskButton(wnd, this);
            btn->setToolButtonStyle(mButtonStyle);

            mButtonsHash.insert(wnd, btn);
            mLayout->addWidget(btn);
        }
    }
    refreshButtonVisibility();
    mLayout->invalidate();
    activeWindowChanged();
    realign();
}

/************************************************

 ************************************************/

void LxQtTaskBar::refreshButtonVisibility() {
    bool haveVisibleWindow = false;
    QHashIterator<Window, LxQtTaskButton*> i(mButtonsHash);
    while (i.hasNext()) {
        i.next();
        bool isVisible = windowOnActiveDesktop(i.key());
        haveVisibleWindow |= isVisible;
        i.value()->setVisible(isVisible);
    }
    mPlaceHolder->setVisible(!haveVisibleWindow);
}

/************************************************

 ************************************************/

void LxQtTaskBar::refreshIconGeometry() {
    // FIXME: sometimes we get wrong globalPos here, especially
    //        after changing the pos or size of the panel.
    //        this might be caused by bugs in lxqtpanel.cpp.
    QHashIterator<Window, LxQtTaskButton*> i(mButtonsHash);
    while (i.hasNext()) {
        i.next();
        LxQtTaskButton* button = i.value();
        QRect rect = button->geometry();
        QPoint globalPos = mapToGlobal(button->pos());
        rect.moveTo(globalPos);
        //TODO: check this function
        //xfitMan().setIconGeometry(button->windowId(), &rect);
    }
}

/************************************************

 ************************************************/
void LxQtTaskBar::activeWindowChanged() {
    Window window = xfitMan().getActiveAppWindow();

    LxQtTaskButton* btn = buttonByWindow(window);

    if (btn)
        btn->setChecked(true);
    else
        LxQtTaskButton::unCheckAll();
}

/************************************************

 ************************************************/

bool LxQtTaskBar::nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE {
    if (eventType == "xcb_generic_event_t") {

        xcb_generic_event_t* ev = static_cast<xcb_generic_event_t *> (message);
        switch (ev->response_type) {
            case XCB_CONFIGURE_NOTIFY:
            {
                xcb_configure_notify_event_t *config = (xcb_configure_notify_event_t *) ev;

                if (config->window == effectiveWinId()) {
                    refreshIconGeometry();
                    qDebug() << "Refresh geomerty";
                }

            }
            case XCB_PROPERTY_NOTIFY:
            {
                xcb_property_notify_event_t *property = (xcb_property_notify_event_t *) ev;
                handlePropertyNotify(property);
                break;
            }
        }
        return false;
    }
}

/************************************************

 ************************************************/
void LxQtTaskBar::handlePropertyNotify(xcb_property_notify_event_t * event) {
    if (event->window == mRootWindow) {
        // Windows list changed ...............................
        if (event->atom == XfitMan::atom("_NET_CLIENT_LIST")) {
            refreshTaskList();
            return;
        }

        // Activate window ....................................
        if (event->atom == XfitMan::atom("_NET_ACTIVE_WINDOW")) {
            activeWindowChanged();
            return;
        }

        // Desktop switch .....................................
        if (event->atom == XfitMan::atom("_NET_CURRENT_DESKTOP")) {
            if (mShowOnlyCurrentDesktopTasks)
                refreshTaskList();
            return;
        }
    } else {
        LxQtTaskButton* btn = buttonByWindow(event->window);
        if (btn)
            btn->handlePropertyNotify(event);
    }

//        char* aname = XGetAtomName(QX11Info::display(), event->atom);
//        qDebug() << "** XPropertyEvent ********************";
//        qDebug() << "  atom:       0x" << hex << event->atom
//                << " (" << (aname ? aname : "Unknown") << ')';
//        qDebug() << "  window:    " << XfitMan::debugWindow(event->window);
//        qDebug() << "  display:   " << event->display;
//        qDebug() << "  send_event:" << event->send_event;
//        qDebug() << "  serial:    " << event->serial;
//        qDebug() << "  state:     " << event->state;
//        qDebug() << "  time:      " << event->time;
//        qDebug();

}

/************************************************

 ************************************************/
void LxQtTaskBar::setButtonStyle(Qt::ToolButtonStyle buttonStyle) {
    mButtonStyle = buttonStyle;

    QHashIterator<Window, LxQtTaskButton*> i(mButtonsHash);
    while (i.hasNext()) {
        i.next();
        i.value()->setToolButtonStyle(mButtonStyle);
    }
}

/************************************************

 ************************************************/
void LxQtTaskBar::settingsChanged() {
    mButtonWidth = m_settings->value("buttonWidth", 400).toInt();
    QString s = m_settings->value("buttonStyle").toString().toUpper();

    if (s == "ICON") {
        setButtonStyle(Qt::ToolButtonIconOnly);
    } else if (s == "TEXT") {
        setButtonStyle(Qt::ToolButtonTextOnly);
    } else {
        setButtonStyle(Qt::ToolButtonTextBesideIcon);
    }

    mShowOnlyCurrentDesktopTasks = m_settings->value("showOnlyCurrentDesktopTasks", mShowOnlyCurrentDesktopTasks).toBool();
    LxQtTaskButton::setShowOnlyCurrentDesktopTasks(mShowOnlyCurrentDesktopTasks);
    LxQtTaskButton::setCloseOnMiddleClick(m_settings->value("closeOnMiddleClick", true).toBool());
    refreshTaskList();
}

/************************************************

 ************************************************/
void LxQtTaskBar::realign() {
    mLayout->setEnabled(false);

    QSize maxSize = QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    QSize minSize = QSize(0, 0);



    if (mButtonStyle == Qt::ToolButtonIconOnly) {
        // Horizontal + Icons **************
        mLayout->setRowCount(1);
        mLayout->setColumnCount(0);
        mLayout->setStretch(LxQt::GridLayout::StretchVertical);

        minSize.rheight() = 0;
        minSize.rwidth() = 0;

        maxSize.rheight() = QWIDGETSIZE_MAX;
        maxSize.rwidth() = mButtonWidth;
    } else {
        // Horizontal + Text ***************
        mLayout->setRowCount(1);
        mLayout->setColumnCount(0);
        mLayout->setStretch(LxQt::GridLayout::StretchHorizontal | LxQt::GridLayout::StretchVertical);

        minSize.rheight() = 0;
        minSize.rwidth() = 0;

        maxSize.rheight() = QWIDGETSIZE_MAX;
        maxSize.rwidth() = mButtonWidth;
    }


    mLayout->setCellMinimumSize(minSize);
    mLayout->setCellMaximumSize(maxSize);


    mPlaceHolder->setFixedWidth(0);


    mLayout->setEnabled(true);
    refreshIconGeometry();
}

/************************************************

 ************************************************/
void LxQtTaskBar::wheelEvent(QWheelEvent * event) {
    XfitMan xf = xfitMan();
    QList<Window> winList = xf.getClientList();
    int current = winList.indexOf(xf.getActiveAppWindow());
    int delta = event->delta() < 0 ? 1 : -1;

    for (int ix = current + delta; 0 <= ix && ix < winList.size(); ix += delta) {
        Window window = winList.at(ix);
        if (xf.acceptWindow(window) && windowOnActiveDesktop(window)) {
            xf.raiseWindow(window);
            break;
        }
    }
}

