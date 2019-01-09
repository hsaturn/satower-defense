#pragma once

#include <SDL/SDL.h>
#include <list>
#include <rect.hpp>

class CFileParser;

class Widget
{
	public:
		Widget(string sName);
		Widget(CFileParser *poDef);
		virtual ~Widget();

		bool isInside(coord c) const { return mRect.isInside(c); }
		const rect getRect() const { return mRect; };

		virtual void highlight(SDL_Surface *dest) const {};
		virtual void onFocus() {}
		virtual void onLeaveFocus() {}
		virtual void onKey(const SDL_KeyboardEvent& event) {};
		virtual void onMouse(const SDL_Event& event, const coord& relative) {};
		virtual void render(SDL_Surface*, Uint32 ellapsed) {}

		/**
		 * Search for a button under a specific coord
		 * (for example the mouse)
		 * @return Button* or 0
		 */
		static Widget* search(coord cMouse);
		static Widget* search(string sName);
		static void renderAll(SDL_Surface*, Uint32 ellapsed);

		// return a widget or nullptr
		// in case of many widget concerned, only one is returned (...)
		static Widget* handleEvent(const SDL_Event&);

	protected:
		string msName;
		rect mRect;

	private:
		static list<Widget*>	mlstWidgets;
};
