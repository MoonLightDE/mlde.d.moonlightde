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

#ifndef CONCURRENT_H
#define	CONCURRENT_H

#include <QFutureInterface>
#include <QRunnable>
namespace utils_concurrent {

    class TaskControl {
    public:

        TaskControl(QFutureInterfaceBase *futureInterface) : m_FutureInterface(futureInterface) {
        }

        bool shouldRun() const {
            return !m_FutureInterface->isCanceled();
        }
    private:
        QFutureInterfaceBase *m_FutureInterface;
    };

    template <class Tipe>
    class ControllableTask {
    public:

        virtual ~ControllableTask() {
        }
        virtual Tipe run(TaskControl& control) = 0;
    };

    template <typename T>
    class RunControllableTask : public QFutureInterface<T>, public QRunnable {
    public:

        RunControllableTask(ControllableTask<T>* tsk) : task(tsk) {
        }

        virtial ~RunControllableTask() {
            delete task;
        }

        QFuture<T> start() {
            this->setRunnable(this);
            this->reportStarted();
            QFuture<T> future = this->future();
            QThreadPool::globalInstance()->start(this, /*m_priority*/ 0);
            return future;
        }

        void run() {
            if (this->isCanceled()) {
                this->reportFinished();
                return;
            }
            TaskControl control(this);
            result = this->task->run(control);
            if (!this->isCanceled()) {
                this->reportResult(result);
            }
            this->reportFinished();
        }

        T result;
        ControllableTask<T> *task;
    };

    class TaskExecutor {
    public:

        template <class T>
        static QFuture<T> run(ControllableTask<T>* task) {
            return (new RunControllableTask<T>(task))->start();
        }

    };
}
#endif	/* CONCURRENT_H */

