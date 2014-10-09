/* 
 * File:   Remote.cpp
 * Author: alexis
 * 
 * Created on 9 de octubre de 2014, 10:22
 */


#include "Remote.h"
#include "remoteadaptor.h"

#include "core/Intent.h"
#include "core/sendIntent.h"

#include <QVariant>
#include <QVariantMap>

using namespace Core;

Remote::Remote(QObject * parent) : QObject(parent) {
    adaptor = new RemoteAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();

    dbus.registerObject("/Remote", this);
    dbus.registerService("org.moonlightde.remote");
}

Remote::Remote(const Remote& orig) {
}

Remote::~Remote() {
    delete adaptor.data();
}

void Remote::performIntent(QString action, QString data) {
    Intent intent(action, data);
    sendIntent(intent);
}

void Remote::performIntent(QString action, QString data, QString component, QString category, QVariantMap extras) {
    Intent intent(action, data);
    intent.SetComponent(component);
    intent.SetCategory(category);

    foreach(QString key, extras.keys()) {
        intent.addExtra(QPair<QString,QString>(key, extras.value(key).toString()));
    }
    sendIntent(intent);
}