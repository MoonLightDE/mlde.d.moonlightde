/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
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

/********************************************************************
  Inspired by freedesktops tint2 ;)

 *********************************************************************/


// Warning: order of those include is important.
#include "lxqttray.h"
#include "trayicon.h"

#include <LXQt/lxqtgridlayout.h>
#include <LXQt/lxqtxfitman.h>


#include <QtDebug>
#include <QX11Info>
#include <QApplication>
#include <QtCore/QTimer>

#include <xcb/xcb_aux.h>
#include <xcb/xcb_atom.h>


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xdamage.h>
#include "c++/fixx11h.h"

#define _NET_SYSTEM_TRAY_ORIENTATION_HORZ 0
#define _NET_SYSTEM_TRAY_ORIENTATION_VERT 1

#define SYSTEM_TRAY_REQUEST_DOCK    0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2

#define XEMBED_EMBEDDED_NOTIFY  0
#define XEMBED_MAPPED          (1 << 0)

using namespace LxQt;

/************************************************

 ************************************************/
LxQtTray::LxQtTray(QWidget *parent) :
QFrame(parent),
mValid(false),
mTrayId(0),
mDamageEvent(0),
mDamageError(0),
mIconSize(TRAY_ICON_SIZE_DEFAULT, TRAY_ICON_SIZE_DEFAULT) {
    mLayout = new LxQt::GridLayout(this);
    realign();

    // Install event filter
    qApp->installNativeEventFilter(this);

    // Init the selection later just to ensure that no signals are sent until
    // after construction is done and the creating object has a chance to connect.
    //QTimer::singleShot(1000, this, SLOT(startTray()));
    QTimer::singleShot(0, this, SLOT(init()));
}

bool LxQtTray::nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE {
    if (eventType == "xcb_generic_event_t") {

        xcb_generic_event_t* ev = static_cast<xcb_generic_event_t *> (message);


        TrayIcon* icon;
        switch (ev->response_type & ~0x80) {
            case XCB_CLIENT_MESSAGE:
                xcb_client_message_event_t * client_message_ev;
                client_message_ev = (xcb_client_message_event_t *) ev;
                processClientMessage(client_message_ev);
                break;

                //        case ConfigureNotify:
                //            icon = findIcon(event->xconfigure.window);
                //            if (icon)
                //                icon->configureEvent(&(event->xconfigure));
                //            break;

            case XCB_DESTROY_NOTIFY:
                xcb_destroy_notify_event_t * destroy_notify_ev;
                destroy_notify_ev = (xcb_destroy_notify_event_t *) ev;
                icon = findIcon(destroy_notify_ev->window);
                if (icon) {
                    mIcons.removeAll(icon);
                    delete icon;
                }
                break;

            default:
                if ((ev->response_type & ~0x80) == mDamageEvent + XDamageNotify) {
                    XDamageNotifyEvent* dmg = reinterpret_cast<XDamageNotifyEvent*> (ev);
                    icon = findIcon(dmg->drawable);
                    if (icon)
                        icon->update();
                }
                break;
        }

        /*
                switch (ev->response_type) {
                    case XCB_CONFIGURE_NOTIFY:
                    {
                        xcb_client_message_event_t
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
                }*/
        return false;
    }
}

/************************************************

 ************************************************/
LxQtTray::~LxQtTray() {
    stopTray();
    cleanup();
}

/************************************************

// ************************************************/
//void LxQtTray::x11EventFilter(XEvent * event) {
//    TrayIcon* icon;
//
//    switch (event->type) {
//        case ClientMessage:
//            clientMessageEvent(&(event->xclient));
//            break;
//
//            //        case ConfigureNotify:
//            //            icon = findIcon(event->xconfigure.window);
//            //            if (icon)
//            //                icon->configureEvent(&(event->xconfigure));
//            //            break;
//
//        case DestroyNotify:
//            icon = findIcon(event->xany.window);
//            if (icon) {
//                mIcons.removeAll(icon);
//                delete icon;
//            }
//            break;
//
//        default:
//            if (event->type == mDamageEvent + XDamageNotify) {
//                XDamageNotifyEvent* dmg = reinterpret_cast<XDamageNotifyEvent*> (event);
//                icon = findIcon(dmg->drawable);
//                if (icon)
//                    icon->update();
//            }
//            break;
//    }
//}

