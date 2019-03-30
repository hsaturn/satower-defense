/*
 * Game.cpp
 *
 *  Created on: 27 juil. 2010
 *      Author: saturn
 */
using namespace std;
#include <string>
#include "game.hpp"
#include "sound.hpp"
#include "text.hpp"
#include "coord.hpp"
#include "button.hpp"
#include "translate.hpp"
#include "tiled/tiledMap.hpp"
#include <ogol_editor.hpp>

#include <string>
#include <iostream>

#include <missileBase.hpp>
#include "towerBase.hpp"
#include "walkerBase.hpp"
#include <ogolVectoroid.hpp>

extern void InitializePathfinder();
extern char walkability [17][14];

template <class T>
static inline std::string toString (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}


using namespace std;
#include "stdio.h"

bool fileExists(const string sFileName)
{
	ifstream f(sFileName.c_str(), ios::binary);
	if (f.is_open())
	{
		f.close();
		return true;
	}
	return false;
}

string Game::msRsrcPath;
string Game::msThemePath;

Game::Game()
	:
	mlScore(0),
	mlLives(20),
	mlBank(100),
	mlInterest(0),
	mlLevel(1),
	mlBonus(0),
	mpoMusic(0),
	miMusicChannel(0),
	mpoFontSmall(0),
	mpoFontButton(0),
	mpoFontHudge(0),
	mpoMap(0),
	mpTowerFont(0),
	mfHealthFactor(1.0),
	mfOgolScale(1.0),
	miBossCount(0),
	mpoLiveLostSound(0),
	miGameState(STATE_NONE),
	mpGameExtension(nullptr)
{
	InitializePathfinder();
	msFileNameImage = "vierge.png";
	Sound::init(); // FIXME
	if (msRsrcPath == "") msRsrcPath = "rsrc/";
}

Game::~Game()
{
	deleteMusic();
}

bool Game::togglePause()
{
	if (miGameState == STATE_PAUSED)
	{
		setState(STATE_PLAY);
	}
	else if (miGameState == STATE_PLAY)
	{
		setState(STATE_PAUSED);
	}
	else
	{
		cerr << "Game::Cannot toggle pause (bad state)" << endl;
		return false;
	}
	return true;
}


EGAME_STATE StrToGameState(string str)
{
	if (str=="over") return STATE_OVER;
	if (str=="play") return STATE_PLAY;
	if (str=="pause") return STATE_PAUSED;
	if (str=="nogame") return STATE_NOGAME;
	return STATE_NOGAME;
}

string Game::findRsrcFile(const string &sFile, bool bTheme)
{
	string sFileName;

	if (fileExists(msThemePath + sFile))
		sFileName = msThemePath + sFile;
	else if (fileExists(msRsrcPath + sFile))
		sFileName = msRsrcPath + sFile;
	else
		cerr << "Unable to locate file [" + sFile + "]";
	return sFileName;
}

void Game::burnLives(int iKills)
{
	if (mlLives)
		mlLives -= iKills;
	if (mlLives <= 0)
	{
		setState(STATE_OVER);
		mlLives = 0;
	}
	if (mpoLiveLostSound)
		mpoLiveLostSound->play();
}

void Game::changeLevel(const string &sLevel)
{
	msLevel = sLevel;
	changeMusic();
}

void Game::deleteMusic()
{
	msCurrentMusic = "";
	if (mpoMusic)
	{
		Mix_FreeMusic(mpoMusic);
		mpoMusic = 0;
	}

}

void Game::bossDieEndMusic()
{
	cout << "boss die end music" << endl;
	if (miBossCount == 1)
	{
		cout << "End of boss music" << endl;
		Mix_FadeOutChannel(miMusicChannel, 1000);
	}
}

void Game::bossDie()
{
	cout << "Boss died !!" << endl;
	miBossCount--;
	if (miBossCount == 0)
		changeMusic();
	if (miBossCount < 0)
		cerr << "ERROR : boss count < 0 !!!" << endl;
}

void Game::changeMusic(string sMusicFile)
{
	cout << "Changing music:" << sMusicFile << endl;
	if (sMusicFile == "")
		sMusicFile = msMusicFile;
	if (sMusicFile == msCurrentMusic)
		return;

	deleteMusic();
	msCurrentMusic = sMusicFile;
	string sFile = findRsrcFile("music/" + sMusicFile);
	mpoMusic = Mix_LoadMUS(sFile.c_str());
	if (!mpoMusic)
	{
		cerr << "Error loading [" << sFile << "] : " << Mix_GetError() << endl;
	}
	else
	{
		//Mix_PlayMusic(mpoMusic, -1);
		miMusicChannel = Mix_FadeInMusic(mpoMusic, -1, 1000);
	}
}

