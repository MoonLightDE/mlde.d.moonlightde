/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
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

#ifndef NOTIFICATIONMEMO_H
#define	NOTIFICATIONMEMO_H


#include <QPixmap>
#include <QStringList>
#include <QVariantMap>

/* Description
 * Encapsulates the Notification data in order to be shared between the 
 * different views: floating box or staked into a side panel).
 * 
 * Sinals are used to notify the reason of destruction, they must be triggered
 * from outside.
 */
class NotificationMemo : public QObject {
    Q_OBJECT
public:
    NotificationMemo(const QString &application,
            const QString &summary, const QString &body,
            /*const QString &icon,*/ const QPixmap &pixmap, int timeout,
            const QStringList& actions, const QVariantMap& hints);

    virtual ~NotificationMemo();

    bool operator==(const NotificationMemo &orig) const;

    QStringList getActions() const {
        return m_actions;
    }

    QString getApplication() const {
        return m_application;
    }

    QString getBody() const {
        return m_body;
    }

    QVariantMap getHints() const {
        return m_hints;
    }

    QPixmap getPixmap() const {
        return m_pixmap;
    }

    //    QString getIcon() const {
    //        return m_icon;
    //    }

    QString getSummary() const {
        return m_summary;
    }

    int getTimeout() const {
        return m_timeout;
    }

    void setActions(QStringList actions) {
        m_actions = actions;
    }

    void setApplication(QString application) {
        m_application = application;
    }

    void setBody(QString body) {
        m_body = body;
    }

    void setHints(QVariantMap hints) {
        m_hints = hints;
    }

    void setPixmap(QPixmap pixmap) {
        m_pixmap = pixmap;
    }

    //    void setIcon(QString icon) {
    //        m_icon = icon;
    //        emit dataUpdated();
    //    }

    void setSummary(QString summary) {
        m_summary = summary;
    }

    void setTimeout(int timeout) {
        m_timeout = timeout;
    }

signals:
    /* Trigered when the timeout is fullfilled.
     */
    void expired();

    /* Trigered by the user.
     */
    void dismissed();

    /* Dropped by the system. By example whe the notification is replaced.
     */
    void droped();



private:
    QString m_application;
    QString m_summary;
    QString m_body;
    //    QString m_icon;
    QPixmap m_pixmap;
    int m_timeout;
    QStringList m_actions;
    QVariantMap m_hints;
};

#endif	/* NOTIFICATIONMEMO_H */

