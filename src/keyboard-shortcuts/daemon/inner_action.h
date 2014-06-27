/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
 * 
 * This file is part of Moonlight Desktop Environment.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INNER_ACTION_H
#define	INNER_ACTION_H

#include "base_action.h"

#include <QObject>

class InnerAction : public QObject, public BaseAction {
    Q_OBJECT
public:
    InnerAction(LogTarget *logTarget, const QString &shortcut, const QString &description);
    virtual ~InnerAction();

    static const char *id() {
        return "inner";
    }

    virtual const char *type() const {
        return id();
    }

    virtual bool call();

    QString getShortCut() const {
        return m_shortcut;
    }
signals:
    void activated();
private:
    QString m_shortcut;
};

#endif	/* LOCAL_ACTION_H */

