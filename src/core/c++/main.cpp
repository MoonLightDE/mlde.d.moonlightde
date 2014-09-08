/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
 * 
 * This file is part of Moonlight Desktop Environment Core.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */


#include "MoonLightDECoreConfig.h"
#include "ModuleManager.h"
#include "Controller.h"
#include "core/ICore.h"

#include <LXQt/Application>

#include <QDebug>
#include <QStringList>
#include <QApplication>
#include <QCommandLineParser>
#include <QHash>


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    LxQt::Application app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    app.setApplicationName(PROJEC_NAME);
    app.setApplicationVersion(PROJEC_VERSION);

    app.setOrganizationName("MoonLight Desktop Environment");
    app.setOrganizationDomain("org.moonlightde");

    // Parse arguments
    QHash<QString, QVariant> config;
    //    TODO: Add a an option to open configuration dialog before start.
    //    bool config;
    { // Scope used to force varibles release 
        QCommandLineParser parser;
        parser.setApplicationDescription("Core of the MoonLight Desktop Environment.\n");
        parser.addHelpOption();
        parser.addVersionOption();

        // Show Config option
        QCommandLineOption configOption(QStringList() << "c" << "configuration",
                QCoreApplication::translate("configuration", "Show the configuration UI."));
        parser.addOption(configOption);
        
        // Profile option
        QCommandLineOption profileOption(QStringList() << "p" << "profile",
                QCoreApplication::translate("profile", "Defines the profile to be used."),
                QCoreApplication::translate("profile", "profile"),
                "Default");
        parser.addOption(profileOption);

        // Modules diretory path option
        QCommandLineOption modulesPathOption(QStringList() << "m" << "modules-path",
                QCoreApplication::translate("Modules location", "Defines the location of the modules to be loaded."),
                QCoreApplication::translate("modules-path", "modules-path"),
                ".");
        parser.addOption(modulesPathOption);

        // Modules descriptors diretory path option
        QCommandLineOption modulesDescriptorsPathOption(QStringList() << "d" << "descriptors-path",
                QCoreApplication::translate("Descriptors location", "Defines the location of the modules descriptors."),
                QCoreApplication::translate("descriptots-path", "descriptots-path"),
                ".");
        parser.addOption(modulesDescriptorsPathOption);

        // Process the actual command line arguments given by the user
        parser.process(app);


        config["showConfig"] = parser.isSet(configOption);
        config["profile"] = parser.value(profileOption);
        config["modulesPath"] = parser.value(modulesPathOption);
        config["descriptorsPath"] = parser.value(modulesDescriptorsPathOption);
    }

    Core::IController * core = new Controller(config);
    core->start();
    return app.exec();
}

