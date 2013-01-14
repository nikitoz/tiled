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
#include "svinota_ffobjects.hpp"

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
bool SvinotaPlugin::write(const Tiled::Map *map, const QString &fileName)
{
    using namespace Tiled;

	QString sOut;

    // Check layer count and type
    if (map->objectGroupCount() <= 0) {
        mError = tr("The map needs to have exactly one tile layer!");
        return false;
    }

	QList<ObjectGroup*> lstObjectGroups = map->objectGroups();
	foreach(const ObjectGroup* pObjGr, lstObjectGroups) {	
		if (!pObjGr->isVisible())
			continue;

		svinota_writer writer;
		foreach (const MapObject* pMapObj, pObjGr->objects())
			try { QString s = writer.write(pMapObj); sOut += s; } 
			catch(const QString& exc) { QMessageBox::critical(NULL, "export error", exc); }
	}

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
