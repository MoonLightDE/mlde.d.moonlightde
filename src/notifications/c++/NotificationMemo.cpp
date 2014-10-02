/* 
 * File:   NotificationMeno.cpp
 * Author: alexis
 * 
 * Created on 19 de septiembre de 2014, 10:38
 */

#include "NotificationMemo.h"

#include <QTimer>

NotificationMemo::NotificationMemo(const QString &application,
        const QString &summary, const QString &body,
        /*const QString &icon,*/ const QPixmap &pixmap, int timeout,
        const QStringList& actions, const QVariantMap& hints) : QObject(),
m_application(application), m_summary(summary), m_body(body), /*m_icon(icon), */
m_pixmap(pixmap), m_timeout(timeout), m_actions(actions), m_hints(hints) {

}

NotificationMemo::~NotificationMemo() {
}

bool NotificationMemo::operator==(const NotificationMemo &other) const {
    if ((other.m_summary == m_summary) &&
            (other.m_body == m_summary) &&
            (other.m_application == m_application))
        return true;
    else return false;
}