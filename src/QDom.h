/***************************************************************************
** The MIT License (MIT)
** Copyright (c) 2016 Tobias Gläßer
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the Software
** is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#ifndef QDOM_H
#define QDOM_H

#include <QFile>
#include <QTextStream>

#include "tinyxml2.h"

using namespace tinyxml2;

#define TXML2QDOM TinyXML2QDomWrapper

namespace TinyXML2QDomWrapper {

    class QDomNode;
    class QDomElement;
    class QDomProcessingInstruction;

    class QDomText
    {
        friend class QDomElement;
        friend class QDomDocument;

        public:
            QDomText();
            QDomText(XMLText*);
            //QDomText(const QDomText&);
        private:
            XMLText* text;
    };

    class QDomElement
    {
        friend class QDomDocument;

        public:
            QDomElement();
            QDomElement(XMLElement* _el);
            void appendChild(const QDomText&);
            void appendChild(const QDomElement&);
            void setAttribute(const QString&, const QString&);
            const QDomNode firstChild() const;
            const QDomElement firstChildElement() const;
            const QDomElement firstChildElement(const QString&) const;
            QList<QDomElement> childNodes();
            bool hasAttribute(const QString&);
            QString attribute(const QString&);
            const QString text() const;
            bool isNull();
            QString tagName();
        private:
            XMLElement* element;
    };

    class QDomNode
    {
        friend class QDomElement;
        friend class QDomDocument;

        public:
            QDomNode(XMLNode* _node);
            QDomNode();
            const QDomNode nextSibling();
            bool isNull();
            const QDomElement toElement() const;

        private:
            QDomNode* next;
            XMLNode* node;
    };

    class QDomDocument : public QDomNode
    {
        friend class QDomElement;

        public:
            QDomDocument();
            ~QDomDocument();
            void appendChild(const QDomText&);
            void appendChild(const QDomElement&);
            void appendChild(const QDomProcessingInstruction&);
            QDomDocument(const QString&);
            QDomElement documentElement();
            QDomElement createElement(const QString&);
            QDomText createTextNode(const QString&);
            void save(QTextStream&,int);
            QDomProcessingInstruction createProcessingInstruction(const QString&, const QString&);
            bool setContent(QFile* file, bool b, QString* s, int* i, int* i2);
        private:
            XMLDocument* document;
    };


    class QDomProcessingInstruction
    {
        public:
        private:
    };

} //namespace end

#endif
