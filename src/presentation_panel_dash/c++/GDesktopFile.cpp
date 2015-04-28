/* 
 * File:   GDesktopFile.cpp
 * Author: akiel
 * 
 * Created on March 30, 2015, 1:12 PM
 */

#include "GDesktopFile.h"
#include <QString>
//#include <gio-unix-2.0/gio/gdesktopappinfo.h>


GDesktopFile::GDesktopFile(GDesktopAppInfo* app) {
    //work in progress
    this->tapp = G_APP_INFO(app);
    name = g_app_info_get_name(this->tapp);
    const char* const* descAux = g_desktop_app_info_get_keywords(app);
    
    if (descAux != NULL) {
        // Just return the first icon
        for (; (*descAux) != NULL; descAux++) {
            description = QString::fromLocal8Bit((*descAux));
            break;
        }
    }
    
    categories = g_desktop_app_info_get_categories(app);
    isshown = !g_desktop_app_info_get_nodisplay(app);
    filename = g_desktop_app_info_get_filename(app);
    GIcon *icon2 = NULL;

    const gchar * const * names = NULL;

    icon2 = g_app_info_get_icon (G_APP_INFO(app));

//    if (icon == NULL)
//        icon = g_file_info_get_symbolic_icon(m_FileInfo);

    if (G_IS_THEMED_ICON(icon2)) {
        names = g_themed_icon_get_names(G_THEMED_ICON(icon2));
    }
    if (names != NULL) {
        // Just return the first icon
        for (; (*names) != NULL; names++) {
            QString filename = QString::fromLocal8Bit((*names));
            icon = QIcon::fromTheme(filename);
            break;
        }
    }
//    g_object_unref(icon2);
}

GDesktopFile::GDesktopFile(GAppInfo* app) {
    this->tapp = app;
    name = g_app_info_get_name(app);
    description = g_app_info_get_description(app);
    GDesktopAppInfo* dainfo = g_desktop_app_info_new(g_app_info_get_id(app));
    categories = g_desktop_app_info_get_categories(dainfo);
    isshown = !g_desktop_app_info_get_nodisplay(dainfo);
    filename = g_desktop_app_info_get_filename(dainfo);
    GIcon *icon2 = NULL;
    int j;
    const gchar * const * names = NULL;

    icon2 = g_app_info_get_icon(app);

//    if (icon == NULL)
//        icon = g_file_info_get_symbolic_icon(m_FileInfo);

    if (G_IS_THEMED_ICON(icon2)) {
        names = g_themed_icon_get_names(G_THEMED_ICON(icon2));
    }
    if (names != NULL) {
        // Just return the first icon
        for (; (*names) != NULL; names++) {
            QString filename = QString::fromLocal8Bit((*names));
            icon = QIcon::fromTheme(filename);
            break;
        }
    }
//    g_object_unref(icon2);
}

//GDesktopFile::GDesktopFile(const GDesktopFile& orig) {
//}

GDesktopFile::~GDesktopFile() {
}

QString GDesktopFile::getName() const{
    return name;
}

QString GDesktopFile::getFilename(){
    return filename;
}

QString GDesktopFile::getDescription(){
    return description;
}

QString GDesktopFile::getCategories(){
    return categories;
}

QIcon GDesktopFile::getIcon(){
    return icon;
}

bool GDesktopFile::isShown(){
    return isshown;
}

bool GDesktopFile::launchApp(){
    return g_app_info_launch(tapp,NULL,NULL,NULL);
}
