/* 
 * File:   TestICore.cpp
 * Author: alexis
 * 
 * Created on 8 de mayo de 2014, 11:46
 */

#include "TestICore.h"

#include <usModuleContext.h>
#include <usModuleActivator.h>
#include <usGetModuleContext.h>

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

    QVERIFY(moduleManager->listAviableModules().contains("moonlightDE-iddle_module"));
    moduleManager->load("moonlightDE-iddle_module");
    QVERIFY(moduleManager->listActiveModules().contains("moonlightDE-iddle_module"));
    moduleManager->unload("moonlightDE-iddle_module");
    QVERIFY(moduleManager->listActiveModules().contains("moonlightDE-iddle_module") == false);
}

void TestICore::IEnvironment() {
    ModuleContext * context = GetModuleContext();
    QVERIFY(context != NULL);

    ServiceReference<Core::IEnvironment> iEnvironmentRef =
            context->GetServiceReference<Core::IEnvironment>();

    Core::IEnvironment * iEnvironment = context->GetService(iEnvironmentRef);
    QVERIFY(iEnvironment != NULL);
    QObject::connect(iEnvironment, SIGNAL(environmentUpdated(QString, QString)),
            this, SLOT(updateValue(QString, QString)));

    iEnvironment->insert("TEST_VALUE", "VALUE");
    QCOMPARE(iEnvironment->generateProcessEnvironment().value("TEST_VALUE"), QString("VALUE"));
    QCOMPARE(values.value("TEST_VALUE"), QString("VALUE"));
    iEnvironment->remove("TEST_VALUE");
    QVERIFY(iEnvironment->generateProcessEnvironment().value("TEST_VALUE") == QString(""));
    QVERIFY(values.value("TEST_VALUE") == QString(""));
}

void TestICore::ISettingsProfile() {
    ModuleContext * context = GetModuleContext();
    QVERIFY(context != NULL);

    ServiceReference<Core::ISettingsProfile> iSettingsProfileRef =
            context->GetServiceReference<Core::ISettingsProfile>();

    Core::ISettingsProfile * settingsProfile = context->GetService(iSettingsProfileRef);
    QVERIFY(settingsProfile != NULL);
    QSettings *settings = settingsProfile->getSettingsOf("TestSettings");
    settings->setValue("Key", "Value");
    delete(settings);

    settings = settingsProfile->getSettingsOf("TestSettings");
    QCOMPARE(settings->value("Key").toString(), QString("Value"));
    delete(settings);
}