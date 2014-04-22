/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef LXQT_MAINMENU_H
#define LXQT_MAINMENU_H

#include "menustyle.h"

#include "panel/IPanel.h"

#include <qt5xdg/xdgmenu.h>
#include <menu-cache/menu-cache.h>

#include <QLabel>
#include <QToolButton>
#include <QDomElement>
#include <QAction>
#include <QSettings>
#include <QPointer>


class QMenu;
class LxQtBar;

namespace LxQt {
    class PowerManager;
    class ScreenSaver;
}

namespace GlobalKeyShortcut {
    class Action;
}

class LxQtMainMenu : public QToolButton, public ILauncher {
    Q_OBJECT
public:
    LxQtMainMenu();
    ~LxQtMainMenu();

    QString themeId() const {
        return "MainMenu";
    }

    QDialog *configureDialog();

    bool isSeparate() const {
        return true;
    }

private:
    QPointer<QSettings> m_settings;
    QPointer<QWidget> m_panel;

    //QToolButton mButton;
    QString mLogDir;
    QMenu* mMenu;
    //    GlobalKeyShortcut::Action *mShortcut;
    MenuStyle mTopMenuStyle;
    MenuStyle mMenuStyle;
    LxQt::PowerManager* mPowerManager;
    LxQt::ScreenSaver* mScreenSaver;


    MenuCache* mMenuCache;
    MenuCacheNotifyId mMenuCacheNotify;
    static void menuCacheReloadNotify(MenuCache* cache, gpointer user_data);

    bool mLockCascadeChanges;

protected slots:

    virtual void settingsChanged();
    void buildMenu();

private slots:
    void showMenu();
    void showHideMenu();
    void shortcutChanged(const QString &oldShortcut, const QString &newShortcut);
};



#endif
