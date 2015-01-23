/* 
 * File:   OpenWindows.h
 * Author: alexis
 *
 * Created on 23 de enero de 2015, 11:15
 */

#include <QWidget>

#ifndef OPENWINDOWS_H
#define	OPENWINDOWS_H
namespace presentation {
    namespace panel {
        namespace area {

            class OpenWindows : public QWidget {
            public:
                OpenWindows();
                virtual ~OpenWindows();
            
            public slots:
                addWindowItem(windowId WId);
                removeWindowItem(windowId WId);
            private:

            };
        }
    }
}
#endif	/* OPENWINDOWS_H */

