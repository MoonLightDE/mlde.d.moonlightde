/* 
 * File:   TestICore.h
 * Author: alexis
 *
 * Created on 8 de mayo de 2014, 11:46
 */

#ifndef TESTICORE_H
#define	TESTICORE_H

#include "core/ICore.h"

#include <QtTest>
#include <QObject>

class TestICore : public QObject {

    Q_OBJECT

public slots:
    void noticeLoadEnded() {
        qDebug() << "Signal recieved: Load process ended.";
    }

    void updateValue(QString arg1, QString arg2) {
        qDebug() << "Signal recieved: Environment update " << arg1 << ":" << arg2;
        values.insert(arg1, arg2);
    }
private slots:
    void ICore();
    void IModuleManager();
    void ModuleManager() ;

private:
    QHash<QString, QString> values;
};

#endif	/* TESTICORE_H */

