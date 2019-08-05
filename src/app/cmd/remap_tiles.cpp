// Aseprite
// Copyright (C) 2019  Igara Studio S.A.
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/cmd/remap_tiles.h"

#include "doc/cel.h"
#include "doc/cels_range.h"
#include "doc/image.h"
#include "doc/layer.h"
#include "doc/layer_tilemap.h"
#include "doc/remap.h"
#include "doc/sprite.h"
#include "doc/tileset.h"

namespace app {
namespace cmd {

using namespace doc;

RemapTiles::RemapTiles(Tileset* tileset,
                       const Remap& remap)
  : WithTileset(tileset)
  , m_remap(remap)
{
}

void RemapTiles::onExecute()
{
  Tileset* tileset = this->tileset();
  Sprite* spr = tileset->sprite();
  spr->remapTilemaps(tileset, m_remap);
  incrementVersions(tileset);
}

void RemapTiles::onUndo()
{
  Tileset* tileset = this->tileset();
  Sprite* spr = tileset->sprite();
  spr->remapTilemaps(tileset, m_remap.invert());
  incrementVersions(tileset);
}

void RemapTiles::incrementVersions(Tileset* tileset)
{
  Sprite* spr = tileset->sprite();
  for (const Cel* cel : spr->uniqueCels()) {
    if (cel->layer()->isTilemap() &&
        static_cast<LayerTilemap*>(cel->layer())->tileset() == tileset) {
      cel->image()->incrementVersion();
    }
  }
}

} // namespace cmd
} // namespace app
