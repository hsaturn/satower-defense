#include <Widget.hpp>
#include <cfileparser.hpp>

list<Widget*>	Widget::mlstWidgets;

Widget::Widget(string sName)
:
msName(sName)
{
	mlstWidgets.push_front(this);
}

Widget::Widget(CFileParser* poDef)
{
	try
	{
		msName = poDef->getNextIdentifier("widget name");
		mlstWidgets.push_front(this);
	}
	catch(CSException *p)
	{
		cerr << "Error while reading widget " << msName << endl;
		cerr << p->getCompleteError() << endl;
	}
}

Widget::~Widget()
{
	mlstWidgets.remove(this);
}


Widget* Widget::search(coord c)
{
	for(auto widget : mlstWidgets)
		if (widget->isInside(c))
			return widget;
	return nullptr;
}

Widget* Widget::search(string sName)
{
	for(auto widget: mlstWidgets)
		if (widget->msName == sName)
			return widget;
	return nullptr;
}

Widget* Widget::handleEvent(const SDL_Event& event)
{
	coord lastMouse;

	static Widget* lastFocus=nullptr;
	switch (event.type)
	{
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			{
				lastMouse=coord(event);
				Widget* newFocus=search(lastMouse);
				if (newFocus != lastFocus)
				{
					if (lastFocus) lastFocus->onLeaveFocus();
					if (newFocus) newFocus->onFocus();
					lastFocus = newFocus;
				}
				if (lastFocus)
				{
					coord relative(lastMouse);
					coord sub(lastFocus->mRect.x1(), lastFocus->mRect.y1());
					relative.substract(sub);
					lastFocus->onMouse(event, relative);
				}
				break;
			}

		case SDL_KEYDOWN:
			if (lastFocus)
			{
				SDL_KeyboardEvent* key_event=(SDL_KeyboardEvent*)(&event);
				lastFocus->onKey(*key_event);
			}
			break;

		default:
			cout << "Unhandled event " << event.type << endl;
	}
	return lastFocus;
}

void Widget::renderAll(SDL_Surface* surface, Uint32 ellapsed)
{
	for(auto widget: mlstWidgets)
		widget->render(surface, ellapsed);
}
