/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 * Alexis López Zubieta
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
#include <QDebug>

#include "meta_types.h"
#include "core.h"

#include "keyboard-shortcuts/IKeyboardShortCutsService.h"
#include "keyboard-shortcuts/shortcut.h"

#include <usModuleActivator.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include <QString>
#include <QPointer>
#include <QStringList>
#include <qt4/QtCore/qobjectdefs.h>

US_USE_NAMESPACE

#define DEFAULT_CONFIG ".config/LXQt/globalkeyshortcuts.conf"

        namespace GlobalKeyShortcut {

    class ShortCutImpl : public ShortCut {
        friend KeyboardShortCutsService;
    public:

        ~ShortCutImpl() {
            bool result;
            m_core->removeAction(result, m_id);
            qDebug() << "Was the keyboard shortcut removed? " << result;
            delete m_action;
        }

        QString changeShortcut(const QString &shortcut) {
            QString result;
            QString prev = shortcut;
            m_core->changeShortcut(result, m_id, shortcut);
            if (result != prev)
                emit shortcutChanged(prev, result);
        }

        bool changeDescription(const QString &description) {
            m_action->setDescription(description);
        }

        QString path() const {
            return m_path;
        }

        QString shortcut() const {
            return m_action->getShortCut();
        }

        QString description() const {
            return m_action->description();
        }

        bool isValid() const {
            return true;
        }


    protected:

        ShortCutImpl(qlonglong id, const QString &path, InnerAction *action, Core * core, QObject *parent = 0) {
            m_core = core;
            m_id = id;
            m_action = action;
            m_path = path;
            connect(action, SIGNAL(activated()), this, SIGNAL(activated()));
        }
    private:



        QPointer<Core> m_core;
        InnerAction *m_action;
        qlonglong m_id;
        QString m_path;
        QString m_shortcut;
    };

    class KeyboardShortCutsService : public IKeyboardShortCutsService {
    public:

        KeyboardShortCutsService() : ready(false) {
            qDebug() << "Shortcuts-Service Startting";

            bool wrongArgs = false;
            bool printHelp = false;
            bool runAsDaemon = true;
            bool useSyslog = false;
            bool minLogLevelSet = false;
            int minLogLevel = LOG_NOTICE;
            bool multipleActionsBehaviourSet = false;
            MultipleActionsBehaviour multipleActionsBehaviour = MULTIPLE_ACTIONS_BEHAVIOUR_FIRST;
            QStringList configFiles;

            configFiles.push_back(QString::fromLocal8Bit(getenv("HOME")) + "/" DEFAULT_CONFIG);

            m_core = new Core(runAsDaemon || useSyslog, minLogLevelSet, minLogLevel, configFiles, multipleActionsBehaviourSet, multipleActionsBehaviour);

            if (!m_core->ready()) {
                qCritical() << "Unable to start Shortcuts-Service";
            } else {
                ready = true;
            }
        }

        virtual ~KeyboardShortCutsService() {
        }

        bool isReady() {
            return ready;
        }

        virtual ShortCut *addShortCut(const QString &shortcut, const QString &path, const QString &description, QObject *parent = 0) {
            if (!ready) {
                qWarning() << "Failed to \"addShortCut\", the daemon is not ready.";
                return NULL;
            }

            QPair<QString, qulonglong> result;
            InnerAction * action = m_core->addInnerAction(result, shortcut, description);
            if (!action) {
                qDebug() << "Failed to create shortcuts, it may be in use by another application.";
                return NULL;
            }

            ShortCutImpl * shortcutImpl = new ShortCutImpl(result.second, path, action, m_core, parent);
            return shortcutImpl;
        }

        //    virtual bool removeShortCut(const QString &path) = 0;

    private:
        QPointer<Core> m_core;
        bool ready;
    };
}

/**
 */
class Activator : public ModuleActivator {
private:

    /**
     * Implements ModuleActivator::Load().
     *
     * @param context the framework context for the module.
     */
    void Load(ModuleContext* context) {



        m_service = new GlobalKeyShortcut::KeyboardShortCutsService();

        if (m_service->isReady()) {
            ServiceProperties props;
            context->RegisterService<GlobalKeyShortcut::IKeyboardShortCutsService>(m_service, props);
        } else {
            delete m_service;
        }
    }

    /**
     * Implements ModuleActivator::Unload().
     *
     * @param context the framework context for the module.
     */
    void Unload(ModuleContext* context) {
        delete m_service;
    }

    GlobalKeyShortcut::KeyboardShortCutsService * m_service;
};
US_EXPORT_MODULE_ACTIVATOR(shortcuts_services, Activator)
