/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
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

#ifndef NODE_H
#define	NODE_H

#include <QDate>
#include <QString>
#include <QList>


namespace model_filesystem {

    class Node {
    public:

        Node(QString uri) : m_Uri(uri) {
        }

        Node(const Node& orig) {
            m_Uri = orig.m_Uri;
        }

        virtual ~Node() {
        }

        // Basic node properties
        virtual QString name() = 0;
        virtual void setName(const QString &name) = 0;

        virtual QString iconName() = 0;
        virtual QString mimetype() = 0;

        virtual bool isValid() {
            return m_Uri.isNull();
        }

        QString uri() {
            return m_Uri;
        }

        void setUri(const QString &uri) {
            m_Uri = m_Uri;
        }

        // Navigation functions
        virtual Node* parent() = 0;

        virtual QList<Node*> children() = 0;

    protected:
        QString m_Uri;
    };


}

#endif	/* NODE_H */

