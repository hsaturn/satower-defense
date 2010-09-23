/*
 * tileset.cpp
 *
 *  Created on: 7 ao�t 2010
 *      Author: saturn
 */

#include "tileset.hpp"
#include "../coord.hpp"
#include "../vars.hpp"
#include "../game.hpp"
#include <SDL/SDL_image.h>

extern void setWalkable(int x,int y);
extern void setBuildable(int x, int y);

tileset::tileset(const SimpleXml* poXml)
:
	mpoTiled(0),
	mbDataTileSet(false)
{
	try
	{
		if (poXml->key()!="tileset")
		{
			throw new CSException("tileset","Not a tileset, got ("+poXml->key()+") instead.");
		}
		msName=poXml->getTextProperty("name");
		miFirstGid=poXml->getLongProperty("firstgid");
		miTileWidth=poXml->getLongProperty("tilewidth");
		miTileHeight=poXml->getLongProperty("tileheight");
		miTileSpacing=poXml->getLongProperty("spacing",0);
		miMargin=poXml->getLongProperty("margin",0);

		mbDataTileSet=(msName=="walk_build");


		const SimpleXml*	poImage=poXml->getNode("image");
		if (poImage==0)
		{
			throw new CSException("tileset","tileset ["+msName+"] has  no image node !");
		}

		if (poImage->hasProperty("trans"))
		{
			msTrans=poImage->getTextProperty("trans");
		}
		string sTileFile=poImage->getTextProperty("source");
		Uint32	iSlash=sTileFile.length()-1;
		while(iSlash>0)
		{
			if (sTileFile[iSlash]=='/')
				break;
			iSlash--;
		}
		if (iSlash)
		{
			sTileFile=sTileFile.substr(iSlash+1);
		}
		sTileFile=gpGame->findRsrcFile("maps/"+sTileFile,true);
		mpoTiled = IMG_Load(sTileFile.c_str());
		if (mpoTiled==0)
			throw new CSException("tileset","Unable to load image "+sTileFile);

		if (msTrans.length())
		{
			Uint8 r=0,v=0,b=0;
			if (msTrans[0]=='f')
				r=255;
			if (msTrans[2]=='f')
				v=255;
			if (msTrans[4]=='f')
				b=255;
			int iRet=SDL_SetColorKey(mpoTiled, SDL_SRCCOLORKEY, SDL_MapRGB(mpoTiled->format,r,v,b));
			if (iRet)
				cerr << "Failed to set transparency.";
		}

		/*mpoTiled = SDL_AllocSurface(SDL_SWSURFACE, img->w, img->h,
				img->format->BitsPerPixel,
				img->format->Rmask, img->format->Gmask,
				img->format->Bmask,img->format->Amask); */
		miNbrTilesX=(mpoTiled->w-2*miMargin+1)/(miTileWidth+miTileSpacing);
		miNbrTilesY=(mpoTiled->h-2*miMargin+1)/(miTileHeight+miTileSpacing);
	}
	catch(CSException *p)
	{
		if (msName.length())
		{
			throw new CSException("tileset",p->getCompleteError()+", name="+msName);
		}
		else
			throw p;
	}
}

tileset::~tileset()
{
	if (mpoTiled)
		delete mpoTiled;
}
void tileset::draw(SDL_Surface* img, Uint32 iTileNr, const coord &pos) const
{
	iTileNr-=miFirstGid;
	if (mbDataTileSet)
	{
		if (iTileNr<=2)	// Walkable
		{
			setWalkable(pos.x(),pos.y());
		}
		else if (iTileNr==3)
		{
			setBuildable(pos.x(),pos.y());
		}
		if (iTileNr==0)	// Start Point
		{
			gpGame->addStartTile(pos);
		}
		else if (iTileNr==1)	// End point
		{
			gpGame->addEndTile(pos);
		}
	}
	else
	{
		Uint32	iY=miMargin+(miTileHeight+miTileSpacing)*(iTileNr/miNbrTilesX);
		Uint32	iX=miMargin+(miTileWidth+miTileSpacing)*(iTileNr%miNbrTilesX);
		coord p(iX,iY);
		SDL_Rect src;
		src.x=iX;
		src.y=iY;
		src.w=miTileWidth;
		src.h=miTileHeight;
		SDL_Rect dest;
		dest.x=pos.x();
		dest.y=pos.y()-miTileHeight;
		dest.w=miTileWidth;
		dest.h=miTileHeight;
		SDL_BlitSurface(mpoTiled,&src,img,&dest);
	}
}
