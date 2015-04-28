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


#include "QTextStream"
#include "DesktopFileCollection.h"
#include <qt5xdg/XdgDesktopFile>
#include <X11/Xlib.h>
#include "QHash"
#include "QHashIterator"
#include "QDebug"


/**
 TODO: Implement some sorting here
 */

struct XdgDesktopFileComparisonFunctor {

    bool
    operator()(const XdgDesktopFile * __x, const XdgDesktopFile * __y) const {
        return __x->name() < __y->name();
    }
};

DesktopFileCollection::DesktopFileCollection() {

}

QList<XdgDesktopFile*> DesktopFileCollection::all() {
    qDebug() << "Building app list";
    QList<XdgDesktopFile*> appList = XdgDesktopFileCache::getAllFiles();
    QList<XdgDesktopFile*> res;

    foreach(XdgDesktopFile * app, appList) {
        if (app->type() != XdgDesktopFile::ApplicationType) {
            continue;
        } else {
            bool indexed = false;
            //esto elimina los duplicados pero demora un poco
//            foreach(XdgDesktopFile * appRes, res) {
//                if (appRes->name().compare(app->name()) == 0 ){
//                    indexed = True;
//                    break;
//                }
//            }
            if (!indexed){
                res.append(app);
            }
        }
    }
    qSort(res.begin(), res.end(), XdgDesktopFileComparisonFunctor());
    return res;
}

QList<XdgDesktopFile*> DesktopFileCollection::filter(QHash<QString, QString> filters) {
    QList<XdgDesktopFile*> appList = all();
    QList<XdgDesktopFile*> res;

    qSort(appList.begin(), appList.end(), XdgDesktopFileComparisonFunctor());

    foreach(XdgDesktopFile * app, appList) {
        QHashIterator<QString, QString> filter(filters);
        while (filter.hasNext()) {
            filter.next();
            QString appValue = app->value(filter.key(), "None").toString();
            if (appValue.contains(filter.value(), Qt::CaseInsensitive)) {
                res.append(app);
                break;
            }
        }
    }
    return res;
}

DesktopFileCollection::~DesktopFileCollection() {
}

