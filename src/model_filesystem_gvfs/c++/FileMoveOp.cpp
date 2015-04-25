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

#include "FileMoveOp.h"

FileMoveOp::FileMoveOp(const QList<QString> &sources, const QString &destination) : QRunnable(), m_Sources(sources), m_Destination(destination) {
    m_GFileDestination = g_file_new_for_uri(destination.toLocal8Bit());
}

QFuture<void> FileMoveOp::status() {
    return m_FutureInterface.future();
}

void FileMoveOp::run() {
    if (m_Sources.isEmpty()) {
        m_FutureInterface.reportFinished();
    } else {
        //        m_Cancellable = g_cancellable_new();
        //        g_cancellable_connect(m_Cancellable, )

        m_FutureInterface.setProgressRange(0, m_Sources.size());
        int progress = 0;
        while (!m_Sources.isEmpty() && !m_FutureInterface.isCanceled()) {
            GError *error = NULL;
            QString current = m_Sources.first();
            m_Sources.pop_front();

            m_CurrentGFileSource = g_file_new_for_uri(current.toLocal8Bit());
            m_FutureInterface.setProgressValueAndText(++progress, current);
            if (!g_file_move(m_CurrentGFileSource, m_GFileDestination, G_FILE_COPY_NONE, NULL, handleProgressCallback, NULL, &error)) {
                qDebug() << error->message;
                m_FutureInterface.setProgressValueAndText(progress, QString::fromLocal8Bit(error->message));
                g_error_free(error);
            }
        }
    }
    m_FutureInterface.reportFinished();
}

FileMoveOp::~FileMoveOp() {
}

void FileMoveOp::handleProgressCallback(goffset current_num_bytes, goffset total_num_bytes, gpointer user_data) {
    //    FileMoveOp * op = static_cast<FileMoveOp*> (user_data);
    //    if (op == NULL)
    //        return;

    //    if (op->m_FutureInterface.isCanceled()) {
    //        g_cancellable_cancel(op->m_Cancellable);
    //    }

    //    (0, total_num_bytes);
    //    op->m_FutureInterface.setProgressValue(current_num_bytes);
}
