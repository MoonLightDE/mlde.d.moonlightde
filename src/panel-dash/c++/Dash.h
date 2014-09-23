/* 
 * File:   Dash.h
 * Author: alexis
 *
 * Created on 10 de septiembre de 2014, 16:21
 */

#ifndef _DASH_H
#define	_DASH_H

#include "ui_Dash.h"
#include <menu-cache/menu-cache.h>
#include <QFrame>

class Dash : public QFrame {
    Q_OBJECT
public:
    Dash(MenuCache* menuCache);
    virtual ~Dash();

    void build(MenuCache* menuCache);
    void free();



private:
    void addMenuItems(QMenu* menu, MenuCacheDir* dir);
    void handleMouseMoveEvent(QMouseEvent *event);

private slots:
    void onItemTrigerred();
    
private:
    Ui::Dash widget;
};

#endif	/* _DASH_H */
