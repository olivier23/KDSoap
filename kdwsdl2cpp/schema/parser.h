// kate: space-indent on; indent-width 2; encoding utf-8; replace-tabs on;
/*
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>
    Copyright (c) 2006 Michaël Larouche <michael.larouche@kdemail.net>
                       based on wsdlpull parser by Vivek Krishna

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
 */

#ifndef SCHEMA_PARSER_H
#define SCHEMA_PARSER_H

#include <QDomElement>
#include <QList>
#include <QFile>

#include "types.h"
#include "annotation.h"
#include <kode_export.h>

class ParserContext;

namespace XSD {

class SCHEMA_EXPORT Parser
{
  public:
    enum { UNBOUNDED = 100000 };

    Parser( ParserContext *context, const QString &nameSpace = QString() );
    Parser( const Parser &other );
    ~Parser();

    Parser &operator=( const Parser &other );

    Types types() const;

    Annotation::List annotations() const;

    /**
      Parse schema from file.
    */
    bool parseFile( ParserContext *context, const QString &fileName );
    bool parseString( ParserContext *context, const QString &data );

    bool parseSchemaTag( ParserContext *context, const QDomElement &element );

    QString targetNamespace() const;

    /**
      Returns the default schema URI.
     */
    static QString schemaUri();

    static bool debugParsing();

  protected:
    bool parse( ParserContext *context, QXmlInputSource *source );

  private:
    void parseImport( ParserContext *context, const QDomElement& );
    /**
     * @brief Parse include element.
     * The <include> element must include a external schema within the same target namespace
     * of the current document. Use <import> if you want to refer to a external namespace.
     * @param context Current parser context.
     * @param element DOM element to parse.
     */
    void parseInclude( ParserContext *context, const QDomElement& element);
    void addGlobalElement( const Element & );
    void addGlobalAttribute( const Attribute & );
    AttributeGroup parseAttributeGroup( ParserContext *context, const QDomElement& );

    Annotation::List parseAnnotation( ParserContext *context, const QDomElement& );
    ComplexType parseComplexType( ParserContext *context, const QDomElement& );

    void all( ParserContext *context, const QDomElement&, ComplexType& );
    void parseCompositor( ParserContext *context,
      const QDomElement&, ComplexType& );

    void setOccurrenceAttributes( Element &newElement,
      const QDomElement &element );

    Element parseElement( ParserContext *context, const QDomElement &,
      const QString &nameSpace, const QDomElement &occurrenceElement );

    Attribute parseAttribute( ParserContext *context, const QDomElement& );
    void addAny( ParserContext *context, const QDomElement&, ComplexType& );
    void addAnyAttribute( ParserContext *context, const QDomElement&, ComplexType& );

    SimpleType parseSimpleType( ParserContext *context, const QDomElement& );
    void parseRestriction( ParserContext *context, const QDomElement&, SimpleType& );
    void parseComplexContent( ParserContext *context, const QDomElement&, ComplexType& );
    void parseSimpleContent( ParserContext *context, const QDomElement&, ComplexType& );


    void importSchema( ParserContext *context, const QString &location );
    /**
     * @brief Read and include the given schema into the current schema.
     * @param context Current parser context.
     * @param localtion Schema location.
     */
    void includeSchema( ParserContext *context, const QString &location );
    QStringList joinNamespaces( const QStringList&, const QStringList& );

    Element findElement( const QName &name );
    Attribute findAttribute( const QName &name );
    AttributeGroup findAttributeGroup( const QName &name );
    void init(ParserContext *context);
    void clear();
    bool resolveForwardDeclarations();

    class Private;
    Private *d;
};

}

#endif


