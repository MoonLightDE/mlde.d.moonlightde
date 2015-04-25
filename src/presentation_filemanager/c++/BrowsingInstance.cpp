/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *  Alexis López Zubieta
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

#include "BrowsingInstance.h"

#include "model_filesystem/Directory.h"
#include "model_filesystem/FileSystem.h"

#include <QUrl>
#include <QStandardPaths>

QList<model_filesystem::FileSystem* > *BrowsingInstance::m_AvailableFileSystems = NULL;
FileSystemsTracker *BrowsingInstance::m_Tracker = NULL;

BrowsingInstance::BrowsingInstance() : QObject(), m_CurrentDir(NULL) {
    m_CurrentPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
   
    m_HistoryPos = 0;
    m_History.prepend(m_CurrentPath);

    if (m_AvailableFileSystems == NULL)
        m_AvailableFileSystems = new QList<model_filesystem::FileSystem* >();

    if (m_Tracker == NULL)
        m_Tracker = new FileSystemsTracker();
}

QString BrowsingInstance::currentPath() {
    return m_CurrentPath;
}

model_filesystem::Directory* BrowsingInstance::currentDir() {
    if (!m_CurrentDir) {
        m_CurrentDir = getDirectory(m_CurrentPath);
    }

    return m_CurrentDir;
}

model_filesystem::Directory* BrowsingInstance::goTo(const QString &path) {
    while (m_HistoryPos > 0) {
        m_HistoryPos--;
        m_History.pop_front();
    }

    m_History.prepend(path);
    delete m_CurrentDir;

    qDebug() << m_History << m_HistoryPos;
    
    m_CurrentPath = path;
    m_CurrentDir = getDirectory(m_CurrentPath);
    Q_EMIT(directoryChanged(m_CurrentDir));
    return m_CurrentDir;
}

model_filesystem::Directory* BrowsingInstance::goBack() {
    if ((m_HistoryPos + 1) >= m_History.size())
        return m_CurrentDir;

    qDebug() << m_History << m_HistoryPos;

    m_HistoryPos++;
    m_CurrentPath = m_History.at(m_HistoryPos);

    delete m_CurrentDir;
    m_CurrentDir = getDirectory(m_CurrentPath);
    Q_EMIT(directoryChanged(m_CurrentDir));
    return m_CurrentDir;
}

model_filesystem::Directory* BrowsingInstance::goForward() {
    if (m_HistoryPos < 1)
        return m_CurrentDir;

    m_HistoryPos--;
    m_CurrentPath = m_History.at(m_HistoryPos);

    delete m_CurrentDir;
    m_CurrentDir = getDirectory(m_CurrentPath);

    Q_EMIT(directoryChanged(m_CurrentDir));
    return m_CurrentDir;
}

bool BrowsingInstance::canGoBack() {
    return m_History.size() > m_HistoryPos;
}

bool BrowsingInstance::canGoForward() {
    return m_HistoryPos >= 0;
}

QList<QString> BrowsingInstance::history() {
    return m_History;
}

BrowsingInstance::~BrowsingInstance() {
}

void BrowsingInstance::addFileSystem(model_filesystem::FileSystem* fileSystem) {
    m_AvailableFileSystems->append(fileSystem);
}

void BrowsingInstance::removeFileSystem(model_filesystem::FileSystem* fileSystem) {
    m_AvailableFileSystems->removeAll(fileSystem);
}

model_filesystem::Directory* BrowsingInstance::getDirectory(const QString &path) {
    if (m_AvailableFileSystems->isEmpty())
        return NULL;
    else {
        QUrl url = QUrl::fromUserInput(path);
        // TODO: Sanitice path

        model_filesystem::FileSystem* fileSystem = NULL;
        for (model_filesystem::FileSystem* itr : *m_AvailableFileSystems) {
            if (itr->getSupportedUriScheme().contains(url.scheme())) {
                fileSystem = itr;
                break;
            }
        }
        if (fileSystem) {
            model_filesystem::Directory * dir = fileSystem->getDirectory(url.toString());
            return dir;
        } else {
            // TODO: Report not suported scheme
        }

    }
}