/************************************************

 ************************************************/
void LxQtTray::realign() {
    mLayout->setEnabled(false);
    /*    ILxQtPanel *panel = mPlugin->panel();

        if (panel->isHorizontal())
        {
            mLayout->setRowCount(panel->lineCount());
            mLayout->setColumnCount(0);
        }
        else
        {
            mLayout->setColumnCount(panel->lineCount());
            mLayout->setRowCount(0);
        }*/
    mLayout->setEnabled(true);
}

/** Handle systray message.
 * \param ev The event.
 * \return 0 on no error.
 */
void LxQtTray::processClientMessage(xcb_client_message_event_t * ev) {
    xcb_connection_t * connection = QX11Info::connection();
    int phys_screen = QX11Info::appScreen();

    int screen_nbr = 0, ret = 0;
    xcb_get_geometry_cookie_t geom_c;
    xcb_get_geometry_reply_t *geom_r;
    xcb_screen_iterator_t iter;

    switch (ev->data.data32[1]) {
        case SYSTEM_TRAY_REQUEST_DOCK:
            geom_c = xcb_get_geometry_unchecked(connection, ev->window);

            if (!(geom_r = xcb_get_geometry_reply(connection, geom_c, NULL)))
                return;

            for (iter = xcb_setup_roots_iterator(xcb_get_setup(connection)), screen_nbr = 0;
                    iter.rem && iter.data->root != geom_r->root; xcb_screen_next(&iter), ++screen_nbr);

            delete(geom_r);

            //ret = systray_request_handle(ev->data.data32[2], screen_nbr, NULL);
            addIcon(ev->data.data32[2]);

            break;
    }


    //    unsigned long opcode;
    //    opcode = clinet_message_ev->data.data32[1];
    //    Window id;
    //
    //    switch (opcode) {
    //        case SYSTEM_TRAY_REQUEST_DOCK:
    //            id = clinet_message_ev->data.data32[2];
    //            if (id)
    //                addIcon(id);
    //            break;
    //
    //
    //        case SYSTEM_TRAY_BEGIN_MESSAGE:
    //        case SYSTEM_TRAY_CANCEL_MESSAGE:
    //            qDebug() << "we don't show balloon messages.";
    //            break;
    //
    //
    //        default:
    //            if (opcode == xfitMan().atom("_NET_SYSTEM_TRAY_MESSAGE_DATA"))
    //                qDebug() << "message from dockapp:" << clinet_message_ev->data.data8;
    //            else
    //                qDebug() << "SYSTEM_TRAY : unknown message type" << opcode;
    //            break;
    //    }
}

/************************************************

 ************************************************/
TrayIcon * LxQtTray::findIcon(Window id) {

    foreach(TrayIcon* icon, mIcons) {
        if (icon->iconId() == id || icon->windowId() == id)
            return icon;
    }
    return 0;
}

/************************************************

 ************************************************/
void LxQtTray::setIconSize(QSize iconSize) {
    mIconSize = iconSize;
    foreach(TrayIcon* icon, mIcons)
    icon->setIconSize(mIconSize);
}

/************************************************

 ************************************************/
VisualID LxQtTray::getVisual() {
    VisualID visualId = 0;
    Display* dsp = QX11Info::display();

    XVisualInfo templ;
    templ.screen = QX11Info::appScreen();
    templ.depth = 32;
    templ.c_class = TrueColor;

    int nvi;
    XVisualInfo* xvi = XGetVisualInfo(dsp, VisualScreenMask | VisualDepthMask | VisualClassMask, &templ, &nvi);

    if (xvi) {
        int i;
        XRenderPictFormat* format;
        for (i = 0; i < nvi; i++) {
            format = XRenderFindVisualFormat(dsp, xvi[i].visual);
            if (format &&
                    format->type == PictTypeDirect &&
                    format->direct.alphaMask) {
                visualId = xvi[i].visualid;
                break;
            }
        }
        XFree(xvi);
    }

    return visualId;
}

