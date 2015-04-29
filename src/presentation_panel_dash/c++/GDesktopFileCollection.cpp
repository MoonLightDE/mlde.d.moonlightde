/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Jorge Alberto Díaz Orozco
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



#include <gio/gio.h>
#include "GDesktopFileCollection.h"

#include <qt5xdg/XdgDesktopFile>

#include <QTextStream>
#include <QHash>
#include <QHashIterator>
#include <QDebug>

#include <X11/Xlib.h>

#define QT_NO_KEYWORDS

/**
 TODO: Implement some sorting here
 */

struct FileNameComparisonFunctor {

    bool
    operator()(const GDesktopFile* __x, const GDesktopFile* __y) const {
        return __x->getName() < __y->getName();
        //        return 0;
    }
};

GDesktopFileCollection::GDesktopFileCollection() {
    allApps = QList<GDesktopFile*>();
    generateCache();
}

QList<GDesktopFile*> GDesktopFileCollection::getAll() {
    return allApps;
}

void GDesktopFileCollection::generateCache() {
    GList* appList = g_app_info_get_all();
    QList<GDesktopFile*> res;

    GList* aux = NULL;
    for (aux = appList; aux; aux = aux->next) {
        GDesktopFile* desktopfile;
        GAppInfo* appInfo = (GAppInfo*) aux->data;
        desktopfile = new GDesktopFile(appInfo);
        if (desktopfile->isShown()) {
            res.append(desktopfile);
        }
        //        qDebug() << "Loading Desktop File for " << desktopfile->getFilename();
    }


    qSort(res.begin(), res.end(), FileNameComparisonFunctor());

    qDeleteAll(allApps);
    allApps.clear();

    allApps = res;
}

QList<GDesktopFile*> GDesktopFileCollection::filter(QHash<QString, QString> filters) {
    QList<GDesktopFile*> appList = allApps;
    QList<GDesktopFile*> res;

    //    qSort(appList.begin(), appList.end(), FileNameComparisonFunctor());

    for (GDesktopFile * app : appList) {
        QHashIterator<QString, QString> filter(filters);
        while (filter.hasNext()) {
            filter.next();
            QString appValue = "";
            if (filter.key() == "name") {
                appValue = app->getName();
            } else if (filter.key() == "description") {
                appValue = app->getDescription();
            } else if (filter.key() == "category") {
                appValue = app->getCategories();
            }

            if (appValue.contains(filter.value(), Qt::CaseInsensitive)) {
                res.append(app);
                break;
            }
        }
    }
    return res;
}

GDesktopFileCollection::~GDesktopFileCollection() {
    qDeleteAll(allApps);
    allApps.clear();
}

