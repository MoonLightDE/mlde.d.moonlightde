/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
 * 
 * This file is part of Moonlight Desktop Environment Core.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CORE_H
#define	CORE_H

#include "core/ICore.h"
#include "CoreConfigurationUI.h"

#include <QString>
#include <QHash>
namespace Core {

    class Controller : public Core::IController {
        Q_OBJECT
        Q_INTERFACES(Core::IController)
    public:
        Controller(const QHash<QString, QVariant> &config);
        void start();
        void finish();
        virtual ~Controller();

    signals:
        void started();
        void finishing();

    private:
        Core::IModuleManager * moduleManager;
        QSettings * m_profile;
        QStringList m_descriptorsPaths;

        CoreConfigurationUI * m_configUi;
    };
}
#endif	/* CORE_H */