/************************************************

 ************************************************/
void LxQtTray::stopTray() {
    qDeleteAll(mIcons);
    if (mTrayId) {
        XDestroyWindow(QX11Info::display(), mTrayId);
        mTrayId = 0;
    }
    mValid = false;
}

/** Initialize systray information in X.
 * \param phys_screen Physical screen.
 */
void LxQtTray::init() {
    xcb_connection_t * connection = QX11Info::connection();
    int phys_screen = QX11Info::appScreen();

    xcb_client_message_event_t ev;
    //xcb_screen_t *xscreen = xutil_screen_get(connection, phys_screen);

    if (xcb_connection_has_error(connection))
        qFatal("X connection invalid");

    xcb_screen_t *xscreen = xcb_aux_get_screen(connection, phys_screen);

    char *atom_name;
    xcb_intern_atom_cookie_t atom_systray_q;
    xcb_intern_atom_reply_t *atom_systray_r;
    xcb_atom_t atom_systray;

    /* Send requests */
    if (!(atom_name = xcb_atom_name_by_screen("_NET_SYSTEM_TRAY", phys_screen))) {
        qWarning("Error getting systray atom");
        return;
    }

    atom_systray_q = xcb_intern_atom_unchecked(connection, false,
            strlen(atom_name), atom_name);

    delete(atom_name);

    //globalconf.screens.tab[phys_screen].systray.window = xcb_generate_id(connection);
    mTrayId = xcb_generate_id(connection);
    xcb_create_window(connection, xscreen->root_depth,
            mTrayId,
            xscreen->root,
            -1, -1, 1, 1, 0,
            XCB_COPY_FROM_PARENT, xscreen->root_visual, 0, NULL);

    /* Fill event */
    //p_clear(&ev, 1);
    memset(&ev, 0, sizeof (ev));
    ev.response_type = XCB_CLIENT_MESSAGE;
    ev.window = xscreen->root;
    ev.format = 32;
    //ev.type = MANAGER;
    ev.type = xfitMan().atom("MANAGER");
    ev.data.data32[0] = XCB_CURRENT_TIME;
    ev.data.data32[2] = mTrayId;
    ev.data.data32[3] = ev.data.data32[4] = 0;

    if (!(atom_systray_r = xcb_intern_atom_reply(connection, atom_systray_q, NULL))) {
        qWarning("Error getting systray atom");
        return;
    }

    ev.data.data32[1] = atom_systray = atom_systray_r->atom;

    delete(atom_systray_r);

    xcb_set_selection_owner(connection,
            mTrayId,
            atom_systray,
            XCB_CURRENT_TIME);

    xcb_send_event(connection, false, xscreen->root, 0xFFFFFF, (char *) &ev);
}

/** Remove systray information in X.
 * \param phys_screen Physical screen.
 */
void LxQtTray::cleanup() {
    xcb_connection_t * connection = QX11Info::connection();
    int phys_screen = QX11Info::appScreen();

    xcb_intern_atom_reply_t *atom_systray_r;
    char *atom_name;

    if (!(atom_name = xcb_atom_name_by_screen("_NET_SYSTEM_TRAY", phys_screen))
            || !(atom_systray_r = xcb_intern_atom_reply(connection,
            xcb_intern_atom_unchecked(connection,
            false,
            strlen(atom_name),
            atom_name),
            NULL))) {
        qWarning("Error getting systray atom");
        delete(atom_name);
        return;
    }

    delete(atom_name);

    xcb_set_selection_owner(connection,
            XCB_NONE,
            atom_systray_r->atom,
            XCB_CURRENT_TIME);

    delete(atom_systray_r);
}

/************************************************

 ************************************************/
void LxQtTray::addIcon(Window winId) {
    TrayIcon* icon = new TrayIcon(winId, this);
    if (!icon->isValid()) {
        delete icon;
        return;
    }

    
    mIcons.append(icon);
    mLayout->addWidget(icon);
    icon->setIconSize(mIconSize);
}

