/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 * 
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

#ifndef MODULESETTINGS_H
#define	MODULESETTINGS_H

#include <usModuleContext.h>

#include <QSettings>
#include <QString>
#include <QPointer>

namespace Core {
    class Controller;
}

/*! Provides an standarized way to access to the module settings, global 
 * settings and module resources.
 */
class ModuleSettings {
public:
    /*! Provides an exclusive settings space for the module.
     * \param context current module context.
     * \retval QSettings* settings for the current module.
     * 
     * Usage:
     *  #include <usGetModuleContext>
     *  
     *  us::ModuleContext *context = getModuleContext();
     *  QSettings settings = getModuleSettings(context);
     *  
     *  ...
     * 
     *  delete settings;
     * 
     */
    static QSettings * getModuleSettings(us::ModuleContext * context);

    /*! Provides an settings space for the whole desktop environment. Use it
     * only for "global" settings, by example icon theme, environment variables,
     * etc.
     * \retval QSettings* settings for the current module.
     * 
     * Usage:
     *  QSettings settings = getGlobalSettings();
     *  
     *  ...
     * 
     *  delete settings;
     * 
     */
    static QSettings * getGlobalSettings();

    /*! Provides an exclusive space for the module resources in the user home.
     * Use it to store persistent data.
     * \param context current module context.
     * \retval QSettings* settings for the current module.
     * 
     * Usage:
     *  #include <usGetModuleContext>
     *  
     *  us::ModuleContext *context = getModuleContext();
     *  QSettings settings = getModuleDataLocation(context);
     *  
     *  ...
     * 
     *  delete settings;
     * 
     */
    static QString getModuleDataLocation(us::ModuleContext * context);
private:
    friend class Core::Controller;

    /*! Prepare the module settings paths by appending a profile name. This must
     * only be used by the Core::Controller.
     * \param profile profile name, usually recived as a cli parameter.
     * 
     */
    static void setProfileName(const QString &profile);

    ModuleSettings();
    virtual ~ModuleSettings();

    static QString m_profile;
    static QString m_settingsBasePath;
    static QString m_resourcesBasePath;
};

#endif	/* MODULESETTINGS_H */

