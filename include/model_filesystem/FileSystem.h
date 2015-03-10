/* 
 * File:   FileSystem.h
 * Author: alexis
 *
 * Created on 3 de marzo de 2015, 10:36
 */

#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#include <QAction>
#include <QString>
#include <QList>

namespace model_filesystem {

    class Node;

    class FileSystem {
    public:

        virtual ~FileSystem() {
        }


        virtual QStringList getSupportedUriScheme() = 0;
        virtual Node getNode(QString path) = 0;
        virtual QList<QAction> getActions(QList<Node> nodes) = 0;
        
        //TODO: Query file system info
    private:

    };
}
#endif	/* FILESYSTEM_H */

