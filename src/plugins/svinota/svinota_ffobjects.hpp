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

namespace {
	static const QString Y		 = "ff_y";
	static const QString X		 = "ff_x";
	static const QString STATIC  = "ff_is_static";
	static const QString SENSOR  = "ff_is_sensor";
	static const QString ID		 = "ff_id";
	static const QString BODY	 = "ff_body";
	static const QString TEXTURE = "ff_texture";

	static const QString PORTAL   = "ff_script_portals";
	static const QString GRAVITY  = "ff_script_gravity";
	static const QString OBJECT   = "ff_object";
	static const QString PLATFORM = "ff_moving_platform";
};

namespace Svinota {

	struct base_writer {
		QString write(const Tiled::MapObject* pObj, int mapHeight, int mapWidth) const {
			Q_UNUSED(mapWidth);
			return "<key>" + pObj->name() + "</key>\n"
					+ "<dict\n>"

						+ buildFromValue (x(), QString::number(pObj->x()), "float")
						+ buildFromValue (y(), QString::number(mapHeight - pObj->y()), "float")
						+ buildFromProp(isSensor(), pObj->properties(), "")
						+ buildFromProp(isStatic(), pObj->properties(), "")
						+ buildFromProp(texture(),  pObj->properties(), "string")
						+ buildFromProp(body(),		pObj->properties(), "string")
						+ buildFromProp(id(),		pObj->properties(), "integer")
						+ auxParams(pObj)

					+ "</dict>\n";
		}

		virtual QString auxParams(const Tiled::MapObject* pObj) const
		{ Q_UNUSED(pObj); return QString::null; }

		virtual QString y() const
		{ return Y; }

		virtual QString x() const
		{ return X; }

		virtual QString isStatic() const
		{ return STATIC; }

		virtual QString isSensor() const
		{ return SENSOR; }

		virtual QString id() const
		{ return ID; };

		virtual QString body() const
		{ return BODY; }

		virtual QString texture() const
		{ return TEXTURE; }

		QString buildFromProp(const QString& sProp, const Tiled::Properties& propMap, const QString& sTag) const {
			return (propMap.contains(sProp) ?
				"\t<key>" + sProp + "</key>\n" + 
					(sTag.isEmpty() ? "\t<"+propMap[sProp]+"/>\n" : "\t<"+sTag+">" + propMap[sProp] + "</"+sTag+">\n")
				: QString::null );
		}

		QString buildFromValue(const QString& sProp, const QString& sVal, const QString& sTag) const {
			return "\t<key>"+sProp+"</key>\n"
				 + "\t<"+sTag+">" + sVal + "</"+sTag+">\n";
		}
	};

	struct portals_writer : base_writer {
		virtual QString auxParams(const Tiled::MapObject* pObj) const 
		{ return buildFromProp("ff_portal_id", pObj->properties(), "integer"); }
	};

	struct gravity_writer : base_writer {
		virtual QString auxParams(const Tiled::MapObject* pObj) const 
		{ return buildFromProp("ff_is_reverse", pObj->properties(), ""); }
	};

	struct object_writer : base_writer {
		virtual QString auxParams(const Tiled::MapObject* pObj) const
		{ Q_UNUSED(pObj); return QString::null; }
	};

	struct platform_writer : base_writer {
		virtual QString auxParams(const Tiled::MapObject* pObj) const
		{ Q_UNUSED(pObj); return QString::null; }
	};

	struct animation_writer : base_writer {
		virtual QString auxParams(const Tiled::MapObject* pObj) const
		{ Q_UNUSED(pObj); return QString::null; }
	};
	struct svinota_writer {
	svinota_writer() {
		writers[PORTAL]   = new portals_writer;
		writers[GRAVITY]  = new gravity_writer;
		writers[OBJECT]   = new object_writer;
		writers[PLATFORM] = new platform_writer;
	}

	QString write(const Tiled::MapObject* pObj) {
		for ( WMap::const_iterator it = writers.begin(); it != writers.end(); ++it ) {
			QString ss1 = pObj->name();
			ss1 = it.key();
			if (pObj->name().contains(it.key()))
				return it.value()->write(pObj, pObj->objectGroup()->map()->height(), pObj->objectGroup()->map()->width());
		}

		throw QString("Couldn't find writer for " + pObj->name() + " object!\n");
	}

	typedef QMap< QString, base_writer* > WMap;
	WMap writers;
	};


} // namespace Svinota

#endif // SVINOTAPLUGIN_H
