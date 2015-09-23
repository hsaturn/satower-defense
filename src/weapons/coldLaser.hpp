/*
 * coldMissile.hpp
 *
 *  Created on: 12 august 2010
 *      Author: saturn
 */
 
 #ifndef COLDMISSILE_HPP_
 #define COLDMISSILE_HPP_
 
 #include "missileBase.hpp"
 
 class coldLaser : public missileBase
 {
 public:

	 coldLaser();
 	virtual ~coldLaser();

 	virtual void drawAt(const coord &p, SDL_Surface*) const;
 	virtual int update(int iTimeEllapsedms);
 	virtual missileBase* clone();

 	void readFromDef(CFileParser*){};
	virtual const string getDamageInfo(const towerBase* pFromTower) const;

 	virtual bool isTargetable(walkerBase*);

 private:
 	mutable int	  miChangeLaser;	// Nr of ms left before changing laser appearance.
 	mutable float mfAlpha0;
 	mutable float mfAlpha1;
 	mutable float mfRatioLength0;
 	mutable float mfRatioLength1;
 	mutable bool  mbLrl;// Left Right Left for drawing the laser
 	float	mfMaxSlow;
 	ogol*	mogol;
 };
 
 #endif
