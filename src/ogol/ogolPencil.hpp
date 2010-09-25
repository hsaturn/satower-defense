/*
 * ogolPencil.hpp
 *
 *  Created on: 2 ao�t 2010
 *      Author: saturn
 */

#ifndef OGOLPENCIL_HPP_
#define OGOLPENCIL_HPP_

class ogolVectoroid;
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
	static float getScale() const;
	void setScale(float fScale) { mfScale=fScale;}
	bool setColor(const string &sColor);
	bool setColor(Uint32 iColor);

	void setOpacity(Uint16 iPercent);
	void setOpacityFactor(Uint16 iPercent) { miOpacityFactor=iPercent; }
	Uint32 getColor() const { return miColor;}

	bool isDown() const { return mbDown && mpoPolygon==0; }

	inline void recordPoint(coord* p, bool bWithLine);
	/**
	 * Start record a polygon and return the current one.
	 * The return value must be re-entered to endRecord.
	 * The return value *is not* the polygon recorded !
	 */
	ogolPolygon* recordPolygon()
	{
		ogolPolygon* pOldOne(mpoPolygon);
		mpoPolygon=new ogolPolygon;
		return pOldOne;
	}

	/**
	 * Stop record a polygon and draw it with the given colors.
	 */
	void endRecord(ogolPolygon* p, SDL_Surface* dst, Uint32 iColor, Uint32 iFillColor)
	{
		mpoPolygon->draw(dst,iColor,iFillColor);
		delete mpoPolygon;
		mpoPolygon=p;
	}

	inline bool isRecordingPoly() { return mpoPolygon!=0; };

	void toggleMirror()	{	mbMirror=!mbMirror; };
	inline bool getMirror()	{	return mbMirror; };
	// return 0 if bad color
	static Uint32 stringToColor(const string &sColor);
	coord getShootPoint() const { return moShootPoint; };
	void setShootPoint(const coord p) { moShootPoint=p; }

	void setExplosion(bool bExplosion) { mbExplosion=bExplosion; }

	// void addExplosion(ogolVectoroid* p){ mlstExplosion.push_front(p);};

	// void drawExplosion(SDL_Surface*);

	void update(int iTimerEllapsedms);
UNUSED CODE SEE OGOL.HPP
private:
	bool mbDown;
	float mfScale;
	Uint32 miColor;
	ogolPolygon* mpoPolygon;
	bool mbMirror;
	coord moShootPoint;
	bool mbExplosion;
	list<ogolVectoroid*>	mlstExplosion;

};

#endif /* OGOLPENCIL_HPP_ */
