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

#include <QDebug>
#include <qt5/QtWidgets/qdesktopwidget.h>

#include <lxqt/lxqtxfitman.h>

Panel::Panel(QWidget *parent) :
QWidget(parent), ui(new Ui::Panel) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    setAttribute(Qt::WA_AlwaysShowToolTips);
    setWindowTitle("MoonLightDE Panel");

    // Set panel at the button of the screen
    const QRect screen = QApplication::desktop()->screenGeometry();
    setGeometry(QRect(0, screen.height() - height(), screen.width(), height()));

    // Services lookup
    us::ModuleContext * context = us::GetModuleContext();
    launcher = getPanelWidget<ILauncher>(context);
    quicklauncher = getPanelWidget<IQuickLauncher>(context);
    taskBar = getPanelWidget<ITaskBar>(context);
    systemTry = getPanelWidget<ISystemTry>(context);
    clock = getPanelWidget<IClock>(context);

    // Connect widgets
    if (launcher)
        connect(ui->startButton, SIGNAL(ui->startButton->released()), launcher, SLOT(launcher->show()));
    if (quicklauncher)
        ui->quickBar = quicklauncher;
    if (taskBar)
        ui->windowsBar = taskBar;
    if (systemTry)
        ui->systrayBar = systemTry;
    if (clock) {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(clock);
        ui->clockBar->setLayout(layout);
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
    reserveScreenArea(geometry());
}

void Panel::reserveScreenArea(const QRect &area) {
    qDebug() << "Reserving screen area " << area;
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
