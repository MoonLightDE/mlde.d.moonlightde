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


#include "module_config.h"
#include "NodeGVFS.h"

#include <glib.h>

#include <QObject>
#include <QDebug>
#include <QStringList>

const QMap<Node::Property, QString> NodeGVFS::mapPropertyToGVFS = {
    {Node::NAME, "standard::name"},
    {Node::ICON_NAME, "standard::icon"},
    {Node::MIMETYPE, "standard::fast-content-type"},
    {Node::SIZE, "standard::size"},
    {Node::STORED_SIZE, "standard::allocated-size"}
};

const QMap<QString, Node::Property > NodeGVFS::mapPropertyFromGVFS = {
    {"standard::name", Node::NAME},
    {"standard::icon", Node::ICON_NAME},
    {"standard::fast-content-type", Node::MIMETYPE},
    {"standard::size", Node::SIZE},
    {"standard::allocated-size", Node::STORED_SIZE}
};

NodeGVFS::NodeGVFS(QString uri) : model_filesystem::Node(uri), m_PropertiesCache() {
    m_File = g_file_new_for_path(uri.toLocal8Bit());
}

NodeGVFS::NodeGVFS(const GFile *file) : model_filesystem::Node(""), m_PropertiesCache() {
    if (file == NULL)
        qWarning() << MODULE_NAME_STR << " node created from NULL GFile.";

    const char * uri = g_file_get_uri(m_File);
    model_filesystem::Node::setUri(uri);

    m_File = g_file_new_for_uri(uri);
}

NodeGVFS::NodeGVFS(const NodeGVFS & orig) : model_filesystem::Node(orig) {
    m_File = g_file_dup(orig.m_File);
    m_PropertiesCache = orig.m_PropertiesCache;
}

NodeGVFS::~NodeGVFS() {
    g_object_unref(m_File);
}

QVariant NodeGVFS::property(Property property) {
    if (m_PropertiesCache.contains(property))
        return m_PropertiesCache.value(property);

    return queryProperty(property);
}

QVariant NodeGVFS::queryProperty(Node::Property property) {
    QList<Node::Property> prop = {property};
    queryInfo(mapProperties(prop));
    return m_PropertiesCache.value(property, QVariant());
}

Node::PropertyMap NodeGVFS::queryProperties(QList<Node::Property> properties) {
    queryInfo(mapProperties(properties));

    return m_PropertiesCache;
}

bool NodeGVFS::queryInfo(const char * attributes) {
    GFileQueryInfoFlags flags;
    GFileInfo *info;
    GError *error;

    if (m_File == NULL)
        return FALSE;

    //    if (attributes == "")
    attributes = "*";

    qDebug() << "[" << MODULE_NAME_STR << "] Getting info :" << attributes;
    flags = G_FILE_QUERY_INFO_NONE;

    error = NULL;

    info = g_file_query_info(m_File, attributes, flags, NULL, &error);

    if (info == NULL) {
        qDebug() << "[" << MODULE_NAME_STR << "] [Error] getting info :" << error->message;
        g_error_free(error);
        return FALSE;
    }


    updateCache(info);

    g_object_unref(info);

    return TRUE;
}

void NodeGVFS::updateCache(GFileInfo * info) {
    char **attributes;
    char *value;
    attributes = g_file_info_list_attributes(info, NULL);
    for (int i = 0; attributes[i] != NULL; i++) {
        /* list the icons in order rather than displaying "GThemedIcon:0x8df7200" */
        if (strcmp(attributes[i], "standard::icon") == 0 ||
                strcmp(attributes[i], "standard::symbolic-icon") == 0) {
            GIcon *icon;
            int j;
            const gchar * const * names = NULL;

            if (strcmp(attributes[i], "standard::symbolic-icon") == 0)
                icon = g_file_info_get_symbolic_icon(info);
            else
                icon = g_file_info_get_icon(info);

            /* only look up names if GThemedIcon */
            if (G_IS_THEMED_ICON(icon)) {
                names = g_themed_icon_get_names(G_THEMED_ICON(icon));
            }
            if (names != NULL) {
                QVariantList nameVariants;
                for (; (*names) != NULL; names++) {
                    QVariant str = QString::fromLocal8Bit((*names));
                    nameVariants.append(str);
                }

                m_PropertiesCache.insert(mapPropertyFromGVFS.value(attributes[i]), nameVariants);
            }

        } else {
            value = g_file_info_get_attribute_as_string(info, attributes[i]);

            qDebug() << MODULE_NAME_STR << " " << attributes[i] << " mapped as " << mapPropertyFromGVFS.value(attributes[i]) << " value " << QString(value);
            qDebug() << MODULE_NAME_STR << " contained: " << mapPropertyFromGVFS.contains(attributes[i]);
            if (mapPropertyFromGVFS.contains(attributes[i]))
                m_PropertiesCache.insert(mapPropertyFromGVFS.value(attributes[i]), value);

            g_free(value);
        }
    }
    qDebug() << MODULE_NAME_STR <<  " : properties cache update:  \n\n" << m_PropertiesCache;
    g_strfreev(attributes);
}

QByteArray NodeGVFS::mapProperties(QList<Node::Property> properties) {
    QStringList props;

    for (const Node::Property &key : properties)
        props.append(mapPropertyToGVFS.value(key, QString()));

    return props.join(",").toLocal8Bit();
}

QList<Node> NodeGVFS::children() {
    QList<Node> children;

    GFileEnumerator *enumerator;
    GFileInfo *info;
    GError *error;
    gboolean res;

    static gboolean nofollow_symlinks = FALSE;
    char attributes[] = "standard::*";

    if (m_File == NULL)
        return children;

    error = NULL;
    enumerator = g_file_enumerate_children(m_File,
            attributes,
            nofollow_symlinks ? G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS : G_FILE_QUERY_INFO_NONE,
            NULL,
            &error);
    if (enumerator == NULL) {
        qWarning() << MODULE_NAME_STR << " " << error->message;
        g_error_free(error);
        error = NULL;
        return children;
    }

    res = TRUE;
    while ((info = g_file_enumerator_next_file(enumerator, NULL, &error)) != NULL) {
        const char * childName = g_file_info_get_name(info);
        const GFile * child = g_file_get_child(m_File, childName);
        children.append(NodeGVFS(child));

        g_object_unref(info);
    }

    if (error) {
        qWarning() << MODULE_NAME_STR << " " << error->message;
        g_error_free(error);
        error = NULL;
        res = FALSE;
    }

    if (!g_file_enumerator_close(enumerator, NULL, &error)) {
        qWarning() << MODULE_NAME_STR << " " << error->message;
        g_error_free(error);
        error = NULL;
        res = FALSE;
    }

    return children;
}
