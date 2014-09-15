/* 
 * File:   AppButton.cpp
 * Author: sigfried
 * 
 * Created on September 12, 2014, 2:21 PM
 */

#include "AppButton.h"

#include <qt5xdg/xdgdesktopfile.h>
#include <qt5xdg/XdgIcon>

#include <QDebug>

AppButton::AppButton(XdgDesktopFile* desktopFile, QWidget* parent) : QPushButton(parent), m_desktopFile(desktopFile) {
    setToolTip(m_desktopFile->comment());
    
    XdgIcon::setThemeName("Razor-by-gibaalav");
    setIcon(m_desktopFile->icon(XdgIcon::defaultApplicationIcon()));
    connect(this, SIGNAL(released()), SLOT(onClick()));
}

AppButton::~AppButton() {
    delete m_desktopFile;
}

void AppButton::onClick() {
    m_desktopFile->startDetached();
}

