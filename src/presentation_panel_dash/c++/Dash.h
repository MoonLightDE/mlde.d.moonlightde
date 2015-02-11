/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 *      Jorge Fernández Sánchez
 *      Jorge Alberto Díaz Orozco
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

#ifndef _DASH_H
#define	_DASH_H

#include "ui_Dash.h"

#include <QFrame>
#include <LXQt/Settings>
#include <QFileSystemWatcher>
#include "DesktopFileCollection.h"
#include "DashViewModel.h"
#include <QListView>

class Dash : public QFrame {
    Q_OBJECT
public:
    Dash();
    virtual ~Dash();

    void build();
    void configView(QListView* view);
    void buildSearch(QString search);
    void cleanApps();
    void addFavorites(XdgDesktopFile* app);
    void getFavorites();
    void hideEvent(QHideEvent *event);


protected:
    /** 
     * Description: 
     *  Setups the widget to before it's displayed. Loads apps in
     *  case of update or first show, igline new apps and restore focus to the
     *  "start" section. 
     **/
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent * event);
public slots:
    void toggle();
    
private slots:
    void onItemTrigerred(const QModelIndex& item);
    void onApplicationsFolderChanged();
    void searchEditChanged(QString);
    void removeFavorites(XdgDesktopFile* app);

    void showContextMenuForApp(QPoint);
    void showContextMenuForStart(QPoint);

    void addFavorite();
    void removeFavorite();
    void onReturnPressed();

private:
    QFileSystemWatcher* monitor;
    DashViewModel* appDashModel;
    DashViewModel* settingsDashModel;
    DashViewModel* startDashModel;

    Ui::Dash m_ui;
    LxQt::Settings m_settings;
    // TODO: arreglar este churre
    bool built;
    int appIndex;
    DesktopFileCollection* appListGenerator;
    void putFavorites(QList<XdgDesktopFile*> favAppList);

};

#endif	/* _DASH_H */
