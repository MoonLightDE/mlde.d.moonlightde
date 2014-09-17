/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *  Ruben S. Garcia San Juan
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
#include "GridLayoutVExpanding.h"

GridLayoutVExpanding::GridLayoutVExpanding(int cMaxRowApps, QWidget* parent) : QGridLayout(parent) {
    maxColumn = cMaxRowApps;
    currentColumn = 0;
    currentRow = 0;
}

GridLayoutVExpanding::GridLayoutVExpanding(const GridLayoutVExpanding& orig) {
}

GridLayoutVExpanding::~GridLayoutVExpanding() {
}

void GridLayoutVExpanding::addWidget(QWidget* widget) {

    if (currentColumn == maxColumn) {
        currentColumn = 0;
        currentRow++;
    }
    
    QGridLayout::addWidget(widget, currentRow, currentColumn, Qt::AlignHCenter);
    currentColumn++;


}