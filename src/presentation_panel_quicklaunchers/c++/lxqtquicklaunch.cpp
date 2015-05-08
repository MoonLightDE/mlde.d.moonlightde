/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2012 Razor team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "lxqtquicklaunch.h"
#include "quicklaunchbutton.h"
#include "quicklaunchaction.h"

#include "core/ModuleSettings.h"

#include <LXQt/lxqtgridlayout.h>

#include <qt5xdg/xdgdesktopfile.h>
#include <qt5xdg/xdgicon.h>
//#include <qt5xdg/xdgmime.h>

#include <usModuleContext.h>
#include <usGetModuleContext.h>
#include <usServiceReference.h>
#include <usServiceProperties.h>

#include <QtDebug>
#include <QSettings>
#include <QUrl>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QToolButton>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileIconProvider>
#include <QSettings>
#include <QLabel>
#include <QMimeDatabase>

US_USE_NAMESPACE

LxQtQuickLaunch::LxQtQuickLaunch(QWidget* parent) :
QFrame(parent),
mPlaceHolder(0) {
    setAcceptDrops(true);
    //    setMinimumWidth(100);
    //    setMaximumWidth(500);
    mLayout = new LxQt::GridLayout(this);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setLayout(mLayout);


    ModuleContext * context = GetModuleContext();
    QSettings *settings = ModuleSettings::getModuleSettings(context);

    int count = settings->beginReadArray("apps");

    QString desktop;
    QString file;
    QString execname;
    QString exec;
    QString icon;
    for (int i = 0; i < count; ++i) {
        settings->setArrayIndex(i);
        desktop = settings->value("desktop", "").toString();
        file = settings->value("file", "").toString();

        if (!desktop.isEmpty()) {
            XdgDesktopFile * xdg = XdgDesktopFileCache::getFile(desktop);
            if (!xdg->isValid()) {
                qDebug() << "XdgDesktopFile" << desktop << "is not valid";
                continue;
            }
            if (!xdg->isApplicable()) {
                qDebug() << "XdgDesktopFile" << desktop << "is not applicable";
                continue;
            }

            addButton(new QuickLaunchAction(xdg, this));
        } else if (!file.isEmpty()) {
            //            qDebug() << "quiqlaunchers test " << desktop << " --- " << file;
            addButton(new QuickLaunchAction(file, this));
        } else {
            execname = settings->value("name", "").toString();
            exec = settings->value("exec", "").toString();
            icon = settings->value("icon", "").toString();
            if (icon.isNull()) {
                qDebug() << "Icon" << icon << "is not valid (isNull). Skipped.";
                continue;
            }

            addButton(new QuickLaunchAction(execname, exec, icon, this));
        }
    } // for

    settings->endArray();

    if (mLayout->isEmpty())
        showPlaceHolder();

    realign();
}

LxQtQuickLaunch::~LxQtQuickLaunch() {
}

int LxQtQuickLaunch::indexOfButton(QuickLaunchButton* button) const {
    return mLayout->indexOf(button);
}

int LxQtQuickLaunch::countOfButtons() const {
    return mLayout->count();
}

void LxQtQuickLaunch::realign() {
    mLayout->setEnabled(false);
    //ILxQtPanel *panel = mPlugin->panel();

    if (mPlaceHolder) {
        mLayout->setColumnCount(1);
        mLayout->setRowCount(1);
    } else {
        mLayout->setColumnCount(countOfButtons());
        //        if (panel->isHorizontal()) {
        //            mLayout->setRowCount(panel->lineCount());
        //            mLayout->setColumnCount(0);
        //        } else {
        //            mLayout->setColumnCount(panel->lineCount());
        //            mLayout->setRowCount(0);
        //        }
    }
    mLayout->setEnabled(true);
}

void LxQtQuickLaunch::addButton(QuickLaunchAction* action) {
    mLayout->setEnabled(false);
    QuickLaunchButton* btn = new QuickLaunchButton(action, this);
    mLayout->addWidget(btn);

    connect(btn, SIGNAL(switchButtons(QuickLaunchButton*, QuickLaunchButton*)), this, SLOT(switchButtons(QuickLaunchButton*, QuickLaunchButton*)));
    connect(btn, SIGNAL(buttonDeleted()), this, SLOT(buttonDeleted()));
    connect(btn, SIGNAL(movedLeft()), this, SLOT(buttonMoveLeft()));
    connect(btn, SIGNAL(movedRight()), this, SLOT(buttonMoveRight()));

    mLayout->removeWidget(mPlaceHolder);
    delete mPlaceHolder;
    mPlaceHolder = 0;
    mLayout->setEnabled(true);
    realign();
}

