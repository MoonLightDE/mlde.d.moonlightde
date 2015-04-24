/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
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

#ifndef BROWSINGINSTANCE_H
#define	BROWSINGINSTANCE_H

#include "FileSystemsTracker.h"

#include <QList>
#include <QObject>
#include <QString>

#include <functional>

namespace model_filesystem {
    class Directory;
    class FileSystem;
}

class BrowsingInstance : public QObject {
    Q_OBJECT

    friend class Activator;
    friend class FileSystemsTracker;
public:
    BrowsingInstance();
    virtual ~BrowsingInstance();

    QString currentPath();
    model_filesystem::Directory * currentDir();

    bool canGoBack();
    bool canGoForward();

    QList<QString> history();

    public
Q_SLOTS:
    model_filesystem::Directory * goTo(const QString &path);
    model_filesystem::Directory * goBack();
    model_filesystem::Directory * goForward();

Q_SIGNALS:
    void directoryChanged(model_filesystem::Directory *);

protected:
    static void addFileSystem(model_filesystem::FileSystem * fileSystem);
    static void removeFileSystem(model_filesystem::FileSystem* fileSystem);

    static model_filesystem::Directory * getDirectory(const QString &path);
private:
    int m_HistoryPos;
    QString m_CurrentPath;
    model_filesystem::Directory * m_CurrentDir;
    QList<QString> m_History;

    static QList<model_filesystem::FileSystem* > *m_AvailableFileSystems;
    static FileSystemsTracker *m_Tracker;
};

#endif	/* BROWSINGINSTANCE_H */