void Game::displayTowerInfos(SDL_Surface* dest, const towerBase* pTower) const
{
	text* font;
	if (mpTowerFont == 0)
		font = mpoFontSmall;
	else
		font = mpTowerFont;
	font->drawSolid(dest, mTowerNameArea.getTopLeft(), pTower->getName());
	coord pos(mTowersInfoArea.getTopLeft());
	coord row(0, font->size());
	font->drawSolid(dest, pos, _T("Cost  : ") + toString(pTower->getCostMoney()) + " $");
	pos.add(row);
	font->drawSolid(dest, pos, _T("Range : ") + toString(pTower->getRange()) + " m");
	pos.add(row);
	font->drawSolid(dest, pos, _T("Damage: ") + pTower->getDamageInfo());
	pos.add(row);
	//font->drawSolid(dest,pos,pTower->getUpgradeInfo());

	pos = mTowerCellArea.getTopLeft();
	font->drawSolid(dest, pos, _T("Sell : ") + toString(pTower->getSellValue()) + " $");
	/*int iThick=4;
	coord p(pTower->getCenterCoord());
	float iOffset=24/2-iThick;	// FIXME fixed data
	for(int i=iOffset;i<=iOffset+iThick;i++)
	{
		int r=255,g=255,b=0;
		int iAlpha=100+100*i/iThick;
		aalineRGBA(dest,p.x()-i,p.y()-i,p.x()+i,p.y()-i,r,g,b,iAlpha);
		aalineRGBA(dest,p.x()-i,p.y()+i,p.x()+i,p.y()+i,r,g,b,iAlpha);
		aalineRGBA(dest,p.x()-i,p.y()-i,p.x()-i,p.y()+i,r,g,b,iAlpha);
		aalineRGBA(dest,p.x()+i,p.y()-i,p.x()+i,p.y()+i,r,g,b,iAlpha);
	}
	 */
	filledCircleRGBA(dest, pTower->getCenterCoord().x(), pTower->getCenterCoord().y(), pTower->getRange(), 128, 128, 255, 30);
	aacircleRGBA(dest, pTower->getCenterCoord().x(), pTower->getCenterCoord().y(), pTower->getRange(), 255, 255, 255, 60);
}

	
void Game::displayWalkerInfos(SDL_Surface* dest, const walkerBase* pWalker) const
{
	text* font;
	if (mpTowerFont == 0)
		font = mpoFontSmall;
	else
		font = mpTowerFont;
	font->drawSolid(dest, mTowerNameArea.getTopLeft(), pWalker->getShortDesc());
	coord pos(mTowersInfoArea.getTopLeft());
	coord row(0, font->size());

	const int width=80;
	const int height=5;
	pWalker->drawAt(coord(pos.x()+20,pos.y()+20), dest);
	SDL_Rect rect;
	rect.x = pos.x()+50;
	rect.y = pos.y()+20;
	rect.w = (float)width*pWalker->getHealth()/pWalker->getInitialHealth();
	rect.h = height;
	SDL_FillRect(dest, &rect,SDL_MapRGBA(dest->format, 0,128,0,0.4));
	rectangleColor(dest, rect.x, rect.y, rect.x+width,rect.y+height, 0x00FF00ff);
	pos.add(coord(0,50));
	
	
	if (pWalker->isBoss())
	{
		font->drawSolid(dest, pos, _T("Boss"));
		pos.add(row);
	}
		
	font->drawSolid(dest, pos, _T("HP   : ") + toString((int)pWalker->getHealth()));
	pos.add(row);
	font->drawSolid(dest, pos, _T("Speed: ") + toString((int)pWalker->getMaxSpeed())+" m/s");
	pos.add(row);
	if (pWalker->getSpeed()!=pWalker->getMaxSpeed())
	{
		font->drawSolid(dest, pos, _T("Current Speed: ") + toString((int)pWalker->getSpeed())+" m/s");
		pos.add(row);
	}

	//font->drawSolid(dest,pos,pTower->getUpgradeInfo());
	filledCircleRGBA(dest, pWalker->getCoord().x(), pWalker->getCoord().y(), 20, 128, 128, 255, 30);
	aacircleRGBA(dest, pWalker->getCoord().x(), pWalker->getCoord().y(), 20, 255, 255, 255, 60);
	
}

