/* 
 * File:   Remote.h
 * Author: alexis
 *
 * Created on 9 de octubre de 2014, 10:22
 */

#ifndef REMOTE_H
#define	REMOTE_H
#include <QObject>
#include <QPointer>
#include <QVariantMap>

class RemoteAdaptor;

class Remote : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.moonlightde.remote")
public:
    Remote(QObject * parent = 0);
    virtual ~Remote();

public slots:
    void performIntent(QString action, QString data);
    void performIntent(QString action, QString data, QString component, QString category, QVariantMap extras);
private:
    Remote(const Remote& orig);
    QPointer<RemoteAdaptor> adaptor;
};

#endif	/* REMOTE_H */

