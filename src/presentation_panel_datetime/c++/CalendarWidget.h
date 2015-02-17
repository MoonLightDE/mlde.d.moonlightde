/* 
 * File:   CalendarWidget.h
 * Author: alexis
 *
 * Created on 30 de junio de 2014, 17:58
 */

#ifndef _CALENDARWIDGET_H
#define	_CALENDARWIDGET_H

#include "ui_CalendarWidget.h"

class CalendarWidget : public QWidget {
    Q_OBJECT
public:
    CalendarWidget();
    virtual ~CalendarWidget();
private:
    Ui::CalendarWidget widget;
};

#endif	/* _CALENDARWIDGET_H */
