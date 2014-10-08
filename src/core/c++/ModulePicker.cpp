/*
 * File:   ModulePicker.cpp
 * Author: alexis
 *
 * Created on 7 de octubre de 2014, 21:26
 */

#include "ModulePicker.h"

#include "core/sendIntent.h"
#include "core/Intent.h"

#include <QListWidget>
#include <QListWidgetItem>

using namespace Core;

ModulePicker::ModulePicker(const QStringList &moduleNames, Intent intent) : m_intent(intent) {
    widget.setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    foreach(QString moduleName, moduleNames) {
        // TODO: Add icon and a detailed module name.
        widget.list->addItem(moduleName);
    }
    connect(widget.list, &QListWidget::itemDoubleClicked, this, &ModulePicker::itemSelected);
}

ModulePicker::~ModulePicker() {
}

void ModulePicker::itemSelected(QListWidgetItem * item) {
    m_intent.SetComponent(item->text());
    sendIntent(m_intent);
    close();
}

void ModulePicker::cancelClicked() {
    hide();
    deleteLater();
}