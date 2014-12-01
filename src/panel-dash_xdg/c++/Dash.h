/* 
 * File:   Dash.h
 * Author: alexis
 *
 * Created on 10 de septiembre de 2014, 16:21
 */

#ifndef _DASH_H
#define	_DASH_H

#include "ui_Dash.h"

#include <QFrame>
#include <LXQt/Settings>
#include <QFileSystemWatcher>
#include "GridLayoutHExpanding.h"
#include "GridLayoutVExpanding.h"
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
    void showEvent(QShowEvent * event);

private slots:
    void onItemTrigerred(const QModelIndex& item);
    void onApplicationsFolderChanged();
    void searchEditChanged(QString);
    void removeFavorites(XdgDesktopFile* app);
    
    void showContextMenuForApp(QPoint);
    void showContextMenuForStart(QPoint);
    
    void addFavorite();
    void removeFavorite();

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