void Game::printScores(SDL_Surface* p)
{
	if (mlLives)
	{
		mpoFontSmall->drawSolid(
								back, mCoordLives,
								toString(mlLives));

		mpoFontSmall->drawSolid(
								back, mCoordScore,
								toString(mlScore));

		mpoFontSmall->drawSolid(
								back, mCoordBank,
								toString(mlBank) + " $");

		mpoFontSmall->drawSolid(
								back, mCoordInterest,
								toString(mlInterest));

		mpoFontSmall->drawSolid(
								back, mCoordLevel,
								toString(mlLevel));

		mpoFontSmall->drawSolid(
								back, mCoordBonus,
								toString(mlBonus));
	}
	else
		mpoFontSmall->drawSolid(
								back, mCoordBank,
								_T("GAME OVER"));


	if (state() == STATE_OVER)
	{
		mpoFontHudge->drawSolidShadowed(back, coord(20, 150), _T("GAME OVER"));
	}
	if (state() == STATE_PAUSED)
	{
		mpoFontHudge->drawSolidShadowed(back, coord(20, 150), _T("GAME PAUSED"));
	}
}

void Game::readConfig(const string &sFileName)
{
	CFileParser oDef(sFileName, true, true);

	oDef.addSearchPath(msRsrcPath);
	while (oDef.good())
	{
		string sItem = oDef.getNextWord();
		if (sItem == "tower")
		{
			towerBase::readOneTower(&oDef);
		}
		else if (sItem == "weapon")
		{
			missileBase::readOneFromDef(&oDef);
		}
		else if (sItem == "walker")
		{
			walkerBase::readOneFromDef(&oDef);
		}
		else
			oDef.throw_("game definition", "definition item (weapon, walker, tower...) expected, [" + sItem + "] got instead !");
		oDef.trim();
	}
}

void Game::readTheme(const string &sTheme)
{
	msThemePath = msRsrcPath + "themes/" + sTheme + "/";
	cout << "Game: reading theme [" + sTheme + "] in path " << msThemePath << endl;

	CFileParser::addSearchPath(msRsrcPath);
	CFileParser::addSearchPath(msThemePath);

	string sFileName = findRsrcFile("theme.def", true);
	CFileParser oDef(sFileName, true, true);

	string sMapFile = "map_2.tmx";
	try
	{
		while (oDef.good())
		{
			text** poReadText = 0;
			Uint32* poInt = 0;

			string sItem = oDef.getNextIdentifier("game item");
			if (sItem == "game_image")
				msFileNameImage = oDef.getNextString("image filename");
			else if (sItem == "state")
			{
				setState(StrToGameState(oDef.getNextIdentifier(sItem)));
			}
			else if (sItem == "extender")
			{
				string sExtender = oDef.getNextString("extender name");
				if (mpGameExtension == nullptr)
				{
					mpGameExtension = createExtension(sExtender);
					if (mpGameExtension == nullptr)
					{
						oDef.throw_("game", "Unknown extension [" + sExtender + "]");
					}
					else
					{
						cout << "Extension " << sExtender << " loaded." << endl;
					}
				}
				else
				{
					oDef.throw_("game", "Only one extender allowed");
				}
			}
			else if (sItem == "music")
			{
				oDef.getExpectedChar("{");
				while (oDef.peekChar() == '"' || oDef.peekChar() == '\'')
				{
					msMusicFile = oDef.getNextString("music filename");
				}
				oDef.getExpectedChar("}");
			}
			else if (sItem == "ogol_scale")
			{
				mfOgolScale = oDef.getNextLong("ogol scale percent") / 100.0;
				cout << "OGOL SCALING : " << mfOgolScale << endl;
			}
			else if (sItem == "map_file")
				sMapFile = oDef.getNextString("Map filename");
			else if (sItem == "small_font")
				poReadText = &mpoFontSmall;
			else if (sItem == "fragments_display_time")
				giOgolVectoroidTimeMs = oDef.getNextLong(sItem + " ms value");
			else if (sItem == "button_font")
				poReadText = &mpoFontButton;
			else if (sItem == "hudge_font")
				poReadText = &mpoFontHudge;
			else if (sItem == "current_next")
				moCurrentNext.readDef(&oDef);
			else if (sItem == "bank")
				mCoordBank.readDef(&oDef);
			else if (sItem == "interest")
				mCoordInterest.readDef(&oDef);
			else if (sItem == "live_lost_sound")
				mpoLiveLostSound = new Sound(&oDef);
			else if (sItem == "score")
				mCoordScore.readDef(&oDef);
			else if (sItem == "lives")
				mCoordLives.readDef(&oDef);
			else if (sItem == "level")
				mCoordLevel.readDef(&oDef);
			else if (sItem == "bonus")
				mCoordBonus.readDef(&oDef);
			else if (sItem == "game_area")
				mGameArea.readDef(&oDef);
			else if (sItem == "towers_areas")
				readTowersAreas(&oDef);
			else if (sItem == "button")
			{
				mlstButtons.push_front(new Button(&oDef));
			}
			else if (!readThemeExtension(sItem, &oDef))
				oDef.throw_("game", "Unknown game item [" + sItem + "]");

			if (poReadText)
			{
				if (*poReadText)
					oDef.throw_("Game", "Font " + sItem + " already defined.");

				*poReadText = new text(&oDef);
			}
			if (poInt)
			{
				*poInt = oDef.getNextLong(sItem + " value");
			}
			oDef.trim();
		}
		if (mpoFontSmall == 0)
			oDef.throw_("Game", "small_font not defined");
		if (mpoFontButton == 0)
			oDef.throw_("Game", "button_font not defined");
		if (mpoFontHudge == 0)
			oDef.throw_("Game", "hudge_font not defined");
	}
	catch (CSException *p)
	{
		cerr << "Error while reading theme file " << sFileName << endl;
		cerr << p->getCompleteError();
		exit(1);
	}

	try
	{
		if (mpoMap)
			delete mpoMap;
		sMapFile = findRsrcFile("maps/" + sMapFile, true);
		mpoMap = new tiledMap(sMapFile);

		mpoMap->drawWalkableBuildable();
		// cout << "game.cpp walkability" << endl;
		for (int y = 0; y < 14; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				// cout << (int)walkability[x][y]<<  " ";
			}
			// cout << endl;
		}
	}
	catch (CSException *p)
	{
		cerr << p->getCompleteError() << endl;
	}
}

