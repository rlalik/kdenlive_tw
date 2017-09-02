/***************************************************************************
                          titledocument.h  -  description
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
#ifndef TYPEWRITERDOCUMENT_H
#define TYPEWRITERDOCUMENT_H

#include "titledocument.h"

#include <QDomDocument>

class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsItem;

class QListWidget;

struct TypeWriterPattern
{
    QString name;
    QString text;
    int length;
    QString fill;
};

class TypeWriterDocument : public TitleDocument
{
public:
    TypeWriterDocument();
    TypeWriterDocument(const TitleDocument & titledoc);

    QDomDocument xml(QGraphicsRectItem *startv, QGraphicsRectItem *endv, bool embed_images = false);
    int loadFromXml(const QDomDocument &doc, QGraphicsRectItem *startv, QGraphicsRectItem *endv, int *duration, const QString &projectpath = QString());

    void setPatternsList(QListWidget * p) { patterns = p; }

private:
    QListWidget * patterns;
};

#endif

