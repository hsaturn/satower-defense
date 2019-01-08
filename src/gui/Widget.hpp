#pragma once

#include <list>
#include <rect.hpp>

class CFileParser;

class Widget
{
	public:
		Widget(CFileParser *poDef);
		virtual ~Widget();

		bool isInside(coord c) const { return mRect.isInside(c); }
		const rect getRect() const { return mRect; };

		virtual void highlight(SDL_Surface *dest) const {};

		/**
		 * Search for a button under a specific coord
		 * (for example the mouse)
		 * @return Button* or 0
		 */
		static Widget* search(coord cMouse);
		static Widget* search(string sName);

	protected:
		string msName;
		rect mRect;

	private:
		static list<Widget*>	mlstWidgets;
};
