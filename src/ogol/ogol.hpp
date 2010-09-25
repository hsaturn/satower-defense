/*
 * ogol.hpp
 *
 *  Created on: 29 juil. 2010
 *      Author: saturn
 */

#ifndef OGOL_HPP_
#define OGOL_HPP_

class CFileParser;
#include <SDL/SDL_gfxPrimitives.h>
#include <list>
#include <string>

using namespace std;

#include "../vars.hpp"
#include "../coord.hpp"

#ifndef PI
	#define PI (6.28318530718/2)
#endif

typedef enum
{
	OGOL_PU,
	OGOL_PD,
	OGOL_FW,
	OGOL_BW,
	OGOL_FWU,
	OGOL_BWU,
	OGOL_TR,
	OGOL_TL,
	OGOL_ANGLE,
	OGOL_CIRCLE,
	OGOL_ADD,
	OGOL_REPEAT,
	OGOL_MIRROR,
	OGOL_COLOR,
	OGOL_ROTATES,
	OGOL_FILLED,
	OGOL_OPACITY,
	OGOL_SETVAR,
	OGOL_SHOOTPOINT

} EOGOL_CMD;

class ogolPolygon;

class ogolPencil
{
public:
	ogolPencil()
		:
		mbDown(true),
		mfScale(1),
		miColor(0xFF),
		mpoPolygon(0),
		mbMirror(false),
		mbExplosion(false),
		miOpacityFactor(100)
		{}

	ogolPencil(bool bDown, float fScale=1)
		:
		mbDown(bDown),
		mfScale(fScale),
		miColor(0xFF),
		mpoPolygon(0),
		mbMirror(false),
		mbExplosion(false),
		miOpacityFactor(100)
		{}

	~ogolPencil();

	void down(){ mbDown=true; }
	void up() { mbDown=false; }
	float getScale() const;
	void setScale(float fScale) { mfScale=fScale;}
	bool setColor(const string &sColor);
	bool setColor(Uint32 iColor);

	void setOpacity(Uint16 iPercent);
	Uint16 getOpacityFF() {return miColor & 0xFF;};
	void setOpacityFactor(Uint16 iPercent) { miOpacityFactor=iPercent; }

	Uint32 getColor() const { return miColor;}

	bool isDown() const { return mbDown; }

	void recordPoint(coord* p, bool bWithLine);
	/**
	 * Start record a polygon and return the current one.
	 * The return value must be re-entered to endRecord.
	 * The return value *is not* the polygon recorded !
	 */
	ogolPolygon* recordPolygon();

	/**
	 * Stop record a polygon and draw it with the given colors.
	 */
	void endRecord(ogolPolygon* p, SDL_Surface* dst, Uint32 iFillColor);

	bool isRecordingPoly() { return mpoPolygon!=0; };

	void toggleMirror()	{	mbMirror=!mbMirror; };
	bool getMirror()	{	return mbMirror; };
	// return 0 if bad color
	static Uint32 stringToColor(const string &sColor, Sint16 iOpacity=100);
	coord getShootPoint() const { return moShootPoint; };
	void setShootPoint(const coord p) { moShootPoint=p; }

	void setExplosion(bool bExplosion) { mbExplosion=bExplosion; }
	bool getExplosion() const { return mbExplosion; }

/*	void addExplosion(ogolVectoroid* p);
	{ mlstExplosion.push_front(p);};
*/
	void drawExplosion(SDL_Surface*);

	void update(int iTimerEllapsedms);

private:
	bool mbDown;
	float mfScale;
	Uint32 miColor;
	ogolPolygon* mpoPolygon;
	bool mbMirror;
	coord moShootPoint;
	bool mbExplosion;
//	list<ogolVectoroid*>	mlstExplosion;
	Uint32 miOpacityFactor;
};

class ogolCommand
{
public:
	ogolCommand(EOGOL_CMD iOgolCommand, long iParam1=0, long iParam2=0)
	:
		miOgolCommand(iOgolCommand),
		miParam1(iParam1),
		miParam2(iParam2){};

	// Draw all
	void draw(SDL_Surface*, coord* poCurrent, coord* poAngle, ogolPencil &oPencil) const;
	EOGOL_CMD getCommand() const { return miOgolCommand; }
private:
	EOGOL_CMD miOgolCommand;	// TODO should be enum
	long miParam1;	// Fixme type should be compatable with pointer
	long miParam2;
};

/**
 * This class can read a small logo like file
 * and draw it.
 */

#define MAX_OGOL_VARS 20

class ogol
{
public:
	ogol(ogol* poParent=0);

	/**
	 * Read the instruction from a definition file.
	 * this method stop parsing when } is reached
	 */
	void readDef(CFileParser*);
	void draw(SDL_Surface* pDest,coord* poPosition,coord* poAngle,ogolPencil &oPencil) const;

	/**
	 * Create a float var and return its pointer
	 * @param sName name of the variable
	 * @return float ptr
	 * This method may throw if MAX_OGOL_VARS is reached.
	 */
	float* createVar(const string &sName);

	/**
	 * Get the mVars index of a var
	 * @see createVar
	 * @return the float pointer or 0
	 */
	float* getVar(const string &sName);

private:
	ogol*				mpoParent;
	list<ogolCommand*>	mlstOgol;
	map<string,float>	mmapVars;
	//float	mVars[MAX_OGOL_VARS];
};

#endif /* OGOL_HPP_ */
