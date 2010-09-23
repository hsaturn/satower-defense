/*
 * CWindow.hpp
 *
 *  Created on: 10 ao�t 2010
 *      Author: saturn
 */

#ifndef CWINDOW_HPP_
#define CWINDOW_HPP_

#include <SDL/SDL.h>

/**
 * Base class for windows. This is a simple rect that
 * can handle some events.
 */
class CWindow
{
public:
	CWindow(const SDL_Rect &oRegion, Uint32 iBackgroundColor)
		: moRegion(oRegion), miBgColor(iBackgroundColor){}

	CWindow(){};

	virtual ~CWindow(){};

	virtual void draw(SDL_Surface*);

	/**
	 * Get the 'drawable' region of the window.
	 */
	virtual SDL_Rect getInnerRegion() const { return moRegion; }

	/**
	 * Get the region occupied by the whole window if the window
	 * is decorated (scrollbars, border, title bar etc).
	 */
	virtual SDL_Rect getOuterRegion() const { return moRegion; }
private:
	SDL_Rect	moRegion;
	Uint32		miBgColor;
};

/**
 * Abstract decorator class for windows
 * We can enhance windows with any kind/number of decorations.
 */
class CWindowDecorator : public CWindow
{
public:
	CWindowDecorator(CWindow* pWindow)
	:
		mpoWindow(pWindow) {}

	virtual ~CWindowDecorator(){ if (mpoWindow) delete mpoWindow; }

	virtual SDL_Rect getInnerRegion() const { return mpoWindow->getInnerRegion(); };

	/*
	 * The decorator should to implement getOuterRegion to
	 * consider its decorations.
	 */
	virtual SDL_Rect getOuterRegion() const { return mpoWindow->getOuterRegion(); };

	virtual void draw(SDL_Surface*) const=0;

protected:
	CWindow*	mpoWindow;
};

/**
 * And here is the very first window decorator
 * A single border line.
 *
 * For example, a red border 2 pixel window.
 *
 * CWindowBorder*p = new CWindowBorder(new CWindow(rect),2,0xFF0000);
 */

class CWindowBorder : public CWindowDecorator
{
public:
	CWindowBorder(CWindow* pWindow, Sint16 iBorderSize, Uint32 iColor)
	: CWindowDecorator(pWindow), miBorderSize(iBorderSize),miColor(iColor){};

	virtual SDL_Rect getOuterRegion() const;
	virtual void draw(SDL_Surface*) const;

private:
	Sint16	miBorderSize;
	Uint32	miColor;
};

/**
 * FIXME This class does not worrk
 * Here is a second simple decorator that allow a whole window
 * to become transparent.
 *
 * Example
 * CWindowTranparent* p=new CWindowTransparent(new CWindow(rect),50);
 */
class CWindowTransparent : public CWindowDecorator
{
public:
	/**
	 * @param Sint16 transparency (0..100 for transparent->opaque)
	 */
	CWindowTransparent(CWindow* pWindow, Sint16 iTransparency)
	:
		CWindowDecorator(pWindow),
		miTransparency(iTransparency){};

	virtual void draw(SDL_Surface*) const;
private:
	Sint16 miTransparency;
};
#endif /* CWINDOW_HPP_ */
