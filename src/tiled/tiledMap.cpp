/*
 * map.cpp
 *
 *  Created on: 7 ao�t 2010
 *      Author: saturn
 */

#include "tiledMap.hpp"

extern int mapWidth;
extern int mapHeight;

tiledMap::tiledMap(const string& sXmlFileName)
:
	msXmlFileName(sXmlFileName)
{
	bool bHasBuildWalkTileSet=false;
	try
	{
		CFileParser	xmlFile(sXmlFileName,true,true);
		SimpleXml	xml(&xmlFile);

		const SimpleXml*	pMap=xml.getNode("map");
		if (pMap==0)
			throw new CSException("tiledMap","xml node map not found !!!");

		string sOrientation=pMap->getTextProperty("orientation");
		if (sOrientation!="orthogonal")
		{
			throw new CSException("tileMap","Only orthogonal maps are supported yet");
		}
		miMapWidth=pMap->getLongProperty("width");
		miMapHeight=pMap->getLongProperty("height");
		mapWidth=miMapWidth;
		mapHeight=miMapHeight;

		miTileWidth=pMap->getLongProperty("tilewidth");
		miTileHeight=pMap->getLongProperty("tileheight");
		// FIXME check against aStarLibrary maxMapWidth/Height
		tileSizeX=miTileWidth;	// FIXME, should be done when activating map
		tileSizeY=miTileHeight;
		list<SimpleXml*>::const_iterator oit=pMap->children()->begin();
		while(oit!=pMap->children()->end())
		{
			SimpleXml*	p=*oit;
			if (p->key()=="tileset")
			{
				tileset* pTileSet=new tileset(p);
				mmapTileSets [pTileSet->getFirstGid()]=pTileSet;
				mmapTileSets2[pTileSet->getName()    ]=pTileSet;
				if (pTileSet->isDataSet())
					bHasBuildWalkTileSet=true;
			}
			else if (p->key()=="layer")
			{
				layer* pLayer=new layer(p);
				mmapLayers[p->getTextProperty("name")]=pLayer;
			}
			else
			{
				cout << "Unknown xml node type " << p->key() << endl;
			}
			oit++;
		}
		checkLayer("walkable");
		checkLayer("buildable");
		checkLayer("map_0");
		if (bHasBuildWalkTileSet==false)
		{
			throw new CSException("tiledMap","No walk_build tileset defined !");
		}
	}
	catch(CSException* p)
	{
		throw new CSException("tiledMap","In file "+msXmlFileName+"\n"+p->getCompleteError());
	}
}

tiledMap::~tiledMap()
{
	map<Uint32,tileset*>::iterator oit=mmapTileSets.begin();
	while(oit!=mmapTileSets.end())
	{
		delete oit->second;
		oit++;
	}

	map<string,layer*>::iterator oitL=mmapLayers.begin();
	while(oitL!=mmapLayers.end())
	{
		delete oitL->second;
		oitL++;
	}
}

void tiledMap::checkLayer(const string& sName)
{
	map<string,layer*>::iterator oit=mmapLayers.find(sName);
	if (oit==mmapLayers.end())
	{
		throw new CSException("tiledMap","Missing layer ["+sName+"]");
	}
}

void tiledMap::draw(SDL_Surface* img,coord pos, bool bEveryOnly)
{
	Uint32 iMapNumber=0;

	while(true)
	{
		string sMap="map_"+to_string(iMapNumber++);
		map<string,layer*>::iterator oit=mmapLayers.find(sMap);
		if (oit==mmapLayers.end())
			break;

		layer* p=oit->second;

		if (p->isEveryTime()==bEveryOnly)
			p->draw(img,pos,this);
	}
}

void tiledMap::drawWalkableBuildable()
{
	layer* p;
	coord zero(0,0);
	map<string,layer*>::iterator oit=mmapLayers.find("buildable");
	if (oit!=mmapLayers.end())
	{
		cout << "Setting buildable..." << endl;
		p=oit->second;
		p->draw(img,zero,this);
	}
	else
		cerr << "No buildable layer found !!!" << endl;

	oit=mmapLayers.find("walkable");
	if (oit!=mmapLayers.end())
	{
		cout << "Setting walkable..." << endl;
		p=oit->second;
		p->draw(img,zero,this);
	}
	else
		cerr << "No walkable layer found !!!" << endl;
}

void tiledMap::drawTile(SDL_Surface* img, Uint32 iTileNr, const coord& pos) const
{

	tileset* pFound=0;

	map<Uint32,tileset*>::const_iterator oit=mmapTileSets.begin();
	while(iTileNr>=oit->first)
	{
		if (oit!=mmapTileSets.end())
		{
			pFound=oit->second;
			oit++;
		}
		else
		{
			break;
		}
	}

	if (pFound)
	{
		pFound->draw(img, iTileNr,pos);
	}
}

void tiledMap::drawTileCentered(SDL_Surface* img, Uint32 iTileNr, coord pos, const string &sTileSet) const
{

	map<string,tileset*>::const_iterator oit=mmapTileSets2.find(sTileSet);
	if (oit!=mmapTileSets2.end())
	{
		tileset* pFound=oit->second;

		pos.setY(pos.y()+pFound->getTileHeight()/2+pFound->getTileWidth()/2);
		pFound->draw(img, iTileNr+pFound->getFirstGid(),pos);
	}
	else
	{
		cerr << "Tileset '" << sTileSet << "' not found in map " << msXmlFileName << endl;
	}
}
