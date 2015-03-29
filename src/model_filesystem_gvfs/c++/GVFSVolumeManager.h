/* 
 * File:   GVFSVolumeManager.h
 * Author: alexis
 *
 * Created on 28 de marzo de 2015, 2:49
 */

#ifndef GVFSVOLUMEMANAGER_H
#define	GVFSVOLUMEMANAGER_H

#include <QObject>

class GVFSVolumeManager : public QObject {
    Q_OBJECT
public:
    GVFSVolumeManager();
    GVFSVolumeManager(const GVFSVolumeManager& orig);
    virtual ~GVFSVolumeManager();

    
    
private:

};

#endif	/* GVFSVOLUMEMANAGER_H */

