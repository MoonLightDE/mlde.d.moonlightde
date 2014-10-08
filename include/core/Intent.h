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

#ifndef INTENT_H
#define	INTENT_H

#include <QString>
#include <QList>
#include <QPair>
namespace Core {

    class Intent {
    public:

        Intent();
        /*!Some examples of action/data pairs are:
         * ACTION_OPEN http://www,host.com -- Open the given URI in a web browser.
         * ACTION_COMPOSE mailto://contact@moonlightde.org -- Opens the mail composer 
         *  with the given address.
         * ACTION_PLAY file:///home/user/music/song.mp3 -- Reproduce the given song.
         */
        Intent(const QString &action, const QString &data);

        Intent(const Intent& orig);
        virtual ~Intent();

    public:

        QString GetAction() const {
            return m_action;
        }

        QString GetCategory() const {
            return m_category;
        }

        QString GetComponent() const {
            return m_component;
        }

        QString GetData() const {
            return m_data;
        }

        QList<QPair<QString, QString> > GetExtras() const {
            return m_extras;
        }

        QString GetType() const {
            return m_type;
        }

        void SetAction(QString action) {
            m_action = action;
        }

        void SetCategory(QString category) {
            m_category = category;
        }

        void SetComponent(QString component) {
            m_component = component;
        }

        void SetData(QString data) {
            m_data = data;
        }

        void addExtra(QPair<QString, QString> extra) {
            m_extras.append(extra);
        }

        void SetType(QString type) {
            m_type = type;
        }


    private:
        /* The general action to be performed, such as: 
         *  ACTION_VIEW, ACTION_SHOW, etc                                           */
        QString m_action;
        
        /* The data to operate on, such as a file expressed as a Uri.               */
        QString m_data;

        /* Gives additional information about the action to execute. For example, 
         * CATEGORY_LAUNCHER means it should appear in the Launcher as a top-level 
         * application, while CATEGORY_ALTERNATIVE means it should be included in 
         * a list of alternative actions the user can perform on a piece of data.
         */
        QString m_category;
        
        /* Specifies an explicit type (a MIME type) of the intent data. Normally
         * the type is inferred from the data itself. By setting this attribute, 
         * you disable that evaluation and force an explicit type.                  */
        QString m_type;
        
        /* Specifies an explicit name of a component class to use for the intent. 
         * Normally this is determined by looking at the other information in the 
         * intent (the action, data/type, and categories) and matching that with a 
         * component that can handle it. If this attribute is set then none of the 
         * evaluation is performed, and this component is used exactly as is. By 
         * specifying this attribute, all of the other Intent attributes become 
         * optional.                                                                 */
        QString m_component;
        
        /*  This can be used to provide extended information to the component. For 
         * example, if we have a action to send an e-mail message, we could also
         * include extra pieces of data here to supply a subject, body, etc.        */
        QList<QPair<QString, QString> > m_extras;
    };
}

#endif	/* INTENT_H */

