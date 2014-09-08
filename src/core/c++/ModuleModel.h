/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
 * 
 * This file is part of Moonlight Desktop Environment Core.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODULEMODEL_H
#define MODULEMODEL_H

#include "../include/core/ICore.h"
#include "../include/core/IQt5.h"

#include <qt5xdg/XdgDesktopFile>

#include <usModuleContext.h>
#include <usServiceTracker.h>

#include <QStringListModel>

class ModuleModel : public QAbstractListModel {
    Q_OBJECT
public:
    ModuleModel(QObject *parent = 0);
    ~ModuleModel();
    //    void reset();
    //    void toggleModule(const QModelIndex &index, bool status);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    int rowCount(const QModelIndex& parent) const;

    int columnCount(const QModelIndex&) const {
        return 3;
    }

    Qt::ItemFlags flags(const QModelIndex& index) const;

public slots:
    //    void updateModuleState(QString moduleName, bool state);
    void toggleModule(int index);
    void writeChanges();
    void updateModelData();

private:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (role == Qt::DisplayRole) {

            switch (section) {
                case 0:
                    return tr("Module");
                case 1:
                    return tr("State");
                case 2:
                    return tr("StartUp");
            }
        }
        return QVariant();
    }

    QMap<QString, XdgDesktopFile> mItemMap;
    QMap<QString, bool> mStateMap;
    QStringList mKeyList;
    //    QDBusInterface* mInterface;

    // Module context
    us::ModuleContext* m_context;
    // The service tracker
    us::ServiceTracker<Core::IModuleManager>* m_ModuleManagerTracker;
};

#endif // MODULEMODEL_H
