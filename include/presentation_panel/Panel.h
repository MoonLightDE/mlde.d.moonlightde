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

#ifndef PANEL_H
#define	PANEL_H

#include <QWidget>
#include <QString>
#include <usServiceInterface.h>

namespace presentation_panel {

    class WidgetFactory;

    class Panel {
    public:

        virtual ~Panel() {
        };
        virtual void addWidgetFactory(WidgetFactory * widgetFactory) = 0;
        virtual void removeWidgetFactory(WidgetFactory * widgetFactory) = 0;
        virtual int desktop() = 0;
        virtual void setDesktop(int desktop) = 0;
        virtual QRect geometry() = 0;
        virtual void setGeometry(QRect geometry) = 0;
    };
};
US_DECLARE_SERVICE_INTERFACE(presentation_panel::Panel, "org.moonlightde.panel.presentation.Panel/1.0")

#endif	/* PANEL_H */

