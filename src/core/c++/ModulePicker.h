/* 
 * File:   ModulePicker.h
 * Author: alexis
 *
 * Created on 7 de octubre de 2014, 21:26
 */

#ifndef _MODULEPICKER_H
#define	_MODULEPICKER_H

#include "ui_ModulePicker.h"
#include "core/Intent.h"

#include <QStringList>

using namespace Core;

class ModulePicker : public QWidget {
    Q_OBJECT
public:
    ModulePicker(const QStringList &moduleNames, Intent intent);
    virtual ~ModulePicker();

public slots:
    void itemSelected(QListWidgetItem * item);
    void cancelClicked();
private:
    Ui::ModulePicker widget;
    Intent m_intent;
    
};

#endif	/* _MODULEPICKER_H */
