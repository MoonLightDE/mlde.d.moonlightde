/* 
 * File:   ModuleSettings.h
 * Author: alexis
 *
 * Created on 2 de octubre de 2014, 23:11
 */

#ifndef MODULESETTINGS_H
#define	MODULESETTINGS_H

#include <usModuleContext.h>

#include <QSettings>
#include <QString>
#include <QPointer>

namespace Core {
    class Controller;
}

class ModuleSettings {
public:
    static QSettings * getModuleSettings(us::ModuleContext * context);
    static QSettings * getGlobalSettings();

    static QString getModuleDataLocation(us::ModuleContext * context);
private:
    friend class Core::Controller;

    static void setProfileName(const QString &profile);

    ModuleSettings();
    virtual ~ModuleSettings();

    static QString m_profile;
    static QString m_settingsBasePath;
    static QString m_resourcesBasePath;
};

#endif	/* MODULESETTINGS_H */