void Game::readTowersAreas(CFileParser* poDef)
{
	poDef->getExpectedChar("{");
	while (poDef->good())
	{
		if (poDef->peekChar() == '}')
		{
			poDef->getExpectedChar("}");
			break;
		}

		string sItem = poDef->getNextIdentifier("towers_areas item");
		if (sItem == "pick_area")
			mTowersArea.readDef(poDef);
		else if (sItem == "info_area")
			mTowersInfoArea.readDef(poDef);
		else if (sItem == "name_area")
			mTowerNameArea.readDef(poDef);
		else if (sItem == "sell_area")
			mTowerCellArea.readDef(poDef);
		else if (sItem == "font")
		{
			mpTowerFont = new text(poDef);
		}
		else
			poDef->throw_("theme", "Unknown towers_areas item [" + sItem + "]");
	}
}

void Game::displayCurrentNextWalkers(int iTimeEllapsedMs, SDL_Surface* dest, const walkers::const_iterator* poit) const
{
	moCurrentNext.displayCurrentNextWalkers(iTimeEllapsedMs, dest, poit);
}

void Game::burnMoney(Uint32 lMoney)
{
	mlBank -= lMoney;
	if (mlBank < 0)
	{
		cerr << "ERROR Should not arrive here (money less than 0)" << endl;
		mlBank = 0;
	}
}

void Game::drawTileCentered(SDL_Surface* dest, Uint32 iTileNumber, const coord &pos, const string sTileset) const
{
	if (mpoMap)
	{
		mpoMap->drawTileCentered(dest, iTileNumber, pos, sTileset);
	}
	else
		cout << "Game::drawTile without map " << endl;
}

void Game::drawMap(SDL_Surface* img, bool bFull)
{
	if (mpoMap)
		mpoMap->draw(img, coord(mGameArea.getTopLeft()), not bFull);
	else
	{
		static bool bFirst = true;
		if (bFirst)
		{
			bFirst = false;
			cerr << "**************************" << endl;
			cerr << "game has no map" << endl;
			cerr << "Please check error(s) above." << endl;
			cerr << "**************************" << endl;
		}
	}
}

bool Game::handleAction(string sAction)
{
	return false;
}

GameExtension* Game::createExtension(string sExtender)
{
	if (sExtender=="ogol_editor")
		return new OgolEditor;
	return nullptr;
}

bool Game::readThemeExtension(string& sItem, CFileParser *oDef)
{
	if (mpGameExtension)
	{
		return mpGameExtension->readTheme(sItem, oDef);
	}
	return false;
}

void Game::setState(EGAME_STATE newState)
{
	if (newState != miGameState)
	{
		if (newState == STATE_PLAY)
			InitializePathfinder();
		miGameState = newState;
	}
}
