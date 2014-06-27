/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2013 Razor team
 * Authors:
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef GLOBAL_KEY_SHORTCUT_CLIENT__ACTION__INCLUDED
#define GLOBAL_KEY_SHORTCUT_CLIENT__ACTION__INCLUDED

#include <QtGlobal>

#include <QObject>
#include <QString>


namespace GlobalKeyShortcut {

    class IKeyboardShortCutsService;

#ifndef SHARED_EXPORT
#define SHARED_EXPORT Q_DECL_IMPORT
#endif

    class SHARED_EXPORT ShortCut : public QObject {
        Q_OBJECT
        friend IKeyboardShortCutsService;

    public:
        virtual ~ShortCut();

        virtual QString changeShortcut(const QString &shortcut) = 0;
        virtual bool changeDescription(const QString &description) = 0;

        virtual QString path() const = 0;
        virtual QString shortcut() const = 0;
        virtual QString description() const = 0;

        virtual bool isValid() const = 0;

    signals:
        void activated();
        void shortcutChanged(const QString &oldShortcut, const QString &newShortcut);

    protected:

        ShortCut(QObject *parent = 0) : QObject(parent) {
        }
    };

}

#endif // GLOBAL_KEY_SHORTCUT_CLIENT__ACTION__INCLUDED
