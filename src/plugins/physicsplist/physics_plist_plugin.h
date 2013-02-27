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

#ifndef PHYSICSPLIST_H
#define PHYSICSPLIST_H

#include "physics_plist_global.h"

#include "map.h"
#include "mapwriterinterface.h"
#include "mapreaderinterface.h"

#include <QObject>

namespace PhysicsPlist {

class PHYSICSPLIST_EXPORT PhysicsPlistPlugin :
        public QObject,
        public Tiled::MapWriterInterface,
        public Tiled::MapReaderInterface
{
    Q_OBJECT
    Q_INTERFACES(Tiled::MapReaderInterface)
    Q_INTERFACES(Tiled::MapWriterInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.mapeditor.MapWriterInterface" FILE "plugin.json")
    Q_PLUGIN_METADATA(IID "org.mapeditor.MapReaderInterface" FILE "plugin.json")
#endif

public:
    PhysicsPlistPlugin();

    // MapReaderInterface
    Tiled::Map *read(const QString &fileName);
    bool supportsFile(const QString &fileName) const;

    // MapWriterInterface
    bool write(const Tiled::Map *map, const QString &fileName);
    QString nameFilter() const;
    QString errorString() const;

private:
    QString mError;
};

} // namespace PhysicsPlist

#endif // PHYSICSPLIST_H
