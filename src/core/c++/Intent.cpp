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

#include "core/Intent.h"

using namespace Core;

Intent::Intent() : m_action(QString()), m_data(QString()) {
}

Intent::Intent(const QString &action, const QString &data) : m_action(action), m_data(data) {

}

Intent::Intent(const Intent& orig) {
    m_action = orig.m_action;
    m_category = orig.m_category;
    m_component = orig.m_component;
    m_data = orig.m_data;
    m_type = orig.m_type;
    m_extras = orig.m_extras;
}

Intent::~Intent() {
}

