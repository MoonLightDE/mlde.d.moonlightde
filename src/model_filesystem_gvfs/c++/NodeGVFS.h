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

#include <QMap>
#include <QString>
#include <QVariant>


using namespace model_filesystem;

class NodeGVFS : public Node {
public:
    NodeGVFS(QString uri);
    NodeGVFS(const GFile *file);
    NodeGVFS(const NodeGVFS& orig);
    virtual ~NodeGVFS();

    virtual QVariant property(Property property);

    virtual QVariant queryProperty(Node::Property property);

    virtual PropertyMap queryProperties(QList<Node::Property> properties);


    virtual QList<Node> children();


    static const QMap<Property, QString> mapPropertyToGVFS;
    static const QMap<QString, Property> mapPropertyFromGVFS;

private:
    inline QByteArray mapProperties(QList<Node::Property> properties);

    // This functions where inspired by GVFS-bin
    bool queryInfo(const char * attributes);
    void updateCache(GFileInfo *info);

    GFile * m_File;

    // Cache
    PropertyMap m_PropertiesCache;
};

#endif	/* NODEGVFS_H */

