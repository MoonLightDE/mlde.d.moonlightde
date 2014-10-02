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

#ifndef GRIDLAYOUTHEXPANDING_H
#define	GRIDLAYOUTHEXPANDING_H

#include <QGridLayout>

class GridLayoutHExpanding : public QGridLayout {
    Q_OBJECT
public:
    GridLayoutHExpanding(int cMaxColumnsApp, QWidget* parent);
    GridLayoutHExpanding(const GridLayoutHExpanding& orig);
    virtual ~GridLayoutHExpanding();
    void addWidget(QWidget* widget);
    void addLayout(QLayout* layout);
private:
    int maxRow; /* Dash max columns */
    int currentRow; // Actual row position from App
    int currentColumn;
};



#endif	/* GRIDLAYOUTHEXPANDING_H */

