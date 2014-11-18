/* 
 * File:   AppButton.cpp
 * Author: sigfried
 * 
 * Created on September 12, 2014, 2:21 PM
 */

#include "AppButton.h"
#include "qevent.h"

#include <qt5xdg/xdgdesktopfile.h>
#include <qt5xdg/XdgIcon>

#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QCursor>

AppButton::AppButton(XdgDesktopFile* desktopFile, QWidget* parent) : QPushButton(parent), m_desktopFile(desktopFile) {
    
    setToolTip(m_desktopFile->comment());
    //Creating menu
    menu = new QMenu(parent);
    //Add menu to button
    QAction* op1 = new QAction(this);

    const QString t1("Agregar a favoritos");

    op1->setText(t1);
    //Connecting QAction with AppButton slot
    connect(op1, SIGNAL(triggered()), this, SLOT(handleMenuFavorites()));

    menu->addAction(op1);
    //setMenu(menu);

    XdgIcon::setThemeName("Razor-by-gibaalav");
    setIcon(m_desktopFile->icon(XdgIcon::defaultApplicationIcon()));
}

AppButton::AppButton(XdgDesktopFile* desktopFile, bool remove, QWidget* parent) : QPushButton(parent), m_desktopFile(desktopFile) {
    
    if (remove) {
        setToolTip(m_desktopFile->comment());
        //Creating menu
        menu = new QMenu(parent);
        //Add menu to button
        QAction* op1 = new QAction(this);

        const QString t1("Eliminar de favoritos");

        op1->setText(t1);
        //Connecting QAction with AppButton slot
        connect(op1, SIGNAL(triggered()), this, SLOT(removeFromFavorites()));

        menu->addAction(op1);

        XdgIcon::setThemeName("Razor-by-gibaalav");
        setIcon(m_desktopFile->icon(XdgIcon::defaultApplicationIcon()));
    }
}


AppButton::~AppButton() {
    delete m_desktopFile;
}

void AppButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        menu->exec(event->globalPos());
    }

    if (event->button() == Qt::LeftButton) {
        m_desktopFile->startDetached();
    }
}

void AppButton::handleMenuFavorites() {
    emit pushFavorites(m_desktopFile);
}

void AppButton::removeFromFavorites() {
    emit removeFavorites(m_desktopFile);
}


