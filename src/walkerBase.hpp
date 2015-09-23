/*
 * walkerBase.hpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#ifndef WALKERBASE_HPP_
#define WALKERBASE_HPP_

#include "mapelt.hpp"
#include "ogol.hpp"

class Sound;
class CFileParser;


extern coord coordToPixels(coord o);

#define DEFAULT_ROTATION_SPEED (3.1416/2)
class walkerBase : public mapelt
{
	public:
		walkerBase(const walkerBase*);
		walkerBase();
		// float fRotationSpeed, rad per sec.
		walkerBase(float fHealth,coord pos, float speed, coord dest, float fRotationSpeed=DEFAULT_ROTATION_SPEED);
		virtual ~walkerBase();

		/**
		 * Reads a walker from a file parser.
		 * The 'walker' keyword has been parsed and
		 * the file parser is about to get the name of the walker.
		 * Once finished, the parser is after the } of the walker.
		 * @param CFileParser* poDef the file parser containing a walker definition
		 */
		static void readOneFromDef(CFileParser*);
		virtual void drawAt(const coord &p, SDL_Surface *) const;

		void drawNative(const coord &p, SDL_Surface*, const coord &oAngle, float fScale) const;
		virtual int update(int iTimeEllapsedms);

		float getHealth() const { return mfHealth; };
		float getInitialHealth() const { return mfInitialHealth; }
		void setHealth(float fHealth) { mfHealth=fHealth; mfInitialHealth=fHealth; }
		int	getRefCount() const { return miRefCount; };
		bool getVisible() const { return mbVisible; }
		void setVisible(bool bVisible){mbVisible=bVisible; }

		bool isDead() const { return mfHealth<=0; }

		void incRef()	{	miRefCount++; }
		void decRef()	{	miRefCount--; }

		void setSoundDie(Sound* poSound){mpoSoundDie=poSound;}

		void damage(float fHealth);

		/**
		 * Slow the walker by slow value
		 * Can go under fMaxCoeff (%)
		 * if fSlowPercent is 0, then only a check is done.
		 * @param fSlowPercent amount of speed decrease
		 * @param fMinPercent min % of slowing to reach
		 * @return true if lowest speed reached.
		 */
		bool damageSpeed(float fSlowPercent, float fMinPercent);

		void setDirection(coord direction);
		void setDestination(coord dest, const string sFrom);
		coord getDestination() const { return mDest; }
		coord getDestinationPixels() const { return coordToPixels(mDest); }
		float getSpeed() const;
		float getMaxSpeed() const { return mfInitialSpeed; }

		long getBank() const { return mlBank; }
		const coord getDirection() const { return mDirection; }
		/**
		 * @return true if path is ok
		 */
		bool computePath(const string sFrom);

		const string getMusic() const { return msMusic; }

		long getMaxCount() const { return miMaxCount; }
		bool isBoss() const { return mbBoss; }

		// Reset path pre computed data
		// When the terran change for example
		static void resetClosedList();
		const coord& getShootPoint() const { return moShootPoint; }

		const string& getShortDesc() const { return msShortDesc; }
	private:
		float	mfHealth;
		float	mfInitialHealth;
		coord	mDest;		// Current destination of the walker (tile, not pixels) (norm=1)
		float	mfSpeed;	// pixels per milliseconds
		int		miRefCount;	// Nr of missile seeking this walker
		mutable bool	mbVisible;	// True if visible, false if out (can be deleted if miRefCount=0)
		int		miPathId;	// Id of a start path.
		coord	mDirection;
		coord	moAngle;	// Angle for drawing the walker
		float	mfRotationSpeed;	// Rotation of the walker rad/sec
		Sound*	mpoSoundDie;
		ogol	mogol;		// How to draw a walker (should be a drawer)
		mutable ogolPencil	moPencil;
		float	mfScale;
		bool	mbFollowDir;	// True if sprite direction follow the path orientation
		mutable int		miExplosion;	// Number of ms to display an explosion, -1 to start explosion
		long	mlBank;		// Amout of money earned when destroyed
		long	mlLifeTime;
		float	mfInitialSpeed;
		int		mlReleaseSlow;	// ms left to allow re-acceleration
		float	mfSpin;			// 1 if W->E -1 if W<-E when not mbFollowDir
		bool	mbAutoRotateSpin;
		mutable coord	moShootPoint;
		string	msShortDesc;
		int		miMaxCount;
		bool	mbBoss;
		string 	msMusic;
		long	miKills;		// lives spent when walker reaches out

	private:
		walkerBase(const walkerBase&);
		walkerBase operator = (const walkerBase&);
};

#endif /* WALKERBASE_HPP_ */
