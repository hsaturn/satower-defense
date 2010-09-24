/*
 * bulletMissile.cpp
 *
 *  Created on: 8 ao�t 2010
 *      Author: saturn
 */

#include "bulletMissile.hpp"
#include "../walkerBase.hpp"
#include "../towerBase.hpp"
#include "../game.hpp"
#include <cmath>

bulletMissile::bulletMissile()
:
missileBase(),
mbEnhancedSystem(false),
mfSpinFactor(0.02)
{
}

bulletMissile::~bulletMissile()
{

}

missileBase* bulletMissile::clone()
{
	bulletMissile* p=new bulletMissile();

	fillBaseClone(p);
	p->mbEnhancedSystem=mbEnhancedSystem;
	p->mfSpinFactor=mfSpinFactor;

	return p;
}

bool bulletMissile::readSpecificDefItem(const string &sItem, CFileParser* poDef)
{
	bool bRet=true;

	if (sItem=="enhanced_targeting")
	{
		mbEnhancedSystem=true;
	}
	else if (sItem=="spin_factor")
	{
		mfSpinFactor=poDef->getNextLong("spin %")/1000.0;
		if (mfSpinFactor<0.005 || mfSpinFactor>0.1)
			poDef->throw_("Spin factor must be between 5 and 100");
	}
	else
		bRet=false;
	return bRet;
}


void bulletMissile::drawAt(const coord& p, SDL_Surface *dest) const
{
	if (mpogol)
	{
		coord oDirection(mDirection);
		oDirection.normalize();
		coord oPos(mpos);
		mpogol->draw(dest,&oPos,&oDirection,moPencil);
	}
	else
	{
		// cout << "bullet missile draw ! " << " speed " << mfSpeed << mDirection << endl;
		coord x2(mpos);
		x2.add(mDirection);
		x2.add(coord(0,-2));
		aalineColor(dest,mpos.x(),mpos.y(),x2.x(),x2.y(),miColor);
		x2=mpos;
		x2.add(coord(2,0));
		aalineColor(dest,mpos.x(),mpos.y(),x2.x(),x2.y(),miColor);
		x2=mpos;
		x2.add(coord(-2,0));
		aalineColor(dest,mpos.x(),mpos.y(),x2.x(),x2.y(),miColor);
		x2=mpos;
		x2.add(coord(0,2));
		aalineColor(dest,mpos.x(),mpos.y(),x2.x(),x2.y(),miColor);
	}
}

const string bulletMissile::getDamageInfo(const towerBase* pFromTower) const
{
	string s="Rockets, "+to_string(pFromTower->getDamage())+" hp\n";
	s+="Speed   : "+to_string((int)getSpeed())+" m/s\n";
	s+="Max fly : "+to_string(miMaxTimems/1000)+" sec.";
	if (mbEnhancedSystem)
		s+"\nEnhanced system";
	return s;
}

float bulletMissile::getSpeed() const
{
	return mfSpeed*gpGame->getOgolScale();
}

int bulletMissile::update(int iTimeEllapsedms)
{
	if (iTimeEllapsedms==0)
		return false;
	bool bRet=missileBase::update(iTimeEllapsedms);
	if (miMaxTimems>0 && miMaxTimems<1000)
	{
		moPencil.setOpacityFactor(miMaxTimems/10);
	}
	if (!bRet)
	{
		coord oDirection;	// Best direction
		float bFalloutDirection=false;

		if (mbEnhancedSystem)
		{
			coord W=mpTarget->getDirection();	// W speed vector of the target
			W.normalize();
			W.scalarMultiply(mpTarget->getSpeed());

			coord D=mpTarget->getShootPoint();
			D.substract(getCoord());			// D vector (distance..) missile->target
			if (abs(D.y())>abs(D.x()))			// CASE 1
			{
				float Z=D.y()*W.x()-D.x()*W.y();	// Z intermediate computation val
				float a=D.x()*D.x()+D.y()*D.y();
				float b=2*D.x()*Z;
				float c=Z*Z-D.y()*D.y()*getSpeed()*getSpeed();
				float delta=b*b-4*a*c;
				if (delta<0)
					bFalloutDirection=true;
				else
				{
					float sq=sqrt(delta);
					float my1=(-b-sq)/(2*a);
					float t1=D.y()/(my1-W.y());
					float my2=(-b+sq)/(2*a);
					float t2=D.y()/(my2-W.y());
					float mx,my=my1;
					//cout << (my1-W.y()) << endl;
					//cout << (my2-W.y()) << endl;
					//cout << "t1=" << t1 << " t2=" << t2 << endl;
					float t=t1;
					if ((t1<0) or ((t2>0) && (t2<t1)))
					{
						t=t2;
						my=my2;
					}

					if (t<0)
					{
						bFalloutDirection=true;
					}

					mx=(D.x()+t*W.x())/t;

					oDirection=coord(mx,my);
				}
			}
			else // CASE 2
			{
				float Z=D.x()*W.y()-D.y()*W.x();	// Z intermediate computation val
				float a=D.x()*D.x()+D.y()*D.y();
				float b=2*D.y()*Z;
				float c=Z*Z-D.x()*D.x()*getSpeed()*getSpeed();
				float delta=b*b-4*a*c;
				if (delta<0)
					bFalloutDirection=true;
				else
				{
					float sq=sqrt(delta);
					float mx1=(-b-sq)/(2*a);
					float t1=D.x()/(mx1-W.x());
					float mx2=(-b+sq)/(2*a);
					float t2=D.x()/(mx2-W.x());
					float my,mx=mx1;
					//cout << (mx1-W.x()) << endl;
					//cout << (mx2-W.x()) << endl;
					//cout << "t1=" << t1 << " t2=" << t2 << endl;
					float t=t1;
					if ((t1<0) or ((t2>0) && (t2<t1)))
					{
						t=t2;
						mx=mx2;
					}

					if (t<0)
					{
						bFalloutDirection=true;
					}

					my=(D.y()+t*W.y())/t;

					oDirection=coord(mx,my);
				}
			}
		}
		else
			bFalloutDirection=true;

		if (bFalloutDirection)
		{
			oDirection=mpTarget->getShootPoint();
			oDirection.substract(getCoord());
		}

		float fFilter=1-mfSpinFactor*(float)iTimeEllapsedms/100.0;
		mDirection.setX(mDirection.x()*fFilter+(oDirection.x()*(1.0-fFilter)));
		mDirection.setY(mDirection.y()*fFilter+(oDirection.y()*(1.0-fFilter)));

		//mDirection=oDirection;
		mDirection.normalize();
		mDirection.scalarMultiply(getSpeed()*iTimeEllapsedms/1000);

		// mDirection is the direction and the distance of the missile displacement.
		// We check if the segment [mpos, mpos+distance] was close to the target.
		// Checking only distance between mpos-target is not enough due to fps reasons.
		// Without that, missiles passes through the target and try after to turn back to the
		// target, resulting in tons of missiles tracking their targets.
		// The equation of the line is ax+by+c=0 (in order to handle vertical line)
		// The distance d=abs(a.x1+b.y1+c)/sqrt(a2+b2) with P(x1,y1)=shoot point

		mpos.add(mDirection);
		coord oDistance=mpTarget->getShootPoint();
		oDistance.substract(getCoord());
		if (oDistance.norm()<8)
		{
			mpTarget->damage(mpoTower->getDamage());	// FIXME
			mbJobFinished=true;
		}
		bRet= mbJobFinished | mpTarget->isDead() | (mpTarget->getVisible()==false);
	}
	return bRet;
}

void bulletMissile::readFromDef(CFileParser*)
{
}
