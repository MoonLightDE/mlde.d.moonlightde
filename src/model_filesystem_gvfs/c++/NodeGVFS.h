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


#ifndef NODEGVFS_H
#define	NODEGVFS_H

// Inclues order matters due names clashing with Qt libs.
#include <gio/gio.h>

#include "model_filesystem/Node.h"

#include <QUrl>
#include <QString>
#include <QVariant>


using namespace model_filesystem;

class NodeGVFS : public model_filesystem::Node {
public:
    NodeGVFS(QString path);
    NodeGVFS(QUrl uri);
    /**
     * Create child NODE using the parent GFile and the child info. This function
     * takes control over the child info, so you must not release it, if it 
     * belongs to other object copy it before calling.
     * @param parent
     * @param info is not duplicated, please be carefull
     */
    NodeGVFS(GFile *parent, GFileInfo* info);
    /**
     * Create a NODE using the GFile. This function takes control over the GFile
     * recieved, so you must not release it, if it belongs to other object copy 
     * it before calling.
     * @param file
     */
    NodeGVFS(GFile *file);
    NodeGVFS(const NodeGVFS& orig);
    virtual ~NodeGVFS();

    virtual Node* parent();
    virtual QList<Node*> children();

    virtual QString name();
    virtual void setName(const QString& name);


    virtual bool isValid();
    
    virtual QString mimetype();

    
    virtual QString iconName();


    virtual QString localPath();


private:
    // This functions where inspired by GVFS-bin
    bool queryInfo(const char * attributes);

    GFile * m_File;
    GFileInfo *m_FileInfo;
    
    
};

#endif	/* NODEGVFS_H */

