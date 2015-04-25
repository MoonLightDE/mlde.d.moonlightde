/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * 
 * Authors:
 *  Alexis López Zubieta
 * 
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

#ifndef THEMEMANAGER_H
#define	THEMEMANAGER_H

#include <QStringList>
#include <QHash>

class ThemeManager {
public:
    ThemeManager();
    virtual ~ThemeManager();

    void loadModuleQSS(const QString &moduleName);

    QString getIconTheme() const {
        return m_iconThemeName;
    }

    void setIconTheme(QString iconThemeName);

    QString getQssTheme() const {
        return m_qssThemeName;
    }

    void setQssTheme(QString qssThemeName);
    void removeQssTheme(QString qssThemeName);

public slots:
    void refreshStyle();


private:
    ThemeManager(const ThemeManager& orig);

    QStringList m_paths;
    QString m_qssThemeName;
    QString m_iconThemeName;
    
    QHash<QString, QString> m_qss;
};

#endif	/* THEMEMANAGER_H */

