/***************************************************************************
                          typewriterwidget.h  -  description
                             -------------------
    begin                : Feb 28 2008
    copyright            : (C) 2008 by Rafał Lalik
    email                : rafallalik@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TYPEWRITERWIDGET_H
#define TYPEWRITERWIDGET_H

#include "ui_typewritertabwidget_ui.h"
#include "ui_typewriterpatternwidget_ui.h"
#include "titler/titlewidget.h"
#include "typewriterdocument.h"

/*! \class TypeWriterWidget
    \brief Title creation dialog
    Instances of TypeWriterWidget classes are instansiated by KdenliveDoc::slotCreateTextClip ()
*/
class TypeWriterWidget : public TitleWidget
{
    Q_OBJECT

public:

    /** @brief Draws the dialog and loads a title document (if any).
     * @param url title document to load
     * @param tc timecode of the project
     * @param projectPath default path to save to or load from title documents
     * @param render project renderer
     * @param parent (optional) parent widget */
    explicit TypeWriterWidget(const QUrl &url, const Timecode &tc, const QString &projectPath, Render *render, QWidget *parent = nullptr);
    virtual ~TypeWriterWidget();

    QDomDocument xml();

protected:
    virtual TypeWriterDocument & document();

private slots:
    void slotAddPattern();
    void slotRemovePattern();
    void slotEditPattern(QListWidgetItem * item);
//     void slotAddPatternLengthChanged(int);

private:
    QWidget * m_TWTabWidget;
    Ui::TypeWriterTabWidget_UI * m_TWTabUi;

    QDialog * m_TWPatternDialog;
    Ui::TypeWriterPatternDialog_UI * m_TWPatternUI;

    QString m_projectTitlePath;
    TypeWriterDocument m_typewriterdocument;

};

#endif
