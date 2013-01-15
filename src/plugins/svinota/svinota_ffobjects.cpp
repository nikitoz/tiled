/*
 * svinota Tiled Plugin
 * Copyright 2011, FF
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "svinota_ffobjects.h"
#include "map.h"
#include "mapwriterinterface.h"
#include "mapreaderinterface.h"
#include "mapobject.h"
#include <QString>
#include <QMap>
#include <QPair>
#include "objectgroup.h"

namespace Svinota {

	static const QString Y		 = "ff_y";
	static const QString X		 = "ff_x";
	static const QString STATIC  = "ff_is_static";
	static const QString SENSOR  = "ff_is_sensor";
	static const QString ID		 = "ff_id";
	static const QString BODY	 = "ff_body";
	static const QString TEXTURE = "ff_texture";

	QString base_writer::write ( const Tiled::MapObject* pObj, int mapHeight, int mapWidth ) const {
		Q_UNUSED(mapWidth);
		return 
			  "<key>" + pObj->name() + "</key>\n"
			+ "<dict\n>"

				+ buildFromValue ( x(), QString::number ( pObj->x() ), "float" )
				+ buildFromValue ( y(), QString::number ( mapHeight - pObj->y() ), "float" )
				+ buildFromProp  ( isSensor(), pObj->properties(), "")
				+ buildFromProp  ( isStatic(), pObj->properties(), "")
				+ buildFromProp  ( texture(),  pObj->properties(), "string")
				+ buildFromProp  ( body(),	   pObj->properties(), "string")
				+ buildFromProp  ( id(),	   pObj->properties(), "integer")
				+ auxParams ( pObj )

			+ "</dict>\n";
	}

	QString base_writer::y () const
	{ return Y;	}

	QString base_writer::x () const 
	{ return X;	}

	QString base_writer::isStatic () const
	{ return STATIC; }

	QString base_writer::isSensor () const
	{ return SENSOR; }

	QString base_writer::id () const
	{ return ID; }

	QString base_writer::body () const 
	{ return BODY; }

	QString base_writer::texture () const 
	{ return TEXTURE; }

	QString base_writer::auxParams ( const Tiled::MapObject* pObj ) const 
	{ Q_UNUSED ( pObj ); return QString::null;	}

	QString base_writer::buildFromProp ( const QString& sProp, const Tiled::Properties& propMap, const QString& sTag ) const	{
		return (propMap.contains(sProp) ?
			"\t<key>" + sProp + "</key>\n" + 
			(sTag.isEmpty() ? "\t<" + propMap[sProp] + "/>\n" : "\t<"+sTag+">" + propMap[sProp] + "</"+sTag+">\n")
			: QString::null );
	}

	QString base_writer::buildFromValue ( const QString& sProp, const QString& sVal, const QString& sTag ) const	{
		return 
			  "\t<key>" + sProp + "</key>\n"
			+ "\t<"+sTag+">" + sVal + "</"+sTag+">\n";
	}


	QString portals_writer::auxParams ( const Tiled::MapObject* pObj ) const
	{ return buildFromProp ( "ff_portal_id", pObj->properties (), "integer" ); }


	QString gravity_writer::auxParams ( const Tiled::MapObject* pObj ) const
	{ return buildFromProp ( "ff_is_reverse", pObj->properties (), "" ); }


	QString object_writer::auxParams ( const Tiled::MapObject* pObj ) const
	{ Q_UNUSED ( pObj ); return QString::null; }


	QString platform_writer::auxParams ( const Tiled::MapObject* pObj ) const
	{ Q_UNUSED ( pObj ); return QString::null; }


	QString animation_writer::auxParams ( const Tiled::MapObject* pObj ) const
	{ Q_UNUSED ( pObj ); return QString::null;	}


	typedef QPair<QString, base_writer*> ffwriter_t;
	ffwriter_t writers[ ] = {
	      ffwriter_t ( "ff_script_portal",    new portals_writer )
		, ffwriter_t ( "ff_script_gravity",   new gravity_writer )
		, ffwriter_t ( "ff_object",			  new object_writer )
		, ffwriter_t ( "ff_moving_platform",  new platform_writer )
		, ffwriter_t ( "ff_script_animation", new animation_writer )
	};

	QString svinota_writer::write ( const Tiled::MapObject* pObj ) {
		int n = sizeof ( writers ) / sizeof ( ffwriter_t );
		for ( int i = 0; i < n; ++i )
			if ( pObj->name ().contains ( writers[ i ].first ) )
				return writers[ i ].second->write ( pObj, pObj->objectGroup()->map()->height(), pObj->objectGroup()->map()->width() );

		throw QString ( "Couldn't find writer for " + pObj->name () + " object!\n" );
	}

} // namespace Svinota
