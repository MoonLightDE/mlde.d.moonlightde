/* 
 * File:   DesktopFileCollection.h
 * Author: akiel
 *
 * Created on October 3, 2014, 3:10 PM
 */

#ifndef DESKTOPFILECOLLECTION_H
#define	DESKTOPFILECOLLECTION_H

#include "QList"
#include <qt5xdg/XdgDesktopFile>

class DesktopFileCollection {
public:
    DesktopFileCollection();
    virtual ~DesktopFileCollection();
    //esto debe permitir filtrar dependiendo del campo en que se busque
    //utilizar el Hash sección, filtro
    QList<XdgDesktopFile*> filter(QHash<QString,QString>);
    QList<XdgDesktopFile*> all();
private:

};

#endif	/* DESKTOPFILECOLLECTION_H */

