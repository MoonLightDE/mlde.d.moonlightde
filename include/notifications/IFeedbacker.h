/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Alec Moskvin <alecm@gmx.com>
 *   Petr Vanek <petr@scribus.info>
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

#ifndef IFEEDBACKER_H
#define IFEEDBACKER_H

#include <usServiceInterface.h>

#include <QObject>
#include <QWidget>

/*
 * Class for interface org.freedesktop.Notifications
 */
class IFeedbacker : public QObject {
public:
    virtual ~IFeedbacker() {
    }

public slots:
    /*! External application requested to close one notification
     * \param id an id obtained by previous \c Notify() call
     */
    virtual void CloseNotification(uint id) = 0;

    /*! Daemon is requested to display the notification.
     * \retval uint an unique notification ID. It should be used for tracking action feedback.
     * \param app_name name of the calling application
     * \param replaces_id if it's provided (!= 0) the already posted notification is "replaced"
     *                    or updated with new content.
     * \param app_icon an icon to show. See specification for more info.
     * \param summary an overview text
     * \param body a notification body
     * \param actions User selectable action list. See specification for more info.
     * \param hints Notification hints. See specification for more info.
     * \param expire_timeout how long should be this notification displayed (-1 = server decides; 0 forever; >0timeout in milliseconds)
     */
    virtual void notify(uint id, const QString &application,
            const QString &summary, const QString &body,
            const QString &icon, int timeout,
            const QStringList& actions, const QVariantMap& hints) = 0;

    /*! Put the given witget in the notifications area. For use with copy, move
     * compressing and other operations.
     * \retval uint an unique notification ID. It should be used for tracking action feedback.
     * \param taskCtrl a widget to control a given task must fith the side panel
     * size.
     */
    virtual uint registerTask(const QWidget *taskCtrl) = 0;

};

Q_DECLARE_INTERFACE(IFeedbacker, "org.moonlightde.notifications.IFeedbacker/1.0")
US_DECLARE_SERVICE_INTERFACE(IFeedbacker, "org.moonlightde.notifications.IFeedbacker/1.0")
#endif // IFEEDBACKER_H
