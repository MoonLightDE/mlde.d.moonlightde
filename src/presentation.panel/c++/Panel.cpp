/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 *      Jorge Fernández Sánchez
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
#include <QDesktopWidget>
#include <QPointer>


#include <LXQt/XfitMan>
#include <X11/Xatom.h>


using namespace us;

Panel::Panel(QWidget *parent) :
QFrame(parent), ui(new Ui::Panel) {
    ui->setupUi(this);

    mHeight = 36;
    adjustSizeToScreen();
    setupWindowFlags();
    visibleDash = false;

    // Services lookup
    us::ModuleContext * context = us::GetModuleContext();

    // Create a service tracker to monitor the launcher.
    m_launcherTracker = new ServiceTracker<ILauncherFactory>(
            context, LDAPFilter(std::string("(&(") + ServiceConstants::OBJECTCLASS() + "=" +
            us_service_interface_iid<ILauncherFactory>() + "))")
            );
    m_launcherTracker->Open();

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));

    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(adjustSizeToScreen()));
}

Panel::~Panel() {
    delete ui;
}

bool Panel::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::Show:
            requestExclusiveScreenArea();
            break;
        case QEvent::ContextMenu:
            break;

        case QEvent::LayoutRequest:
            qDebug() << "presentation_panel: desktop layout changed.";
            adjustSizeToScreen();
            break;

        case QEvent::WinIdChange:
            qDebug() << "presentation_panel: Wid changed.";
            setupWindowFlags();
            adjustSizeToScreen();
        default:
            break;
    }
    return QFrame::event(event);
}

void Panel::adjustSizeToScreen() {
    // Panel widgth must match the screen widgth
    const QRect currentScreenGeometry = QApplication::desktop()->screenGeometry(this);
    QRect rect;

    rect.setHeight(mHeight);
    rect.setWidth(currentScreenGeometry.width());

    rect.moveBottom(currentScreenGeometry.bottom());


    qDebug() << "presentation_panel: Adjusting workare size to: " << rect;
    if (rect != geometry()) {
        setGeometry(rect);
        setFixedSize(rect.size());

        // Update reserved screen area on resize
        requestExclusiveScreenArea();
    }
}

inline void Panel::setupWindowFlags() {
    KWindowSystem::setOnAllDesktops(effectiveWinId(), true);
    KWindowSystem::setType(effectiveWinId(), NET::Dock);
}

void Panel::requestExclusiveScreenArea() {
    Window wid = effectiveWinId();
    if (wid == 0 || !isVisible())
        return;

    LxQt::XfitMan xf = LxQt::xfitMan();
    const QRect wholeScreen = QApplication::desktop()->geometry();
    // qDebug() << "wholeScreen" << wholeScreen;
    const QRect rect = geometry();
    // NOTE: http://standards.freedesktop.org/wm-spec/wm-spec-latest.html
    // Quote from the EWMH spec: " Note that the strut is relative to the screen edge, and not the edge of the xinerama monitor."
    // So, we use the geometry of the whole screen to calculate the strut rather than using the geometry of individual monitors.
    // Though the spec only mention Xinerama and did not mention XRandR, the rule should still be applied.
    // At least openbox is implemented like this.

    xf.setStrut(wid, 0, 0, 0, wholeScreen.bottom() - rect.y(),
            /* Left   */ 0, 0,
            /* Right  */ 0, 0,
            /* Top    */ 0, 0,
            /* Bottom */ rect.left(), rect.right()
            );

}

void Panel::startButtonClicked() {
    ILauncherFactory * factory = m_launcherTracker->GetService();
    if (factory == NULL)
        qDebug() << "presentation_panel: There aren't available launchers.";
    else {
        QPointer<QWidget> launcher = factory->getLauncher(XdgMenu::getMenuFileName());
        if (launcher.isNull())
            return;

        if (launcher->isVisible()) {
            launcher->hide();
        } else {
            if (qobject_cast<QMenu *>(launcher)) {
                QPoint position = pos();
                position.ry() -= (launcher->sizeHint().height());
                //qDebug() << " menu pos: " << position;
                qobject_cast<QMenu *>(launcher)->popup(position);
            } else { //Dash
                if(!visibleDash)
                {
                    QRect rect = qApp->desktop()->geometry();
                    
                    launcher->move(QPoint(0, 0));
                    launcher->resize(rect.width(), rect.height() - size().height());
                    launcher->show();
                    visibleDash = true;
                }
                else
                {
                    launcher->hide();
                    visibleDash = false;
                }
            }
        }
    }
}