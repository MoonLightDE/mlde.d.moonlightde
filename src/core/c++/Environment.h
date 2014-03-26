#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "core/ICore.h"

#include <QObject>
#include <QSettings>

class Environment : public CoreContext::IEnvironment {
    Q_OBJECT
public:
    explicit Environment();

    QProcessEnvironment generateProcessEnvironment();
    void insert(QString key, QString value);
    void remove(QString key);
    bool contains(QString key);

    virtual ~Environment();
signals:
    void environmentUpdated(QString key, QString value);


private:
    QSettings * settings;

};

#endif // ENVIRONMENT_H
