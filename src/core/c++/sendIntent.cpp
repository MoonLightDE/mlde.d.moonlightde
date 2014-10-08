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
 * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ModulePicker.h"

#include "core/sendIntent.h"
#include "core/Intent.h"
#include "core/IIntentFilter.h"

#include <usGetModuleContext.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>
#include <QStringList>

#include <QDebug>
#include <QStringList>
#include <CppMicroServices/usModule.h>

using namespace Core;

void Core::sendIntent(Intent intent) {
    us::ModuleContext *context = us::GetModuleContext();

    QStringList pattern;
    pattern << "(&";
    if (intent.GetComponent().isNull()) {

        pattern << "(Action=" << intent.GetAction() << ")";
        if (!intent.GetType().isNull())
            pattern << "(Type=" << intent.GetType() << ")";
        if (!intent.GetCategory().isNull())
            pattern << "(Category=" << intent.GetCategory() << ")";
    } else {
        pattern << "(Component=" << intent.GetComponent() << ")";
    }
    pattern << ")";
    qDebug() << pattern;


    std::vector<us::ServiceReference<IIntentFilter> > refs =
            context->GetServiceReferences<IIntentFilter>(pattern.join("").toStdString());

    if (refs.size() > 0) {
        IIntentFilter * filter = NULL;
        if (refs.size() == 1) {
            filter = context->GetService<IIntentFilter>(refs.front());
            filter->filter(intent);
        } else {
            QStringList components;
            for (std::vector<us::ServiceReference<IIntentFilter> >::const_iterator ref = refs.begin();
                    (ref != refs.end()); ++ref) {
                components << QString::fromStdString(ref->GetProperty("Component").ToString());
            }
            ModulePicker *picker = new ModulePicker(components, intent);
            picker->show();
        }

    }
}
