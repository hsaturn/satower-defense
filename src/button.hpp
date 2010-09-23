/*
 * button.hpp
 *
 *  Created on: 4 ao�t 2010
 *      Author: saturn
 */

#ifndef BUTTON_HPP_
#define BUTTON_HPP_
#include <cfileparser.hpp>
#include "rect.hpp"

#include <string>

using namespace std;

#include "coord.hpp"

class Button
{
public:
	Button();
	virtual ~Button();

	void readDef(CFileParser*);
	const string getAction() const { return msAction; }
	const rect getRect() const { return mRect; }

	bool isInside(coord c) const { return mRect.isInside(c); }

	void highlight(SDL_Surface *dest) const;
	/**
	 * Search for a button under a specific coord
	 * (for example the mouse)
	 * @return Button* or 0
	 */
	static Button* searchButton(coord cMouse);
private:
	string	msName;
	rect	mRect;
	string	msAction;

	static list<Button*>	mlstButtons;
};
#endif /* BUTTON_HPP_ */
