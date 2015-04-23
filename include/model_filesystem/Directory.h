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

#ifndef DIRECTORY_H
#define	DIRECTORY_H

#define QT_NO_KEYWORDS
#include <QFile>
#include <QFuture>
#include <QString>

#include <usServiceInterface.h>

namespace model_filesystem {

    class Directory : public QObject {
        Q_OBJECT
    public:

        virtual ~Directory() {
        }

        virtual QString name() = 0;
        virtual QString uri() = 0;
        virtual QString parentUri() = 0;

        // Childs data quering 
        /**
         * Resolve the mimetype of a child.
         * @param child
         * @return 
         */
        virtual QString mimetype(const QString &child) = 0;

        /**
         * Resolve the recommende icon name for the child.
         * @param child
         * @return 
         */
        virtual QString iconName(const QString &child) = 0;
        /**
         * Resolve the permissions over a given target. By default returns the 
         * folder permissions else the permissions over the child named by "target".
         * @param target
         * @return 
         */
        virtual QFlags<QFile::Permission> permission(const QString &target = ".") = 0;

        /**
         * Sets the permissions over a given target. By default affects the folder 
         * permissions else the permissions over the child named by "target".
         * @param permissions
         * @param target
         */
        virtual QFuture<void> setPermission(QFlags<QFile::Permission> permissions, const QString &target = ".") = 0;

        /**
         * Resolve the target size. By default returns the current folder size else
         * returns the size of the file specified by "target". If the target points
         * to a folder you must also set to true the recursive param.
         * @param target
         * @param recursive Calculate folders size recursively?
         * @return 
         */
        virtual QFuture<unsigned long int> size(const QString &target = ".", bool recursive = false) = 0;
        /**
         * Resolve the target size in the device. By default returns the current folder size else
         * returns the size of the file specified by "target". If the target points
         * to a folder you must also set to true the recursive param.
         * @param target
         * @param recursive Calculate folders size recursively?
         * @return 
         */
        virtual QFuture<unsigned long int> storedSize(const QString &target = ".", bool recursive = false) = 0;

        /**
         * Resolve the last access time to the target. By default returns the last
         * access time of the current folder.
         * @param target
         * @return 
         */
        virtual unsigned long int timeAccess(const QString &target = ".") = 0;
        /**
         * Resolve the last time when the target was modified. By default returns 
         * the last access time of the current folder.
         * @param target
         * @return 
         */
        virtual unsigned long int timeModified(const QString &target = ".") = 0;
        virtual unsigned long int timeChanged(const QString &target = ".") = 0;

        /**
         * Resolve the uri of a child.
         * @param name
         * @return 
         */
        virtual QString childUri(const QString &name) = 0;

        virtual QString ownerUser(const QString &target) = 0;
        virtual QString ownerGroup(const QString &target) = 0;

        virtual int childernCount() = 0;
        virtual QList<QString> children() = 0;

        virtual QFuture<void> status() = 0;

        public
Q_SLOTS:
        virtual void update() = 0;

Q_SIGNALS:
        void changed();
        void failure(QString msg);
    };
}
Q_DECLARE_INTERFACE(model_filesystem::Directory, "org.moonlightde.panel.model_filesystem.Directory/1.0")
#endif	/* DIRECTORY_H */

