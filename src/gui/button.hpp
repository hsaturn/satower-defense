/*
 * button.hpp
 *
 *  Created on: 4 ao�t 2010
 *      Author: saturn
 */
#pragma once

#include <cfileparser.hpp>
#include "rect.hpp"
#include <Widget.hpp>

#include <string>

using namespace std;

#include "coord.hpp"

class Button : public Widget
{
public:
	Button(CFileParser*);
	virtual ~Button() {};

	const string getAction() const { return msAction; }

	void highlight(SDL_Surface *dest) const;
private:
	string	msAction;

};
