/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
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


#ifndef IACTIONSPROVIDER_H
#define	IACTIONSPROVIDER_H

#include <usServiceInterface.h>

#include <QIcon>
#include <QList>
#include <QObject>
#include <QString>

#define MAX_RANK 0xFFFF

/*! The CommandProviderItem class provides an item for use with CommandProvider.
    Items usually contain title, comment, toolTip and icon.
 */
class CommandProviderItem : public QObject {
    Q_OBJECT
public:
    CommandProviderItem() : QObject() {
    }

    virtual ~CommandProviderItem() {
    }

    virtual bool run() const = 0;
    virtual bool compare(const QRegExp &regExp) const = 0;

    /// Returns the item's icon.

    QIcon icon() const {
        return mIcon;
    }

    /// Returns the item's title.

    QString title() const {
        return mTitle;
    }

    /// Returns the item's comment.

    QString comment() const {
        return mComment;
    }

    /// Returns the item's tooltip.

    QString toolTip() const {
        return mToolTip;
    }

    /*! The result of this function is used when searching for a apropriate item.
        The item with the highest rank will be highlighted.
            0 - not suitable.
            MAX_RANK - an exact match.
        In the most cases you can yse something like:
          return stringRank(mTitle, pattern);
     */
    virtual unsigned int rank(const QString &pattern) const = 0;

protected:
    /// Helper function for the CommandProviderItem::rank
    unsigned int stringRank(const QString str, const QString pattern) const {
        int n = str.indexOf(pattern, 0, Qt::CaseInsensitive);
        if (n < 0)
            return 0;

        return MAX_RANK - ((str.length() - pattern.length()) + n * 5);
    }

    QIcon mIcon;
    QString mTitle;
    QString mComment;
    QString mToolTip;
};

/*! The CommandProvider class provides task for the lxqt-runner.
 */
class ICommandProvider : public QObject, public QList<CommandProviderItem*> {

    Q_OBJECT
public:
    ICommandProvider() : QObject(), QList<CommandProviderItem*>() {
    }

    virtual ~ICommandProvider() {
        qDeleteAll(*this);
    }
    virtual void rebuild() {};

    virtual bool isOutDated() const {
        return false;
    }

signals:
    void aboutToBeChanged();
    void changed();
};

US_DECLARE_SERVICE_INTERFACE(ICommandProvider, "org.moonlightde.core.ICommandProvider/1.0")

#endif	/* IACTIONSPROVIDER_H */

