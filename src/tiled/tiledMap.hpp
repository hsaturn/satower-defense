/*
 * map.hpp
 *
 *  Created on: 7 ao�t 2010
 *      Author: saturn
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include <SDL/SDL.h>
#include "../coord.hpp"

#include "tileset.hpp"
#include "layer.hpp"

#include <map>
#include <list>

using namespace std;

class tiledMap
{
public:
	tiledMap(const string& sXmlFileName);
	~tiledMap();

	void drawWalkableBuildable();
	void draw(SDL_Surface*, coord pos, bool bEveryOnly);
	void drawTile(SDL_Surface*, Uint32 iTileNr,const coord &pos) const;

	/**
	 * Draw a tile so the center of the tile corresponds to pos
	 */
	void drawTileCentered(SDL_Surface*, Uint32 iTileNr, coord pos, const string &sTileSet) const;

	Uint32 getMapWidth() const { return miMapHeight; }
	Uint32 getMapHeight() const { return miMapWidth; }
	Uint32 getTileWidth() const { return miTileWidth; }
	Uint32 getTileHeight() const { return miTileHeight; }

private:
	void checkLayer(const string &sName);

	string					msXmlFileName;
	map<Uint32,tileset*>	mmapTileSets;	// The number is the firstgid of the tileset
	map<string,tileset*>	mmapTileSets2;	// Same map ordered with names.
	map<string,layer*>		mmapLayers;
	Uint32					miMapWidth;
	Uint32					miMapHeight;
	Uint32					miTileWidth;
	Uint32					miTileHeight;
};
#endif /* MAP_HPP_ */
