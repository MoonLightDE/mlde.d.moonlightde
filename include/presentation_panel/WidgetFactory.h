/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
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


#ifndef WIDGETFACTORY_H
#define	WIDGETFACTORY_H

#include <QWidget>
#include <usServiceInterface.h>

namespace presentation_panel {

    // List of well known panel widgets
    static const char * MAINMENUBUTTON = "MainMenuButton";
    static const char * USERTASKS = "UserTasks";
    static const char * INDICATORS = "Indicators";
    static const char * DATETIME = "DateTime";
    static const char * QUICKLAUNCHS = "QuickLaunchers";

    class WidgetFactory {
    public:

        virtual ~WidgetFactory() {
        }

        /**
         * The widget provides a custumization UI ?
         * @return 
         */
        virtual bool isCustomizable() {
            return false;
        }

        /**
         * Resolve the customization widget for the current Panel Widget.
         * @return 
         */
        virtual QWidget * custimizationWidget(const QString &prefix) {
            return NULL;
        }

        /**
         * @return Widget name.
         */
        virtual QString name() = 0;

        /**
         * Creates a new instace of the widget. The user must take care of it's
         * release when no longer needed or when the service provider is gone.
         * @param prefix provides a namespace for the widget settings.
         * @param parent 
         * @return 
         */
        virtual QWidget * build(const QString &prefix, QWidget *parent) = 0;
    };
};

US_DECLARE_SERVICE_INTERFACE(presentation_panel::WidgetFactory, "org.moonlightde.presentation.panel.WidgetFactory/1.0")

#endif	/* WIDGETFACTORY_H */

