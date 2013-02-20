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

#include "svinotaplugin.h"

#include "map.h"
#include "tile.h"
#include "tileset.h"
#include "tilelayer.h"
#include "compression.h"

#include <QFile>
#include <QFileInfo>
#include "mapobject.h"
#include "objectgroup.h"
#include <QList>
#include <QMessageBox>
#include "svinota_ffobjects.h"

using namespace Svinota;

SvinotaPlugin::SvinotaPlugin() { }

// Reader
Tiled::Map *SvinotaPlugin::read(const QString &fileName)
{
	// not implemented
    return NULL;
}

bool SvinotaPlugin::supportsFile(const QString &fileName) const
{
    return QFileInfo(fileName).suffix() == QLatin1String(".plist");
}

// Writer
bool SvinotaPlugin::write(const Tiled::Map *map, const QString& fileName)
{
    using namespace Tiled;

	QString sOut (
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<plist version=\"1.0\">\n"
		"<dict>\n"
		);

	QString sObjects;

    // Check layer count and type
    if (map->objectGroupCount() <= 0) {
        mError = tr("The map needs to have exactly one tile layer!");
        return false;
    }

	foreach( const ObjectGroup* pObjGr, map->objectGroups() )
		if ( pObjGr->isVisible() )
			foreach ( const MapObject* pMapObj, pObjGr->objects() )
				try { 
					if ( pMapObj->type() == "ff_include" )
						sOut += svinota_writer::write( pMapObj );
					else
						sObjects += svinota_writer::write( pMapObj );
				} catch( const QString& exc ) 
				{ QMessageBox::critical( NULL, "export error", exc ); }

	sOut += sObjects + "<key>ff_debug_draw</key>\n<false/>\n</dict>\n</plist>";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        mError = tr("Could not open file for writing.");
        return false;
    }

    file.write(sOut.toLatin1());
    file.close();

    return true;
}

QString SvinotaPlugin::nameFilter() const
{
    return tr("Svinota plist files (*.plist)");
}

QString SvinotaPlugin::errorString() const
{
    return mError;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Svinota, SvinotaPlugin)
#endif
