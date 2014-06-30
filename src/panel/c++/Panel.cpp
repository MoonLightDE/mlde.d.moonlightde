/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 * 
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

#include "Panel.h"
#include "ui_panel.h"

#include <xcb/xcb.h>
#include <LXQt/XfitMan>

#include <QDebug>
#include <QDesktopWidget>
#include <QX11Info>

#include <LXQt/lxqtxfitman.h>

Panel::Panel(QWidget *parent) :
QWidget(parent), ui(new Ui::Panel) {
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    setAttribute(Qt::WA_AlwaysShowToolTips);
    setAttribute(Qt::WA_TranslucentBackground);

    // set freedesktop.org EWMH hints properly
    if (QX11Info::isPlatformX11() && QX11Info::connection()) {
        xcb_connection_t* con = QX11Info::connection();
        const char* atom_name = "_NET_WM_WINDOW_TYPE_DOCK";
        xcb_atom_t atom = xcb_intern_atom_reply(con, xcb_intern_atom(con, 0, strlen(atom_name), atom_name), NULL)->atom;
        const char* prop_atom_name = "_NET_WM_WINDOW_TYPE";
        xcb_atom_t prop_atom = xcb_intern_atom_reply(con, xcb_intern_atom(con, 0, strlen(prop_atom_name), prop_atom_name), NULL)->atom;
        xcb_atom_t XA_ATOM = 4;
        xcb_change_property(con, XCB_PROP_MODE_REPLACE, winId(), prop_atom, XA_ATOM, 32, 1, &atom);

        Atom windowTypes[] = {
            LxQt::xfitMan().atom("_NET_WM_WINDOW_TYPE_DOCK"),
            LxQt::xfitMan().atom("_KDE_NET_WM_WINDOW_TYPE_OVERRIDE"), // required for Qt::FramelessWindowHint
            LxQt::xfitMan().atom("_NET_WM_WINDOW_TYPE_NORMAL")
        };
        XChangeProperty(QX11Info::display(), winId(), LxQt::xfitMan().atom("_NET_WM_WINDOW_TYPE"),
                XA_ATOM, 32, PropModeReplace, (unsigned char*) windowTypes, 3);
    }


    // Services lookup
    us::ModuleContext * context = us::GetModuleContext();
    launcher = getPanelWidget<ILauncher>(context);
    quicklauncher = getPanelWidget<IQuickLauncher>(context);
    taskBar = getPanelWidget<ITaskBar>(context);
    sysTray = getPanelWidget<ISystemTray>(context);
    clock = getPanelWidget<IClock>(context);

    // Connect widgets
    if (launcher) {
        launcher->setObjectName("startButton");
        ui->launcherArea->layout()->addWidget(launcher);
    }
    if (quicklauncher)
        ui->quickLaunchArea->layout()->addWidget(quicklauncher);
    if (taskBar)
        ui->windowsArea->layout()->addWidget(taskBar);
    if (sysTray)
        ui->systrayArea->layout()->addWidget(sysTray);
    if (clock) {
        ui->clockArea->layout()->addWidget(clock);
        clock->show();
    }
}

Panel::~Panel() {
    delete ui;
}

void Panel::moveEvent(QMoveEvent * event) {
    reserveScreenArea(geometry());
}

void Panel::resizeEvent(QResizeEvent * event) {
    // Set panel geometry
    const QRect screen = QApplication::desktop()->screenGeometry();
    setGeometry(QRect(0, screen.height() - height(), screen.width(), height()));

    reserveScreenArea(geometry());
}

void Panel::reserveScreenArea(const QRect &area) {
    int mScreenNum = QApplication::desktop()->screenNumber(this);
    const QRect screen = QApplication::desktop()->screenGeometry();

    LxQt::XfitMan xf = LxQt::xfitMan();
    Window wid = effectiveWinId();

    xf.setStrut(wid, 0, 0, 0, screen.height() - area.y(),
            /* Left   */ 0, 0,
            /* Right  */ 0, 0,
            /* Top    */ 0, 0,
            /* Bottom */ area.left(), area.right()
            );
}

template<class Interface> inline QWidget * Panel::getPanelWidget(us::ModuleContext * context) {
    QWidget *component = NULL;

    us::ServiceReference<Interface> ref =
            context->GetServiceReference<Interface>();

    if (ref)
        component = dynamic_cast<QWidget*> (context->GetService(ref));

    if (!ref || !component)
        qDebug() << "Unable to find: " << us_service_interface_iid<Interface>();
    return component;
};
