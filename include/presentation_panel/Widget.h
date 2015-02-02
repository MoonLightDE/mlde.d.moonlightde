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


#ifndef WIDGET_H
#define	WIDGET_H

#include <QWidget>

namespace presentation_panel {

    // Predefined Widget names
    static const char * MAINMENUBUTTON = "MainMenuButton";
    static const char * USERTASKS = "UserTasks";
    static const char * INDICATORS = "Indicators";
    static const char * DATETIME = "DateTime";
    
    class Widget : public QWidget {
    public:

        virtual ~Widget() {
        }

        /**
         * The widget provides a custumization UI ?
         * @return 
         */
        virtual bool isCustomizable() = 0;
        /**
         * Resolve the customization widget for the current PanelWidget.
         * @return 
         */
        virtual QWidget * custimizationWidget() = 0;

    };
}

US_DECLARE_SERVICE_INTERFACE(presentation_panel::Widget, "org.moonlightde.presentation.panel.Widget/1.0")

#endif	/* WIDGET_H */

