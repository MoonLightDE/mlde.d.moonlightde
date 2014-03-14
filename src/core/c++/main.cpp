/*
 * Copyright (C) 2013 Moonlight Desktop Environment Team
 * Authors:
 * Alexis López Zubieta
 * This file is part of Moonlight Desktop Environment.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */


#include "ModuleManager.h"
#include "MoonLightDECoreConfig.h"
#include "CoreContext.h"

#include <QDebug>
#include <QStringList>
#include <QApplication>
#include <QCommandLineParser>



using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setApplicationName("MoonLightDE");
    app.setApplicationVersion("0.2.0.development1");

    app.setOrganizationName("MoonLight Desktop Environment");
    app.setOrganizationDomain("moonlightde.org");

    // Parse arguments
    QString profile;
    QString pluginsDir;
    bool config;
    { // Scope used to force varibles release 
        QCommandLineParser parser;
        parser.setApplicationDescription("A modular desktop environment for low performance devices.");
        parser.addHelpOption();
        parser.addVersionOption();


        QCommandLineOption configOption(QStringList() << "c" << "config", "Opens the configuration window before start.");
        parser.addOption(configOption);

        // Profile option
        QCommandLineOption profileOption(QStringList() << "p" << "profile",
                QCoreApplication::translate("profile", "Defines the profile to be used."),
                QCoreApplication::translate("profile", "profile"),
                "Default");
        parser.addOption(profileOption);

        // Plugins dir option
        QCommandLineOption pluginsdDirOption(QStringList() << "d" << "modulesDir",
                QCoreApplication::translate("Modules location", "Defines the location of the modules to be loaded."),
                QCoreApplication::translate("modulesDir", "modulesDir"),
                MODULES_OUTPUT_DIR);
        parser.addOption(pluginsdDirOption);

        // Process the actual command line arguments given by the user
        parser.process(app);

        const QStringList args = parser.positionalArguments();
        // source is args.at(0), destination is args.at(1)

        config = parser.isSet(configOption);
        profile = parser.value(profileOption);
        pluginsDir = parser.value(pluginsdDirOption);
    }
    if (0)
        qDebug() << "test";

    CoreContext::init(profile, pluginsDir);
    return app.exec();
}

