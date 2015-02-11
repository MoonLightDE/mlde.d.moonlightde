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
    AppButton(XdgDesktopFile* item, bool remove, QWidget* parent = 0);
    virtual ~AppButton();
    void mouseReleaseEvent(QMouseEvent* event);

protected slots:
    void addFavoritesAction();
    void rightClickAction();
    void handleMenuFavorites();
    void removeFromFavorites();

signals:
    void pushFavorites(XdgDesktopFile* app);
    void removeFavorites(XdgDesktopFile* app);
    
private:
    XdgDesktopFile* m_desktopFile;
    QMenu* menu;
    //Optional attribute to check if the application is already in favorites
    //bool isFavorite;
};

#endif	/* APPBUTTON_H */

