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

#include <QAction>
#include <QString>
#include <QList>
#include <usServiceInterface.h>

namespace model_filesystem {

    class Node;

    class FileSystem {
    public:

        virtual ~FileSystem() {
        }


        virtual QStringList getSupportedUriScheme() = 0;
        virtual Node* getNode(QString path) = 0;
        virtual QList<QAction> getActions(QList<Node*> nodes) = 0;
        
        //TODO: Query file system info
    private:

    };
}
US_DECLARE_SERVICE_INTERFACE(model_filesystem::FileSystem, "org.moonlightde.panel.model_filesystem.FileSystem/1.0")
#endif	/* FILESYSTEM_H */

