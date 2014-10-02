/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 *      Jorge Fernández Sánchex
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

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "panel/IPanel.h"

#include <usModuleContext.h>

#include <QFrame>
#include <QRect>
#include <usServiceTracker.h>

namespace Ui {
    class Panel;
}

class Panel : public QFrame, public IPanel {
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);

    ~Panel();

public slots:
    void setupWindowFlags();
    void adjustSizeToScreen();
    void requestExclusiveScreenArea();

    void startButtonClicked();

protected:
    bool event(QEvent *event);


    template<class Interface> inline QWidget *getPanelWidget(us::ModuleContext * context);

private:
    Ui::Panel *ui;
    
    us::ServiceTracker<ILauncherFactory> *m_launcherTracker;
    QWidget *quicklauncher;
    QWidget *taskBar;
    QWidget *sysTray;
    QWidget *clock;

    /* Panel properties */
    int mHeight;

    //jfsanchez@estudiantes.uci.cu
    bool visibleDash;
    
};

#endif // LAUNCHER_H
