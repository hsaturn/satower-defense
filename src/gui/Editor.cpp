#include <Editor.hpp>
#include <cfileparser.hpp>

Editor::Editor(CFileParser *poDef)
:
Widget(poDef), caret(8,16)
{
	try
	{
		poDef->getExpectedChar("{");
		while(poDef->good())
		{
			if (poDef->peekChar()=='}')
			{
				poDef->getExpectedChar("}");
				break;
			}
			string s=poDef->getNextIdentifier("button data");

			if (s=="area")
				mRect.readDef(poDef);
			else
				poDef->throw_("editor","Unknown data ["+s+"]");
		}
		// mlstButtons.push_front(this);
	}
	catch (CSException *p)
	{
		cerr << "Error while reading editor " << msName << endl;
		cerr << p->getCompleteError() << endl;
	}
}

void Editor::render(SDL_Surface* surface, Uint32 ellapsed)
{
	// TODO compute caret position (not here !)
	caret.setPos(mRect.x1(), mRect.y1());
	caret.render(surface, ellapsed);
	for(const auto [line,str] : lines)
	{

	}
}
