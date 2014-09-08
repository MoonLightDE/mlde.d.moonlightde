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


#include <usGetModuleContext.h>

#include <QDebug>
#include <qt5xdg/xdgicon.h>

#include "ModuleModel.h"
#include "ModuleManager.h"

ModuleModel::ModuleModel(QObject* parent)
: QAbstractListModel(parent) {

    m_context = us::GetModuleContext();

    // Create a service tracker to monitor dictionary services.
    m_ModuleManagerTracker = new us::ServiceTracker<Core::IModuleManager>(
            m_context, us::LDAPFilter(std::string("(&(") + us::ServiceConstants::OBJECTCLASS() + "=" +
            us_service_interface_iid<Core::IModuleManager>() + "))")
            );
    m_ModuleManagerTracker->Open();

    updateModelData();
}

ModuleModel::~ModuleModel() {

    delete m_ModuleManagerTracker;
}

QVariant ModuleModel::data(const QModelIndex& index, int role) const {
    Core::IModuleManager * moduleManager = m_ModuleManagerTracker->GetService();
    if (moduleManager == NULL)
        qDebug() << "Unable to locate the ModuleManager Service.";
    else {
        QList<QString> activeModules = moduleManager->getActiveModules();
        QString name = mKeyList.at(index.row());
        switch (index.column()) {
            case 0:
                switch (role) {
                    case Qt::DecorationRole:
                        return mItemMap[name].icon();
                    case Qt::DisplayRole:
                        return mItemMap[name].name();
                    case Qt::ToolTipRole:
                        return mItemMap[name].comment();
                    default:
                        return QVariant();
                }
            case 1:
                switch (role) {
                    case Qt::DisplayRole:
                        if (activeModules.contains(name))
                            return tr("Active");
                        else
                            return tr("Unactive");
                    default:
                        return QVariant();
                }
            case 2:
                switch (role) {
                    case Qt::CheckStateRole:
                        return mStateMap[name] ? Qt::Checked : Qt::Unchecked;
                    default:
                        return QVariant();
                }
        }
    }
    return QVariant();
}

void ModuleModel::toggleModule(int index) {
    QString moduleName = mKeyList[index];

    Core::IModuleManager * moduleManager = m_ModuleManagerTracker->GetService();
    if (moduleManager == NULL)
        qDebug() << "Unable to locate the ModuleManager Service.";
    else {
        if (moduleManager->getActiveModules().contains(moduleName))
            moduleManager->unload(moduleName);
        else
            moduleManager->load(moduleName);
    }
    updateModelData();
}

void ModuleModel::updateModelData() {
    mKeyList.clear();
    mItemMap.clear();
    mStateMap.clear();
    Core::IModuleManager * moduleManager = m_ModuleManagerTracker->GetService();
    if (moduleManager == NULL)
        qDebug() << "Unable to locate the ModuleManager Service.";
    else {
        QStringList startUpModules = moduleManager->getStartUpModules();
        foreach (QString moduleName, startUpModules) {
            mStateMap[moduleName] = true;
        }

        mKeyList = moduleManager->getAviableModules();
        QStringList activeModules = moduleManager->getActiveModules();

        foreach(QString moduleName, mKeyList) {
            mItemMap.insert(moduleName, *moduleManager->getModuleDescriptor(moduleName));
        }
    }
}

bool ModuleModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role == Qt::CheckStateRole) {
        QString key = mKeyList.at(index.row());
        mStateMap[key] = (value == Qt::Checked);
        emit dataChanged(index, index);
        writeChanges();
        return true;
    }
    return false;
}

Qt::ItemFlags ModuleModel::flags(const QModelIndex & index) const {
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

int ModuleModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return mKeyList.size();
}

void ModuleModel::writeChanges() {
    int count = 0;

    QStringList startUpModules;
    foreach(QString key, mStateMap.keys()) {
        if (mStateMap[key])
            startUpModules.append(key);
    }

    Core::IModuleManager * moduleManager = m_ModuleManagerTracker->GetService();
    if (moduleManager == NULL)
        qDebug() << "Unable to locate the ModuleManager Service.";
    else {
        moduleManager->setStartUpModules(startUpModules);
    }

}