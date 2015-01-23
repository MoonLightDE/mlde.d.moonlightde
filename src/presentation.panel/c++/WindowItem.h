/* 
 * File:   WindowItem.h
 * Author: alexis
 *
 * Created on 23 de enero de 2015, 11:20
 */

#ifndef WINDOWITEM_H
#define	WINDOWITEM_H
#include <QFrame>

class WindowItem {
public:
    WindowItem(WId windowId);
    virtual ~WindowItem();

    raise();
    maximize();
    minimize();
    restore();
    close();
    moveToDesktop(int desktopId);
    showMenu();

    WId window() {
        return m_windowId
    }

private:
    WId m_windowId;

};

#endif	/* WINDOWITEM_H */

