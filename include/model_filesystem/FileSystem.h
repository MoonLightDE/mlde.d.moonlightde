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

#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#include <QString>
#include <QList>
#include <usServiceInterface.h>

#include "Directory.h"

namespace model_filesystem {

    class FileSystem {
    public:
        virtual ~FileSystem() {
        };


        virtual QStringList getSupportedUriScheme() = 0;

        /**
         * Retrieve a directory descriptor for the given uri. The result must be
         * released later with <code>releaseDirectory</code>
         * @param uri
         * @return 
         */
        virtual Directory* getDirectory(const QString &uri) = 0;
        virtual void releaseDirectory(Directory* dir) = 0;

        //    virtual QFuture<void> copy(QList<QString> sources, QString destination);
        //    virtual QFuture<void> move(QList<QString> sources, QString destination);
        //    virtual QFuture<void> trash(QList<QString> targets);
        //    virtual QFuture<void> remove(QList<QString> targets);
        //    
        //    virtual QFuture<void> mkdir(QString target);

    };
}
US_DECLARE_SERVICE_INTERFACE(model_filesystem::FileSystem, "org.moonlightde.panel.model_filesystem.FileSystem/1.0")
#endif	/* FILESYSTEM_H */