void LxQtQuickLaunch::dragEnterEvent(QDragEnterEvent *e) {
    // Getting URL from mainmenu...
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
        return;
    }

    if (e->source() && e->source()->parent() == this) {
        e->acceptProposedAction();
    }
}

void LxQtQuickLaunch::dropEvent(QDropEvent *e) {
    const QMimeData *mime = e->mimeData();
    // duplicates storage: [quicklaunch] issue dragging from qtfm, #252
    QList<QUrl> duplicates;
    // urls from mainmenu

    foreach(QUrl url, mime->urls()) {
        if (duplicates.contains(url))
            continue;
        else
            duplicates << url;

        QString filePath;
        if (url.isLocalFile())
            filePath = url.toLocalFile();
        else
            filePath = url.toString();
        //        qDebug() << "Dropped URL "<< filePath;

        QMimeDatabase database;
        QString type = database.mimeTypeForFile(filePath).name();
        qDebug() << "Dropped file type " << type;
        QFileInfo fi(filePath);
        XdgDesktopFile * xdg = NULL;
        if (type == "application/x-desktop") {
            xdg = XdgDesktopFileCache::getFile(fi.filePath());
        }

        //        qDebug() << "Dropped app" << xdg->fileName() << " filename "<< fi.fileName() << " isvalid "<< xdg->isValid() << "is applicable" << xdg->isApplicable();

        if (xdg != NULL && xdg->isValid()) {
            if (xdg->isApplicable())
                addButton(new QuickLaunchAction(xdg, this));
        } else if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
            //            qDebug() << "Dropped pure app" << fi.filePath();
            addButton(new QuickLaunchAction(filePath, filePath, "", this));
        } else if (fi.exists()) {
            qDebug() << "Dropped file" << fi.filePath();
            addButton(new QuickLaunchAction(filePath, this));
        } else {
            qWarning() << "XdgDesktopFile" << filePath << "is not valid";
            //            QMessageBox::information(this, tr("Drop Error"),
            //                    tr("File/URL '%1' cannot be embedded into QuickLaunch for now").arg(filePath)
            //                    );
        }
    }
    saveSettings();
}

void LxQtQuickLaunch::switchButtons(QuickLaunchButton *button1, QuickLaunchButton *button2) {
    if (button1 == button2)
        return;

    int n1 = mLayout->indexOf(button1);
    int n2 = mLayout->indexOf(button2);

    int l = qMin(n1, n2);
    int m = qMax(n1, n2);

    mLayout->moveItem(l, m);
    mLayout->moveItem(m - 1, l);
    saveSettings();
}

void LxQtQuickLaunch::buttonDeleted() {
    QuickLaunchButton *btn = qobject_cast<QuickLaunchButton*>(sender());
    if (!btn)
        return;

    mLayout->removeWidget(btn);
    btn->deleteLater();
    saveSettings();

    if (mLayout->isEmpty())
        showPlaceHolder();

    realign();
}

void LxQtQuickLaunch::buttonMoveLeft() {
    QuickLaunchButton *btn = qobject_cast<QuickLaunchButton*>(sender());
    if (!btn)
        return;

    int index = indexOfButton(btn);
    if (index > 0) {
        mLayout->moveItem(index, index - 1);
        saveSettings();
    }
}

void LxQtQuickLaunch::buttonMoveRight() {
    QuickLaunchButton *btn1 = qobject_cast<QuickLaunchButton*>(sender());
    if (!btn1)
        return;

    int index = indexOfButton(btn1);
    if (index < countOfButtons() - 1) {
        mLayout->moveItem(index, index + 1);
        saveSettings();
    }
}

void LxQtQuickLaunch::saveSettings() {

    ModuleContext * context = GetModuleContext();

    QSettings * settings = ModuleSettings::getModuleSettings(context);


    settings->remove("apps");
    settings->beginWriteArray("apps");
    int i = 0;

    for (int j = 0; j < mLayout->count(); ++j) {
        QuickLaunchButton *b = qobject_cast<QuickLaunchButton*>(mLayout->itemAt(j)->widget());
        if (!b)
            continue;

        settings->setArrayIndex(i);

        QHashIterator<QString, QString> it(b->settingsMap());
        while (it.hasNext()) {
            it.next();
            settings->setValue(it.key(), it.value());
        }

        ++i;
    }

    settings->endArray();
    delete settings;
}

void LxQtQuickLaunch::showPlaceHolder() {
    if (!mPlaceHolder) {
        mPlaceHolder = new QLabel(this);
        mPlaceHolder->setAlignment(Qt::AlignCenter);
        mPlaceHolder->setObjectName("QuickLaunchPlaceHolder");
        mPlaceHolder->setText(tr("Drop application\nicons here"));
    }

    mLayout->addWidget(mPlaceHolder);
}
