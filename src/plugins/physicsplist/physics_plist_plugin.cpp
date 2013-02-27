/*
 * physics .plist Tiled Plugin
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

#include "physics_plist_plugin.h"

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

using namespace PhysicsPlist;

const int TILE_SIZE = 32;

PhysicsPlistPlugin::PhysicsPlistPlugin() { }

// Reader
Tiled::Map *PhysicsPlistPlugin::read(const QString &fileName)
{
	// not implemented
    return NULL;
}

bool PhysicsPlistPlugin::supportsFile(const QString &fileName) const
{
    return QFileInfo(fileName).suffix() == QLatin1String(".plist");
}

// Writer
bool PhysicsPlistPlugin::write(const Tiled::Map *map, const QString& fileName)
{
    using namespace Tiled;

	// Check layer count and type
    if (map->layerCount() <= 0) {
        mError = tr("The map needs to have at least one tile layer!");
        return false;
    }


	QString sOut (
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
	"<plist version=\"1.0\">"
		"<dict>"
		"<key>metadata</key>"
		"<dict>"
		"<key>format</key>"
		"<integer>1</integer>"
		"<key>ptm_ratio</key>"
		"<real>32</real>"
		"</dict>"
		"<key>bodies</key>"
		"<dict>"

		"<key>%1</key>"
		"<dict>
		"<key>anchorpoint</key>"
		"<string>{ 0.0000,0.0000 }</string>"
		"<key>fixtures</key>"
		"<array>"

		"<dict>
		"<key>density</key> <real>999999</real>"
		"<key>friction</key> <real>0.9</real>"
		"<key>restitution</key> <real>0</real>"
		"<key>filter_categoryBits</key> <integer>1</integer>"
		"<key>filter_groupIndex</key> <integer>0</integer>"
		"<key>filter_maskBits</key> <integer>65535</integer>"
		"<key>isSensor</key> <false/>"
		"<key>id</key> <string></string>"
		"<key>fixture_type</key> <string>POLYGON</string>"

		"<key>polygons</key>"
		"<array>"
		).arg( fileName );

	QString sObjects;

	foreach( const Layer* pLayer, map->layers(Layer::TileLayerType) )
	{
		if ( pLayer->isVisible() )
		{
			int h = pLayer->asTileLayer()->size().height();
			int w = pLayer->asTileLayer()->size().width();
			for (int j = 0; j < h; ++j)
			{
				for (int i = 0; i < w; ++i)
				{
					if ( !pLayer->asTileLayer()->cellAt( i, j ).isEmpty() && pLayer->asTileLayer()->cellAt( i, j ) )
						sObjects += makeQuad( i*TILE_SIZE, h*TILE_SIZE - j);
				}
			}
		}
	}

	sOut += sObjects + "</array>\n</dict>\n</array>\n</dict>\n</dict>\n</dict>\n</plist>";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        mError = tr("Could not open file for writing.");
        return false;
    }

    file.write(sOut.toLatin1());
    file.close();

    return true;
}

QString makeQuad( int x, int y ) const
{
	return "<array> "
		"<string>{" + QString(" %1, %2 ").arg(x, y)		+ "}</string>"
		"<string>{" + QString(" %1, %2 ").arg(x+1, y)   + "}</string>"
		"<string>{" + QString(" %1, %2 ").arg(x, y+1)   + "}</string>"
		"<string>{" + QString(" %1, %2 ").arg(x+1, y+1) + "}</string>"
		" </array>\n";
}

QString PhysicsPlistPlugin::nameFilter() const
{
    return tr("Physics plist files (*.plist)");
}

QString PhysicsPlistPlugin::errorString() const
{
    return mError;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Physics, PhysicsPlistPlugin)
#endif
