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
	Widget* pWidget = Widget::search(coord(event));
	if (pWidget)
	{
	}
	return pWidget;
}

void Widget::renderAll(SDL_Surface* surface, Uint32 ellapsed)
{
	for(auto widget: mlstWidgets)
		widget->render(surface, ellapsed);
}
