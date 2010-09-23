/*
 * layer.hpp
 *
 *  Created on: 7 ao�t 2010
 *      Author: saturn
 */

#ifndef LAYER_HPP_
#define LAYER_HPP_

#include <SimpleXml.hpp>
#include <SDL/SDL.h>

#include "../coord.hpp"
#include <list>

using namespace std;

class tiledMap;

class layer
{
public:
	layer(const SimpleXml*);
	~layer();

	// bAll allow to request drawing for undefined tags (0).
	// Thus, we can set walkability / buildability for
	// layers that are dedicated to this.
	void draw(SDL_Surface*, const coord& pos, const tiledMap*) const;

	bool isEveryTime() const { return mbEveryTime; }
private:
	string			msName;
	unsigned int	miWidth;
	unsigned int	miHeight;
	bool 			mbVisible;
	list<int>		mlstTiles;
	bool			mbDataLayer;	// walkability / buildability map
	bool			mbEveryTime;	// True if map is drawn each frame (everytime xml node property)
};

#endif /* LAYER_HPP_ */
