/*
 * Game.hpp
 *
 *  Created on: 27 juil. 2010
 *      Author: saturn
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <cfileparser.hpp>
#include "coord.hpp"
#include "rect.hpp"
#include "tiled/tiledMap.hpp"
#include "CurrentNextArea.hpp"
#include "walkers.hpp"
#include "walkerBase.hpp"

class Sound;
class Button;

extern int tileSizeX;
extern int tileSizeY;

#include <list>
#include <string>

using namespace std;

class text;
class towerBase;

typedef enum
{
	STATE_OVER,
	STATE_PLAY,
	STATE_PAUSED,
	STATE_SHOW_HISCORE,
	STATE_ENTER_HISCORE
} EGAME_STATE;

class Game
{
public:
	Game();
	~Game();
	
	static void setRsrcPath(const string& sPath) { msRsrcPath=sPath; }

	/**
	 * Find a resource file.
	 * If bTheme, the file is searched inside the current theme.
	 */
	string findRsrcFile(const string &sFileName, bool bTheme=false);

	void changeLevel(const string &sLevel);

	text*	getSmallText()  { return mpoFontSmall; }
	text*	getButtonText()	{ return mpoFontButton; }
	text*	getHudgeText()	{ return mpoFontHudge; }
	/**
	 * Read ini parameters.
	 * sIniFile has no path
	 */
	void readTheme(const string& sIniFile);

	/**
	 * Read the whole game configuration
	 * - walkers
	 * - weapons
	 * - towers
	 * etc
	 * @param sDefFileName definitino file (path+name)
	 */
	void readConfig(const string& sDefFileName);

	void readTowersAreas(CFileParser*);

	void printScores(SDL_Surface*);

	void deleteMusic();
	void changeMusic(string sMusic="");

	void burnLives(int iKills);
	void burnMoney(Uint32 lMoney);

	int getTileSizeX() const { return tileSizeX; }
	int getTileSizeY() const { return tileSizeY; }

	void displayTowerInfos(SDL_Surface*,const towerBase*) const;
	
	void displayWalkerInfos(SDL_Surface*, const walkerBase*) const;
	/**
	 * Draw the map on the surface
	 * @param SDL_Surface* where to draw the map
	 * @param bFull true if all layers are drawn, else
	 */
	void drawMap(SDL_Surface*, bool bFull);

	/**
	 * Draw a tile so the center of the tile is at pos.
	 */
	void drawTileCentered(SDL_Surface* dest, Uint32 iTileNumber, const coord &pos, const string sTileset) const;

	void nextLevel() { mlLevel++; }

	long	mlScore;
	long	mlLives;
	long	mlBank;
	long	mlInterest;
	long	mlLevel;
	long	mlBonus;

	coord& getCoordScore()	{	return mCoordScore; }
	float getOgolScale() const { return mfOgolScale; }

	rect& getMapArea() { return mGameArea; }
	float getHealthFactor() { return mfHealthFactor; }

	void addStartTile(const coord& p) { mlstStartTiles.push_front(p); };
	void addEndTile(const coord& p)		{ mlstEndTiles.push_front(p); };
	void setHealthFactor(float fFactor) { mfHealthFactor=fFactor; }

	list<coord>::const_iterator beginStarts	() const { return mlstStartTiles.begin(); }
	list<coord>::const_iterator endStarts	() const { return mlstStartTiles.end(); }

	list<coord>::const_iterator beginEnds	() const { return mlstEndTiles.begin(); }
	list<coord>::const_iterator endEnds		() const { return mlstEndTiles.end(); }

	int startsCount() const { return mlstStartTiles.size(); }
	int endsCount() const { return mlstEndTiles.size(); }

	void displayCurrentNextWalkers(int iTimeEllapsedMs,SDL_Surface*,const walkers::const_iterator*) const;
	void bossAdd() { miBossCount++; };
	void bossDie();
	void bossDieEndMusic();	// Fade out current music if bosscount==1
	int  bossCount() const { return miBossCount; }
	EGAME_STATE state() const { return miGameState; }

	bool togglePause();

private:
	static string msRsrcPath;
	Mix_Music*	mpoMusic;
	int			miMusicChannel;
	string	msLevel;
	string	msFileNameImage;
	text*	mpoFontSmall;
	text*	mpoFontButton;
	text*	mpoFontHudge;
	coord	mCoordBank;
	coord	mCoordInterest;
	coord	mCoordScore;
	coord	mCoordLives;
	coord	mCoordLevel;
	coord	mCoordBonus;
	rect	mGameArea;
	rect	mTowersArea;	// Towers pick area

	list<Button*>	mlstButtons;

	tiledMap	*mpoMap;
	string	msThemePath;

	list<coord>		mlstStartTiles;
	list<coord>	mlstEndTiles;
	CurrentNextArea	moCurrentNext;
	rect	mTowersInfoArea;	// Towers display info area
	rect	mTowerNameArea;		// Towers name display area
	rect	mTowerCellArea;
	text*	mpTowerFont;
	float	mfHealthFactor;	// Health factor for spawned walkers
	string	msMusicFile;
	float	mfOgolScale;
	string	msCurrentMusic;
	int		miBossCount;	// Number of alive bosses.
	Sound*	mpoLiveLostSound;	// Sound when a live is burning
	EGAME_STATE	miGameState;
};
#endif /* GAME_HPP_ */
