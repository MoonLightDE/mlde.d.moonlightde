/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 * Ruben S. Garcia San Juan
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

#ifndef GRIDLAYOUTVEXPANDING_H
#define	GRIDLAYOUTVEXPANDING_H
#include <QGridLayout>

class GridLayoutVExpanding : public QGridLayout {
    Q_OBJECT
public:
    GridLayoutVExpanding(int cMaxRowApp, QWidget* parent);
    GridLayoutVExpanding(const GridLayoutVExpanding& orig);
    virtual ~GridLayoutVExpanding();
    void addWidget(QWidget* widget);
    int getCurrentRow() { return currentRow; }
private:
    int maxColumn; /* Dash max rows */
    int currentColumn;
    int currentRow;
    const QSize iconSize;
    const QSize boxSize;
};

#endif	/* GRIDLAYOUTVEXPANDING_H */

