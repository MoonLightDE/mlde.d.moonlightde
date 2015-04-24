/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
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



#include "FileSystemsTracker.h"
#include "module_config.h"
#include "FileManager.h"

#include <usModuleContext.h>
#include <usGetModuleContext.h>

#include <QDebug>

FileSystemsTracker::FileSystemsTracker(FileManager *fileManager) {
    m_Context = us::GetModuleContext();

    if (fileManager == NULL)
        qCritical() << MODULE_NAME_STR << ": invalid FileSystems service tracker initialization.";
    else {
        m_FileManager = fileManager;
        m_tracker = new us::ServiceTracker<model_filesystem::FileSystem>(m_Context, this);
        m_tracker->Open();

        qDebug() << MODULE_NAME_STR << ": FileSystems tracker up and running.";
    }
}

FileSystemsTracker::~FileSystemsTracker() {
    m_tracker->Close();
}

model_filesystem::FileSystem* FileSystemsTracker::AddingService(const us::ServiceReference<model_filesystem::FileSystem>& reference) {
    model_filesystem::FileSystem * fileSystem = m_Context->GetService(reference);

    if (m_FileManager.isNull()) {
        qCritical() << MODULE_NAME_STR << ": FileSystems service tracker not initialized.";
        return fileSystem;
    }

    m_FileManager->addFileSystem(fileSystem);
    return fileSystem;
}

void FileSystemsTracker::ModifiedService(const us::ServiceReference<model_filesystem::FileSystem>& reference, model_filesystem::FileSystem *service) {
    // do nothing
}

void FileSystemsTracker::RemovedService(const us::ServiceReference<model_filesystem::FileSystem>& reference, model_filesystem::FileSystem *service) {
    if (m_FileManager.isNull()) {
        qCritical() << MODULE_NAME_STR << ": FileSystems service tracker not initialized.";
        return;
    }
    m_FileManager->removeFileSystem(service);
}
