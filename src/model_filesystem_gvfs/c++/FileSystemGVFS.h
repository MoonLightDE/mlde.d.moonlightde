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
#ifndef FILESYSTEMGVFS_H
#define	FILESYSTEMGVFS_H

#include "model_filesystem/FileSystem.h"
#include "GVFSDirectory.h"

#include <QFuture>
#include <QString>

using namespace model_filesystem;

class FileSystemGVFS /*: public model_filesystem::FileSystem */{
public:
    FileSystemGVFS();
    virtual ~FileSystemGVFS();


    virtual QStringList getSupportedUriScheme();
    
    /**
     * Retrieve a directory descriptor for the given uri. This function usually
     * takes a while so it's recomended to run it with <code>QtConcurrent::Run</code>.
     * @param uri
     * @return 
     */
    virtual GVFSDirectory*  getDirectory(const QString &uri);
    virtual void releaseDirectory(GVFSDirectory* dir);

    //    virtual QList<QAction> getActions(QList<QString> paths);

private:
    /**
     * Sanitises the path string in order to obtain a homogeneous representation.
     * @param path
     * @return 
     */
    QString cleanPath(const QString &path); 
    QStringList m_SupportedUriSchemes;
    GVfs * m_GVfs;
    QHash<QString, GVFSDirectory*> m_Cache;
    QHash<GVFSDirectory*, int> m_Refs;
};

#endif	/* FILESYSTEMGVFS_H */

