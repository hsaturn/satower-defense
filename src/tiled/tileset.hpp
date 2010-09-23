/*
 * tileset.hpp
 *
 *  Created on: 7 août 2010
 *      Author: saturn
 */

#ifndef TILESET_HPP_
#define TILESET_HPP_
#include <SimpleXml.hpp>
#include <SDL/SDL.h>

#include "../coord.hpp"

class tileset
{
public:
	tileset(const SimpleXml*);
	~tileset();

	Uint32	getFirstGid() const { return miFirstGid; }

	bool isDataSet() const	{ return mbDataTileSet; }

	void draw(SDL_Surface*, Uint32 iTileNr, const coord& pos) const;

	const string getName() const { return msName; }

	Uint32 getTileWidth () const { return miTileWidth; }
	Uint32 getTileHeight() const { return miTileHeight; }

private:

	string msName;
	Uint32	miFirstGid;
	Uint32 miTileWidth;
	Uint32 miTileHeight;
	Uint32 miTileSpacing;
	Uint32 miMargin;

	Uint32	miNbrTilesX;	// Number of tiles per row
	Uint32	miNbrTilesY;	// Number of tiles per column

	string	msTrans;
	SDL_Surface*	mpoTiled;
	bool	mbDataTileSet;	// walkabe and buildable tileset info
};
#endif /* TILESET_HPP_ */
