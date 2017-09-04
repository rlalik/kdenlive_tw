/***************************************************************************
                          titlewidget.cpp  -  description
                             -------------------
    begin                : Feb 28 2008
    copyright            : (C) 2008 by Marco Gittler
    email                : g.marco@freenet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "typewriterwidget.h"
#include "typewriterdocument.h"

#include <cmath>

#include <KMessageBox>
#include <klocalizedstring.h>
#include <KRecentDirs>

#include "kdenlive_debug.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QDomDocument>
#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include <QTimer>
#include <QToolBar>
#include <QSignalMapper>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QCryptographicHash>
#include <QKeyEvent>
#include <QImageReader>

#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>

#include <iostream>
#include <QStandardPaths>

static QList<TitleTemplate> titletemplates;

// What exactly is this variable good for?
const int IMAGEITEM = 7;
const int RECTITEM = 3;
const int TEXTITEM = 8;

const int NOEFFECT = 0;
const int BLUREFFECT = 1;
const int SHADOWEFFECT = 2;
const int TYPEWRITEREFFECT = 3;

TypeWriterWidget::TypeWriterWidget(const QUrl &url, const Timecode &tc, const QString &projectPath, Render *render, QWidget *parent) :
    TitleWidget(url, tc, projectPath, render, parent),
    m_TWTabWidget(new QWidget(this)),
    m_TWTabUi(new Ui::TypeWriterTabWidget_UI()),
    m_projectTitlePath(projectPath)
{
    m_TWTabUi->setupUi(m_TWTabWidget);
    m_TWTabWidget->show();

    tabWidget->addTab(m_TWTabWidget, i18n("Patterns"));

    connect(m_TWTabUi->pattern_add, SIGNAL(clicked()), this, SLOT(slotAddPattern()));
    connect(m_TWTabUi->pattern_delete, SIGNAL(clicked()), this, SLOT(slotRemovePattern()));

    connect(m_TWTabUi->list_patterns, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(slotEditPattern(QListWidgetItem *)));

    m_TWPatternUI = new Ui::TypeWriterPatternDialog_UI();
    m_TWPatternDialog = new QDialog();
    m_TWPatternUI->setupUi(m_TWPatternDialog);

    connect(m_TWPatternUI->length, SIGNAL(valueChanged(int)), m_TWPatternUI->fill, SLOT(setEnabled(int)));

    m_TWPatternUI->length->setValue(0);
    document();
}

TypeWriterWidget::~TypeWriterWidget()
{
    delete m_TWTabWidget;
    delete m_TWTabUi;

    delete m_TWPatternDialog;
    delete m_TWPatternUI;
}

TypeWriterDocument & TypeWriterWidget::document()
{
    TitleDocument tdoc = TitleWidget::document();
    m_typewriterdocument = tdoc;
    m_typewriterdocument.setPatternsList(m_TWTabUi->list_patterns);
    return m_typewriterdocument;
}

QDomDocument TypeWriterWidget::xml()
{
    QDomDocument doc_tw = m_typewriterdocument.xml(nullptr, nullptr);
    return doc_tw;
}

void TypeWriterWidget::slotAddPattern()
{
    m_TWPatternUI->name->setText("");
    m_TWPatternUI->text->setPlainText("");
    m_TWPatternUI->length->setValue(0);
    m_TWPatternUI->fill->setText("");

    if (m_TWPatternDialog->exec() == QDialog::Accepted)
    {
        QString s_name = m_TWPatternUI->name->text();
        QString s_text = m_TWPatternUI->text->toPlainText();
        int n_length = m_TWPatternUI->length->value();
        QString s_fill = m_TWPatternUI->fill->text();

        if (s_name.length())
        {
            QListWidgetItem * item = new QListWidgetItem(s_name);
            item->setData(Qt::UserRole+0, s_text);
            item->setData(Qt::UserRole+1, n_length);
            item->setData(Qt::UserRole+2, s_fill);

            m_TWTabUi->list_patterns->addItem(item);
        }
    }
}

void TypeWriterWidget::slotEditPattern(QListWidgetItem* item)
{
    m_TWPatternUI->name->setText(item->text());
    m_TWPatternUI->text->setPlainText(item->data(Qt::UserRole+0).toString());
    m_TWPatternUI->length->setValue(item->data(Qt::UserRole+1).toInt());
    m_TWPatternUI->fill->setText(item->data(Qt::UserRole+2).toString());

    if (m_TWPatternDialog->exec() == QDialog::Accepted)
    {
        QString s_name = m_TWPatternUI->name->text();
        QString s_text = m_TWPatternUI->text->toPlainText();
        int n_length = m_TWPatternUI->length->value();
        QString s_fill = m_TWPatternUI->fill->text();

        if (s_name.length())
        {
            item->setText(s_name);
            item->setData(Qt::UserRole+0, s_text);
            item->setData(Qt::UserRole+1, n_length);
            item->setData(Qt::UserRole+2, s_fill);
        }
    }
}

void TypeWriterWidget::slotRemovePattern()
{
    QListWidgetItem * item = m_TWTabUi->list_patterns->currentItem();
    if (item)
    {
        m_TWTabUi->list_patterns->removeItemWidget(item);
        delete item;
    }
}
