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


#ifndef ISIDEPANEL_H
#define	ISIDEPANEL_H

#include <usServiceInterface.h>

#include <QWidget>

struct ISidePanel {
    /* Description:
     * Displays the side panel with the wigdet inside. If autohide is enabled
     * the panel will act as a menu and hide on focus lost event, else the 
     * widget must hide its parent.
     * 
     * The widget is deleted when the panel hides. If you have to swicht between
     * multiple widgets without closing then, consider using a QStackedWidget.
     * 
     * Inputs: 
     *  widget - widget to be shown in the side panel.
     *  autohide - enable to make the panel hide on focus lost.
     */
    virtual void showWidget(QWidget *widget, const bool autohide = true) = 0;

    virtual ~ISidePanel() {
    };
};


US_DECLARE_SERVICE_INTERFACE(ISidePanel, "org.moonlightde.ISidePanel/1.0")

#endif	/* ISIDEPANEL_H */

