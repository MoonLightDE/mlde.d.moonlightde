/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 * Alexis López Zubieta
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

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include <QDebug>
#include <QListView>

US_USE_NAMESPACE
/**g_object_unref(child);
 */
class Activator : public ModuleActivator {
private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {
        item = new NodeGVFS("/usr/bin");
        
        qDebug() << "URI: " << item->uri() << ", name: " << item->name() << ", type: " << item->mimetype() << ", icon: " << item->iconName();
        Node * parent = item->parent();
        qDebug() << "URI: " << parent->uri() << ", name: " << parent->name() << ", type: " << parent->mimetype() << ", icon: " << parent->iconName();
        
        QList<Node *> children = item->children();
        for (Node * child : children) {
            qDebug() << "URI: " << child->uri() << ", name: " << child->name() << ", type: " << child->mimetype() << ", icon: " << child->iconName();
        }
        
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        delete item;
    }

    QListView view;
    NodeGVFS *item;

};
US_EXPORT_MODULE_ACTIVATOR(model_filesystem_gvfs, Activator)
