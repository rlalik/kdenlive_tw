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

#include "typewriterdocument.h"
#include "gradientwidget.h"

#include "kdenlivesettings.h"
#include "timecode.h"
#include "effectstack/graphicsscenerectmove.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <KIO/FileCopyJob>

#include "kdenlive_debug.h"
#include <QListWidgetItem>
#ifdef Q_OS_MAC
#include <xlocale.h>
#endif

#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>


TypeWriterDocument::TypeWriterDocument() :
    TitleDocument(), patterns(nullptr)
{
}

TypeWriterDocument::TypeWriterDocument(const TitleDocument& titledoc) :
    TitleDocument(titledoc)
{
}

QDomDocument TypeWriterDocument::xml(QGraphicsRectItem *startv, QGraphicsRectItem *endv, bool embed)
{
    QDomDocument doc = TitleDocument::xml(startv, endv, embed);
    QDomElement root = doc.documentElement();

    if (!patterns)
        return doc;

    QDomElement el_pats = doc.createElement(QStringLiteral("patterns"));
    el_pats.setAttribute(QStringLiteral("version"), 1);
    doc.appendChild(el_pats);

    int n = patterns->count();
    for (int i = 0; i < n; ++i)
    {
        QListWidgetItem * item = patterns->item(i);

        QDomElement el_pat = doc.createElement(QStringLiteral("pattern"));
        el_pat.setAttribute(QStringLiteral("name"), item->text());

        QDomElement el_text = doc.createElement(QStringLiteral("text"));
        el_text.appendChild(
            doc.createTextNode(item->data(Qt::UserRole+0).toString()));
        el_pat.appendChild(el_text);

        QDomElement el_length = doc.createElement(QStringLiteral("length"));
        el_length.appendChild(
            doc.createTextNode(QStringLiteral("%1").arg(
            item->data(Qt::UserRole+1).toInt())));
        el_pat.appendChild(el_length);

        QDomElement el_fill = doc.createElement(QStringLiteral("fill"));
        el_fill.appendChild(
            doc.createTextNode(item->data(Qt::UserRole+2).toString()));
        el_pat.appendChild(el_fill);

        el_pats.appendChild(el_pat);
    }

    root.appendChild(el_pats);

    return doc;
}

int TypeWriterDocument::loadFromXml(const QDomDocument &doc, QGraphicsRectItem *startv, QGraphicsRectItem *endv, int *duration, const QString &projectpath)
{
    patterns->clear();

    QDomNodeList titles = doc.elementsByTagName(QStringLiteral("kdenlivetitle"));

    if (!titles.isEmpty())
    {
        QDomNodeList items = titles.item(0).childNodes();
        for (int i = 0; i < items.count(); ++i)
        {
            QDomNode itemNode = items.item(i);
            if (itemNode.nodeName() == QStringLiteral("patterns"))
            {
                QDomNodeList ps = itemNode.childNodes();
                for (int j = 0; j < ps.count(); ++j)
                {
                    QDomNode p = ps.item(j);
                    QListWidgetItem * item = new QListWidgetItem(
                        p.attributes().namedItem(QStringLiteral("name")).nodeValue());
                    QDomNodeList l = p.childNodes();
                    for (int k = 0; k < l.count(); ++k)
                    {
                        QDomNode pi = l.item(k);
                        if (pi.nodeName() == "text")
                            item->setData(Qt::UserRole+0, pi.firstChild().nodeValue());
                        else if (pi.nodeName() == "length")
                            item->setData(Qt::UserRole+1, pi.firstChild().nodeValue().toInt());
                        else if (pi.nodeName() == "fill")
                            item->setData(Qt::UserRole+2, pi.firstChild().nodeValue());
                    }
                    patterns->addItem(item);
                }
            }
        }
    }

    return TitleDocument::loadFromXml(doc, startv, endv, duration, projectpath);
}
