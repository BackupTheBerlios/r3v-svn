/****************************************************************************
** glWidget meta object code from reading C++ file 'glwidget.h'
**
** Created: Thu Mar 3 19:18:47 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "glwidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *glWidget::className() const
{
    return "glWidget";
}

QMetaObject *glWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_glWidget( "glWidget", &glWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString glWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "glWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString glWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "glWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* glWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUMethod slot_0 = {"openMap", 0, 0 };
    static const QUMethod slot_1 = {"updateFPS", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "openMap()", &slot_0, QMetaData::Private },
	{ "updateFPS()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"glWidget", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_glWidget.setMetaObject( metaObj );
    return metaObj;
}

void* glWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "glWidget" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

bool glWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: openMap(); break;
    case 1: updateFPS(); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool glWidget::qt_emit( int _id, QUObject* _o )
{
    return QGLWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool glWidget::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool glWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
