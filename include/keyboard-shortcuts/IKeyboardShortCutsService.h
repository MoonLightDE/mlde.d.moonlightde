/* 
 * File:   IGlobalKeyboardShortCuts.h
 * Author: alexis
 *
 * Created on 27 de junio de 2014, 12:39
 */

#ifndef IKEYBOARDSHORTCUTSSERVICE_H
#define	IKEYBOARDSHORTCUTSSERVICE_H

#include "keyboard-shortcuts/shortcut.h"

#include <QString>
#include <QObject>

#include <usServiceInterface.h>

namespace GlobalKeyShortcut {

    class IKeyboardShortCutsService {
    public:
        virtual ShortCut *addShortCut(const QString &shortcut, const QString &path, const QString &description, QObject *parent = 0) = 0;
//        virtual bool removeShortCut(const QString &path) = 0;
//
//        public
//slots:
//        virtual void grabShortcut(uint timeout) = 0;
//        virtual void cancelShortcutGrab() = 0;
//
//signals:
//        virtual void shortcutGrabbed(const QString &) = 0;
//        virtual void grabShortcutFailed() = 0;
//        virtual void grabShortcutCancelled() = 0;
//        virtual void grabShortcutTimedout() = 0;
    };

}

US_DECLARE_SERVICE_INTERFACE(GlobalKeyShortcut::IKeyboardShortCutsService, "org.moonlightde.keyboardshortcuts.IService/1.0")
#endif	/* IKEYBOARDSHORTCUTSSERVICE_H */

