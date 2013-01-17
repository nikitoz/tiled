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

#ifndef SVINOTA_FF_OBJECTS_H
#define SVINOTA_FF_OBJECTS_H

#include "svinota_global.h"

#include "map.h"
#include "mapwriterinterface.h"
#include "mapreaderinterface.h"
#include "mapobject.h"
#include <QString>
#include <QMap>
#include <QPair>

namespace Svinota {

	class base_writer {
	public:
		QString				write ( const Tiled::MapObject* pObj, int mapHeight, int mapWidth ) const;

	protected:
		virtual QString		auxParams ( const Tiled::MapObject* pObj ) const;
		virtual QString		y () const;
		virtual QString		x () const;
		virtual QString		isStatic () const;
		virtual QString		isSensor () const;
		virtual QString		id () const;
		virtual QString		body () const;
		virtual QString		texture () const;

		QString				buildFromProp  ( const QString& sProp, const Tiled::Properties& propMap, const QString& sTag ) const;
		QString				buildFromValue ( const QString& sProp, const QString& sVal, const QString& sTag ) const;
	};

	class portals_writer   : public base_writer {
	protected:
		virtual QString		auxParams ( const Tiled::MapObject* pObj ) const;
	};

	class gravity_writer   : public base_writer {
	protected:
		virtual QString		auxParams ( const Tiled::MapObject* pObj ) const;
	};

	class platform_writer  : public base_writer {
	protected:
		virtual QString		auxParams ( const Tiled::MapObject* pObj ) const;
	};

	class animation_writer : public base_writer	{
	protected:
		virtual QString		auxParams ( const Tiled::MapObject* pObj ) const;
	};

	class tuffster_writer : public base_writer 
	{	};

	class moostery_writer : public base_writer 
	{	};

	class object_writer   : public base_writer 
	{	};

	struct svinota_writer
	{ static QString		write	  ( const Tiled::MapObject* pObj ); };

} // namespace Svinota

#endif // SVINOTAPLUGIN_H
