/* 
 * File:   AppButton.h
 * Author: sigfried
 *
 * Created on September 12, 2014, 2:21 PM
 */

#ifndef APPBUTTON_H
#define	APPBUTTON_H

#include <menu-cache/menu-cache.h>

#include <QPushButton>

class AppButton : public QPushButton {
    Q_OBJECT
public:
    AppButton(MenuCacheItem* item, QWidget* parent = 0);
    virtual ~AppButton();

protected slots:
    void onClick();

private:
    MenuCacheItem* _item;

};

#endif	/* APPBUTTON_H */

