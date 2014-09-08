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


#include "CoreConfigurationUI.h"

CoreConfigurationUI::CoreConfigurationUI() : m_treemodel(new ModuleModel(this)) {
    widget.setupUi(this);

    widget.treeView->setModel(m_treemodel);
    connect(widget.controlButton, SIGNAL(released()), this, SLOT(onClickedControlButon()));
}

CoreConfigurationUI::~CoreConfigurationUI() {
    delete m_treemodel;
}

void CoreConfigurationUI::onClickedControlButon() {
    QModelIndex index = widget.treeView->currentIndex();
    m_treemodel->toggleModule(index.row());
}