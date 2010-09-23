/*
 * layer.cpp
 *
 *  Created on: 7 ao�t 2010
 *      Author: saturn
 */

#include "layer.hpp"
#include "tiledMap.hpp"

layer::layer(const SimpleXml* poXml)
: mbVisible(true),
mbDataLayer(false)
{
	if (poXml->key()!="layer")
	{
		throw new CSException("tileset","Not a layer, got ("+poXml->key()+") instead.");
	}
	msName=poXml->getTextProperty("name");
	if (msName=="buildable" || msName=="walkable")
	{
		mbDataLayer=true;
	}
	miWidth=poXml->getLongProperty("width");
	miHeight=poXml->getLongProperty("height");
	if (poXml->hasProperty("visible"))
		mbVisible=poXml->getLongProperty("visible",0)!=0;

	mbEveryTime=false;

	const SimpleXml* poProperties=poXml->getNode("properties");
	if (poProperties)
	{
		mbEveryTime=true;
		// poXml->getLongProperty("everytime",0);
	}

	cout << "layer " << msName << ' ' << poXml->getTextProperty("everytime","???") << ' ';
	if (mbEveryTime)
		cout << " every";
	else
		cout << " once";
	cout << endl;

	const SimpleXml* pData=poXml->getNode("data");
	if (pData==0)
	{
		throw new CSException("tileset","Layer ["+msName+"] must have a data node !");
	}

	if (pData->getTextProperty("encoding")!="csv")
	{
		throw new CSException("layer","Data layer must be csv encoded. Layer:["+msName+"]");
	}
	string sCsv=pData->value();
	while(sCsv.length())
	{
		int i=atoi(sCsv.c_str());
		mlstTiles.push_back(i);
		while(isdigit(sCsv[0]))
			sCsv=sCsv.substr(1);

		while(!isdigit(sCsv[0]) && sCsv.length()>0)
			sCsv=sCsv.substr(1);
	}
	if (mlstTiles.size()!=miWidth*miHeight)
		throw new CSException("layer","Csv data must be "+to_string(miWidth*miHeight)+" numbers. "+to_string(mlstTiles.size())+" found.");
}

layer::~layer()
{

}

void layer::draw(SDL_Surface *img, const coord &pos, const tiledMap* pMap) const
{
	list<int>::const_iterator oit=mlstTiles.begin();

	if (mbVisible)
	{
		for(Uint32 iRows=0;iRows<miHeight;iRows++)
		{
			for(Uint32 iCols=0;iCols<miWidth; iCols++)
			{
				coord p;
				if (mbDataLayer)
				{
					p.setX(iCols);
					p.setY(iRows);
				}
				else
				{
					p.setX(pos.x()+iCols*pMap->getTileWidth());
					p.setY(pos.y()+(1+iRows)*pMap->getTileHeight());
				}
				Uint32 iTileNr=*oit++;
				if (iTileNr)
					pMap->drawTile(img,iTileNr,p);
			}
		}
	}
}
