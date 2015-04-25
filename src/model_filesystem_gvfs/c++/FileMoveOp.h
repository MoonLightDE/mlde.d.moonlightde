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

#ifndef FILEMOVEOP_H
#define	FILEMOVEOP_H

#define QT_NO_KEYWORDS
#include <gio/gio.h>

#include <QList>
#include <QString>
#include <QFuture>
#include <QRunnable>
#include <QFutureInterface>

class FileMoveOp : public QRunnable {
public:
    FileMoveOp(const QList<QString> &sources, const QString &destination);
    QFuture<void> status();
    void run();
    virtual ~FileMoveOp();
private:
    static void handleProgressCallback(goffset current_num_bytes, goffset total_num_bytes, gpointer user_data);

    QFutureInterface<void> m_FutureInterface;
    QList<QString> m_Sources;
    QString m_Destination;

    GFile * m_CurrentGFileSource;
    GFile * m_GFileDestination;

    //    GCancellable *m_Cancellable;
};

#endif	/* FILEMOVEOP_H */

