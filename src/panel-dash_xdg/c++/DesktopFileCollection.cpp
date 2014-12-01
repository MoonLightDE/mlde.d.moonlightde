/* 
 * File:   DesktopFileCollection.cpp
 * Author: akiel
 * 
 * Created on October 3, 2014, 3:10 PM
 */

#include "DesktopFileCollection.h"
#include <qt5xdg/XdgDesktopFile>
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
    QList<XdgDesktopFile*> appList = XdgDesktopFileCache::getAllFiles();
    QList<XdgDesktopFile*> res;

    foreach(XdgDesktopFile * app, appList) {
        if (app->type() != XdgDesktopFile::ApplicationType) {
            continue;
        } else {
            res.append(app);
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

