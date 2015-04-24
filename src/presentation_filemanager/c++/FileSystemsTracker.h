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

#ifndef FILESYSTEMSTRACKER_H
#define	FILESYSTEMSTRACKER_H


#include "model_filesystem/FileSystem.h"

#include <usServiceTracker.h>
#include <usServiceTrackerCustomizer.h>

#include <QPointer>

class FileManager;
class FileSystemsTracker : public us::ServiceTrackerCustomizer<model_filesystem::FileSystem> {
public:
    FileSystemsTracker(FileManager *fileManager);
    virtual ~FileSystemsTracker();

    virtual model_filesystem::FileSystem* AddingService(const us::ServiceReference<model_filesystem::FileSystem>& reference);

    virtual void ModifiedService(const us::ServiceReference<model_filesystem::FileSystem>& reference, model_filesystem::FileSystem *service);

    virtual void RemovedService(const us::ServiceReference<model_filesystem::FileSystem>& reference, model_filesystem::FileSystem *service);

private:
    QPointer<FileManager> m_FileManager;
    us::ModuleContext* m_Context;
    us::ServiceTracker<model_filesystem::FileSystem> * m_tracker;
};

#endif	/* WIDGETSTRACKER_H */

