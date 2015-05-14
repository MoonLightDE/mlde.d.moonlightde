/* 
 * File:   cli-main.cpp
 * Author: alexis
 *
 * Created on 9 de octubre de 2014, 11:15
 */

#include "RemoteClient.h"

#include <QApplication>
#include <QCommandLineParser>

#include <cstdlib>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("MoonLight Desktop Environment Command Line Interface.\n");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption actionOption(QStringList() << "a" << "Action",
            QCoreApplication::translate("action", "The general action to be performed."),
            QCoreApplication::translate("action", "action"),
            "");
    parser.addOption(actionOption);

    QCommandLineOption dataOption(QStringList() << "d" << "Data",
            QCoreApplication::translate("data", "The data to operate on, such as a file expressed as a Uri."),
            QCoreApplication::translate("data", "data"),
            "");
    parser.addOption(dataOption);

    QCommandLineOption componentOption(QStringList() << "c" << "Component",
            QCoreApplication::translate("c", "Specifies an explicit name of a component class to use for the intent."),
            QCoreApplication::translate("component", "component"),
            "");
    parser.addOption(componentOption);
    //    
    //    QCommandLineOption categoryOption(QStringList() << "a" << "Category",
    //            QCoreApplication::translate("a", "Gives additional information about the action to execute.."),
    //            QCoreApplication::translate("category", "category"),
    //            "");
    //    parser.addOption(categoryOption);


    // Process the actual command line arguments given by the user
    parser.process(app);

    QString action = parser.value(actionOption);
    QString data = parser.value(dataOption);
    QString component = parser.value(componentOption);
    //    QString category = parser.value(categoryOption);

    org::moonlightde::remote *interface =
            new org::moonlightde::remote("org.moonlightde.remote", "/Remote",
            QDBusConnection::sessionBus());
    
    if (component.isEmpty())
        interface->performIntent(action, data);
    else
        interface->performIntent(action, data, component, QString(), QVariantMap());

    return 0;

}

