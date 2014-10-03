/* 
 * File:   ModuleSettings.cpp
 * Author: alexis
 * 
 * Created on 2 de octubre de 2014, 23:11
 */

#include "core/ModuleSettings.h"

#include <usModule.h>

#include <QStandardPaths>
#include <QApplication>
#include <QDir>
#include <QDebug>

QString ModuleSettings::m_profile;
QString ModuleSettings::m_settingsBasePath;
QString ModuleSettings::m_resourcesBasePath;

ModuleSettings::ModuleSettings() {
}

ModuleSettings::~ModuleSettings() {
}

QSettings * ModuleSettings::getModuleSettings(us::ModuleContext * context) {
    if (m_profile.isEmpty())
        setProfileName("default");

    QSettings * settings;
    QString moduleName = QString::fromStdString(context->GetModule()->GetName());
    settings = new QSettings(m_settingsBasePath + QDir::separator() + moduleName + QDir::separator() + "settings.conf", QSettings::NativeFormat);
    
    return settings;
}

QSettings * ModuleSettings::getGlobalSettings() {
    if (m_profile.isEmpty())
        setProfileName("default");

    QSettings * settings;
    settings = new QSettings(m_settingsBasePath + QDir::separator() + "global.conf", QSettings::NativeFormat);
    
    return settings;
}

QString ModuleSettings::getModuleDataLocation(us::ModuleContext * context) {
    if (m_profile.isEmpty())
        setProfileName("default");

    QString moduleName = QString::fromStdString(context->GetModule()->GetName());
    QString moduleDataLocation = m_resourcesBasePath + QDir::separator() + moduleName;

    // Ensure that the path exists
    QDir().mkpath(moduleDataLocation);

    return moduleDataLocation;
}

void ModuleSettings::setProfileName(const QString &profile) {
    if (m_profile.isNull()) {
        m_profile = profile;

        // Generate base paths
        m_settingsBasePath.clear();
        m_settingsBasePath += QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        m_settingsBasePath += QDir::separator() + QApplication::organizationName();
        m_settingsBasePath += QDir::separator() + m_profile;

        m_resourcesBasePath.clear();
        m_resourcesBasePath += QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
        m_resourcesBasePath += QDir::separator() + QApplication::organizationName();
    } else {
        qDebug() << "Attempting to change settings profile fail, it is already set.";
    }
}
