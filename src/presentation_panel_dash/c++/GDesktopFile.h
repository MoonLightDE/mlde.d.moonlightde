/* 
 * File:   GDesktopFile.h
 * Author: akiel
 *
 * Created on March 30, 2015, 1:12 PM
 */

#ifndef GDESKTOPFILE_H
#define	GDESKTOPFILE_H

#define QT_NO_KEYWORDS

#include <gio/gdesktopappinfo.h>
#include <gio/gio.h>

#include <QIcon>
#include <QString>


class GDesktopFile {
public:
    GDesktopFile(GAppInfo* app);
    GDesktopFile(GDesktopAppInfo* app);
//    GDesktopFile(const GDesktopFile& orig);
    virtual ~GDesktopFile();
    QString getName() const;
    QString getFilename();
    QString getDescription();
    QString getCategories();
    QIcon getIcon();
    GAppInfo* getApp();
    bool isShown();
    bool launchApp();
private:
    QString name;
    QString filename;
    QString description;
    QString categories;
    QIcon icon;
    GAppInfo* tapp;
    bool isshown;
};

#endif	/* GDESKTOPFILE_H */

