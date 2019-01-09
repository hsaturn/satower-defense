#include <Editor.hpp>
#include <cfileparser.hpp>
#include <ogol.hpp>

Editor::Editor(CFileParser *poDef)
:
Widget(poDef), caret(8,16), mpFont(nullptr)
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
			{
				mRect.readDef(poDef);
			}
			else if (s=="caret")
			{
				string sColor=poDef->getNextIdentifier("color");

				caret.setColor(ogolPencil::stringToColor(sColor));
			}
			else if (s=="font")
			{
				if (mpFont==nullptr)
				{
					mpFont=new text(poDef);
				}
				else
					poDef->throw_("Only one font allowed per editor");
			}
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
	if (mpFont)
	{
		mpFont->textSize("W", font_w, font_h);
		caret.setSize(font_w,font_h);

		if (font_w) window_cols = (mRect.x2()-mRect.x1()) / font_w;
		if (font_h) window_rows = (mRect.y2()-mRect.y1()) / font_h;
		cout << "Editor rows=" << window_rows << ", cols=" << window_cols << endl;
	}

	top_row=0;
	left_col=0;
	lines[1]="wwaouh";
}

void Editor::renderText(SDL_Surface* surface, int line, const string& str)
{
	if (line < top_row) return;
	if (line >= top_row + window_rows) return;

	if (str.length()<left_col) return;

	coord pos(mRect.x1()+line*font_h, mRect.y1());

	const char* p=str.c_str()+left_col;
	while(*p)
	{
		if (*p != ' ')
			mpFont->drawSolid(surface, pos, *p);
		pos.setX(pos.x()+font_w);
		p++;
	}
	cout << endl;
}

void Editor::render(SDL_Surface* surface, Uint32 ellapsed)
{
	// TODO compute caret position (not here !)
	caret.setPos(mRect.x1(), mRect.y1());
	caret.render(surface, ellapsed);

cout << "----------" << endl;
	for(const auto [line,str] : lines)
	{
		cout << line << ':' << str << endl;
		renderText(surface, line, str);
	}
}

