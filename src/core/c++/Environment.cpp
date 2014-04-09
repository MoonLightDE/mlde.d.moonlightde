#include "Environment.h"

#include <cstdlib>

#include <usModule.h>
#include <usModuleContext.h>
#include <usGetModuleContext.h>
#include <usServiceReference.h>

#include <QDebug>

using namespace us;

Environment::Environment() {
    ModuleContext * context  = GetModuleContext();

    std::vector<ServiceReference<Core::ISettingsProfile> > refs =
            context->GetServiceReferences<Core::ISettingsProfile>("");
    if (refs.empty()) {
        qWarning() << "Unable to find the SettingsProfile service.";
    } else {
        Core::ISettingsProfile * settingsProfile = context->GetService(refs.front());
        settings = settingsProfile->getSettingsOf("Environment");
    }
}

QProcessEnvironment Environment::generateProcessEnvironment() {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QStringList keys = settings->allKeys();
    foreach (QString key, keys) {
        env.insert(key,settings->value(key).toString());
    }
    return env;
}

void Environment::insert(QString key, QString value) {
    setenv(key.toLocal8Bit().constData(),value.toLocal8Bit().constData(), 1);
    settings->setValue(key,value);
    settings->sync();
    emit ( environmentUpdated(key, value) );
}

void Environment::remove(QString key) {
    settings->remove(key);
    unsetenv(key.toLocal8Bit().constData());
    settings->sync();
    emit ( environmentUpdated(key, "") );
}

bool Environment::contains(QString key) {
    return settings->contains(key);
}

Environment::~Environment() {
    settings->sync();
    delete( settings );
}
