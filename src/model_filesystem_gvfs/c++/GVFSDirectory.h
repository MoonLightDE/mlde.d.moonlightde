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


#ifndef GVFSDIRECTORY_H
#define	GVFSDIRECTORY_H

#define QT_NO_KEYWORDS
#include "model_filesystem/Directory.h"

#include <QFile>
#include <QObject>
#include <QList>
#include <QString>
#include <QHash>
#include <QFuture>
#include <QFutureInterface>

#include <glib.h>
#include <gio/gio.h>
#undef QT_NO_KEYWORDS

class GVFSDirectoryPriv : public QObject {
    Q_OBJECT
    friend class GVFSDirectory;
public:
    GVFSDirectoryPriv(GFile * gfile);
    ~GVFSDirectoryPriv();
    QFutureInterface<void> m_FutureInterface;

    QString m_uri;
    QString m_User;
    QString m_Password;
    GFile * m_File;
    GFileInfo *m_FileInfo;
    QHash<QString, GFileInfo *> m_ChildrenInfo;

    void startMount();
    void startQueryFileInfo();
    void startEnumerateChildren();

    static void handleMountAskPassword(GMountOperation *op, gchar *message, gchar *default_user, gchar *default_domain, GAskPasswordFlags flags, gpointer userdata);
    static void handleMountDone(GObject *source_object, GAsyncResult *res, gpointer user_data);
    static void handleQueryFileInfoDone(GObject *source_object, GAsyncResult *res, gpointer user_data);
    static void handleEnumerateChildrenDone(GObject *source_object, GAsyncResult *res, gpointer user_data);
    static void handleEnumeratorNextFilesDone(GObject *source_object, GAsyncResult *res, gpointer user_data);
    static void handleEnumeratorCloseDone(GObject *source_object, GAsyncResult *res, gpointer user_data);

    void reportError(const QString &msg);

    void clear();

    static char attributes[];
    static bool nofollow_symlinks;

Q_SIGNALS:
    void changed();
    void failure(QString msg);
};

/**
 * Design principles:
 * - Asynchronic
 * - Allways resposive and valid.
 * 
 * Representation of a directory. 
 * 
 * This also allow the access to the file system where the folder is stored in 
 * order to retrieve information relevant to it and to monitor the changes on
 * the data.
 */
class GVFSDirectory : public model_filesystem::Directory {
    Q_OBJECT
    Q_INTERFACES(model_filesystem::Directory)
public:
    GVFSDirectory(const QString &uri);
    GVFSDirectory(GFile * gfile);

    virtual ~GVFSDirectory();

    virtual QString name();
    virtual QString uri();
    virtual QString parentUri();

    // Childs data quering 
    /**
     * Resolve the mimetype of a child.
     * @param child
     * @return 
     */
    virtual QString mimetype(const QString &child);

    /**
     * Resolve the recommende icon name for the child.
     * @param child
     * @return 
     */
    virtual QString iconName(const QString &child);
    /**
     * Resolve the permissions over a given target. By default returns the 
     * folder permissions else the permissions over the child named by "target".
     * @param target
     * @return 
     */
    virtual QFlags<QFile::Permission> permission(const QString &target = ".");

    /**
     * Sets the permissions over a given target. By default affects the folder 
     * permissions else the permissions over the child named by "target".
     * @param permissions
     * @param target
     */
    virtual QFuture<void> setPermission(QFlags<QFile::Permission> permissions, const QString &target = ".");

    /**
     * Resolve the target size. By default returns the current folder size else
     * returns the size of the file specified by "target". If the target points
     * to a folder you must also set to true the recursive param.
     * @param target
     * @param recursive Calculate folders size recursively?
     * @return 
     */
    virtual QFuture<unsigned long int> size(const QString &target = ".", bool recursive = false);
    /**
     * Resolve the target size in the device. By default returns the current folder size else
     * returns the size of the file specified by "target". If the target points
     * to a folder you must also set to true the recursive param.
     * @param target
     * @param recursive Calculate folders size recursively?
     * @return 
     */
    virtual QFuture<unsigned long int> storedSize(const QString &target = ".", bool recursive = false);

    /**
     * Resolve the last access time to the target. By default returns the last
     * access time of the current folder.
     * @param target
     * @return 
     */
    virtual unsigned long int timeAccess(const QString &target = ".");
    /**
     * Resolve the last time when the target was modified. By default returns 
     * the last access time of the current folder.
     * @param target
     * @return 
     */
    virtual unsigned long int timeModified(const QString &target = ".");
    virtual unsigned long int timeChanged(const QString &target = ".");

    /**
     * Resolve the uri of a child.
     * @param name
     * @return 
     */
    virtual QString childUri(const QString &name);

    virtual QString ownerUser(const QString &target);
    virtual QString ownerGroup(const QString &target);

    virtual int childernCount();
    virtual QList<QString> children();

    virtual QFuture<void> status();

    public
Q_SLOTS:
    void update();

Q_SIGNALS:
    void changed();
    void failure(QString msg);

private:


    //    QFutureInterface<void> m_FutureInterface;
    //
    //    QString m_uri;
    //    QString m_User;
    //    QString m_Password;
    //    GFile * m_File;
    //    GFileInfo *m_FileInfo;
    //    QHash<QString, GFileInfo *> m_ChildrenInfo;
    //


    GVFSDirectoryPriv* m_Priv;
    static QHash<QString, GVFSDirectoryPriv*> * m_Cache;
    static QHash<GVFSDirectoryPriv*, int> *m_Refs;
};


#endif	/* GVFSDIRECTORY_H */

