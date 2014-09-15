/* 
 * File:   AppButton.h
 * Author: sigfried
 *
 * Created on September 12, 2014, 2:21 PM
 */

#ifndef APPBUTTON_H
#define	APPBUTTON_H

#include <qt5xdg/XdgDesktopFile>

#include <QPushButton>

class AppButton : public QPushButton {
    Q_OBJECT
public:
    AppButton(XdgDesktopFile* item, QWidget* parent = 0);
    virtual ~AppButton();

protected slots:
    void onClick();

private:
    XdgDesktopFile* m_desktopFile;
    

};

#endif	/* APPBUTTON_H */

