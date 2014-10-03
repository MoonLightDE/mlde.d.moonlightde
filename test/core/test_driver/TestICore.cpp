/* 
 * File:   TestICore.cpp
 * Author: alexis
 * 
 * Created on 8 de mayo de 2014, 11:46
 */

#include "TestICore.h"
#include "core/ModuleSettings.h"

#include <usModuleContext.h>
#include <usModuleActivator.h>
#include <usGetModuleContext.h>

#include <QSettings>
#include <QDir>
#include <QFile>

#include <QString>

US_USE_NAMESPACE

void TestICore::ICore() {
    ModuleContext * context = GetModuleContext();
    QVERIFY(context != NULL);

    ServiceReference<Core::IController> iCoreRef = context->GetServiceReference<Core::IController>();
    Core::IController * iCore = context->GetService(iCoreRef);
    QVERIFY(iCore != NULL);
    QObject::connect(iCore, SIGNAL(started()), this, SLOT(noticeLoadEnded()));
}

void TestICore::IModuleManager() {
    ModuleContext * context = GetModuleContext();
    QVERIFY(context != NULL);

    ServiceReference<Core::IModuleManager> iModuleManagerRef =
            context->GetServiceReference<Core::IModuleManager>();

    Core::IModuleManager * moduleManager = context->GetService(iModuleManagerRef);
    QVERIFY(moduleManager != NULL);


    QVERIFY(moduleManager->getAviableModules().contains("moonlightDE-test_module"));
    moduleManager->load("moonlightDE-test_module");

    QVERIFY(moduleManager->getActiveModules().contains("moonlightDE-test_module"));
    moduleManager->unload("moonlightDE-test_module");

    QVERIFY(moduleManager->getActiveModules().contains("moonlightDE-test_module") == false);

    XdgDesktopFile * desc = moduleManager->getModuleDescriptor("moonlightDE-test_driver");
    QVERIFY(desc != NULL);
    if (desc) {
        qDebug() << "Descriptor Name: " << desc->name();
        qDebug() << "Descriptor Comment: " << desc->comment();
    }
}

void TestICore::ModuleManager() {
    ModuleContext * context = GetModuleContext();
    QSettings * settings = ModuleSettings::getModuleSettings(context);
    QVERIFY(settings != NULL);

    settings->setValue("TEST_KEY", "TEST_VALUE");
    delete settings;
    
    settings = ModuleSettings::getModuleSettings(context);
    QString value = settings->value("TEST_KEY", "").toString();
    QVERIFY(value == "TEST_VALUE");
    delete settings;
    
    QSettings * settingsGlobal = ModuleSettings::getGlobalSettings();
    QVERIFY(settingsGlobal != NULL);

    settingsGlobal->setValue("TEST_KEY", "TEST_VALUE");
    delete settingsGlobal;
    
    settingsGlobal = ModuleSettings::getGlobalSettings();
    settingsGlobal->value("TEST_KEY", "").toString();
    QVERIFY(value == "TEST_VALUE");
    delete settingsGlobal;
    
    QString resDir = ModuleSettings::getModuleDataLocation(context);
    QDir dir (resDir);
    QVERIFY(dir.exists());
}