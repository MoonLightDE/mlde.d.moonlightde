/* 
 * File:   ICore.h
 * Author: alexis
 *
 * Created on 23 de marzo de 2014, 22:58
 */

#ifndef ICORE_H
#define	ICORE_H
#include <usServiceInterface.h>

#include <QObject>
#include <QString>
#include <QSettings>
#include <QProcessEnvironment>

namespace CoreContext {

    class ICore : public QObject {
    public:
        virtual void start() = 0;
        virtual void finish() = 0;

    signals:
        void started();
    };

    class IEnvironment : public QObject {
    public:
        virtual QProcessEnvironment generateProcessEnvironment() = 0;
        virtual void insert(QString key, QString value) = 0;
        virtual void remove(QString key) = 0;
        virtual bool contains(QString key) = 0;

    signals:
        virtual void environmentUpdated(QString key, QString value) = 0;

    };
    class IModuleManager {
    public:
        virtual bool load(const QString &name) = 0;
        virtual bool unload(const QString &name) = 0;
        virtual void loadFromProfile(QSettings * profile) = 0;
        virtual QList<QString> listAviableModules() = 0;
        virtual QList<QString> listActiveModules() = 0;

    };

    /*Public interface for the "SettingsProfile" class.*/
    class ISettingsProfile {
    public:
        virtual QSettings * getSettingsOf (QObject * object = 0) = 0;
        virtual QSettings * getSettingsOf (const QString & objectPath) = 0;

    };


}
US_DECLARE_SERVICE_INTERFACE(CoreContext::ICore, "org.moonlightde.core.ICore/1.0")
US_DECLARE_SERVICE_INTERFACE(CoreContext::IEnvironment, "org.moonlightde.core.IEnvironment/1.0")
US_DECLARE_SERVICE_INTERFACE(CoreContext::IModuleManager, "org.moonlightde.core.IModuleManager/1.0")
US_DECLARE_SERVICE_INTERFACE(CoreContext::ISettingsProfile, "org.moonlightde.core.ISettingsProfile/1.0")

#endif	/* ICORE_H */

