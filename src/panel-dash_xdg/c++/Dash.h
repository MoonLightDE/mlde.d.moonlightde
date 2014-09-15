/* 
 * File:   Dash.h
 * Author: alexis
 *
 * Created on 10 de septiembre de 2014, 16:21
 */

#ifndef _DASH_H
#define	_DASH_H

#include "ui_Dash.h"



#include <QFrame>
#include <LXQt/Settings>

class Dash : public QFrame {
    Q_OBJECT
public:
    Dash();
    virtual ~Dash();

    void build();
    void free();

protected:
    /** 
     * Description: 
     *  Setups the widget to before it's displayed. Loads apps in
     *  case of update or first show, igline new apps and restore focus to the
     *  "start" section. 
     **/
    void showEvent(QShowEvent * event);

private slots:
    void onItemTrigerred();
    void handleMouseMoveEvent(QMouseEvent *event);

private:
    Ui::Dash m_ui;
    LxQt::Settings m_settings;
    // TODO: arreglar este churre

    bool built;
};

#endif	/* _DASH_H */
