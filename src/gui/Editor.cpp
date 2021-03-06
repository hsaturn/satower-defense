#include <Editor.hpp>
#include <cfileparser.hpp>
#include <ogol.hpp>
#include "Canvas.hpp"
#include <ogol.hpp>

#include <sstream>
#include <fstream>
#include <walkerBase.hpp>

static const char* tmp="/tmp/test.ogol";	// TODO very ugly

Editor::Editor(CFileParser *poDef)
:
Widget(poDef), caret(8,16), mpFont(nullptr), mOgolCanvas(nullptr)
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
			else if (s=="canvas")
			{
				string sCanvas=poDef->getNextIdentifier("canvas name");
				mOgolCanvas = dynamic_cast<Canvas*>(Widget::search(sCanvas));
				if (mOgolCanvas==nullptr)
					poDef->throw_("Cannot find canvas "+sCanvas);
			}
			else if (s=="autoindent")
			{
				autoindent=true;
			}
			else if (s=="repeat")
			{
				delay=poDef->getNextLong("repeat delay");
				interval=poDef->getNextLong("repeat interval");
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
		caret.setSize(font_w-1,font_h);

		if (font_w) window_cols = (mRect.x2()-mRect.x1()) / font_w;
		if (font_h) window_rows = (mRect.y2()-mRect.y1()) / font_h;
		cout << "Editor rows=" << window_rows << ", cols=" << window_cols << endl;
	}

	top_row=1;
	left_col=3;
	row=1;
	col=0;
	updateCaret();
	updateOgol();
	readFile(tmp);
}

void Editor::renderText(SDL_Surface* surface, int line, const string& str)
{
	line -= top_row;
	if (line < 0) return;
	if (line > window_rows) return;

	if ((int)str.length()<left_col) return;

	coord pos(mRect.x1(), mRect.y1()+line*font_h);

	const char* p=str.c_str()+left_col;
	while(*p)
	{
		if (*p != ' ')
			mpFont->drawSolid(surface, pos, *p);
		pos.setX(pos.x()+font_w);
		p++;
		if (pos.x() > mRect.x2()) break;
	}
}

void Editor::render(SDL_Surface* surface, Uint32 ellapsed)
{
	// TODO compute caret position (not here !)
	caret.render(surface, ellapsed);

	for(const auto [line,str] : lines)
	{
		renderText(surface, line, str);
	}

	SDL_SetClipRect(surface, nullptr);
	coord pos(300,300);
	for(const auto& pWalker: mWalkers)
	{
		pWalker->update(ellapsed);
		pWalker->drawAt(pos, surface);
	}
}


void Editor::onKey(const SDL_KeyboardEvent& event)
{
	switch (event.keysym.sym)
	{
		case SDLK_INSERT:
			caret.setInsertMode(!caret.isInsertMode());
			break;

		case SDLK_BACKSPACE:
			if (col>=1)
			{
				col--;
				lines[row].erase(col,1);
				break;
			}
			if (row<=0)
				break;
			row--;
			col=lines[row].length();

		case SDLK_DELETE:
			{
				if ((size_t)col < lines[row].length())
				{
					lines[row].erase(col,1);
					break;
				}
				lines[row]+=lines[row+1];

				int current=row+1;
				int last_row = lines.rbegin()->first;
				while(current < last_row)
				{
					lines[current]=lines[current+1];
					current++;
				}
				lines.erase(current);
				break;
			}

		case SDLK_RIGHT:
			col++;
			if (col > (int)lines[row].length())
				col--;
			break;

		case SDLK_LEFT:
			col--;
			if (col<0) col=0;
			break;

		case SDLK_UP:
			row--;
			if (row<0) row=0;
			virtualCol();
			break;

		case SDLK_DOWN:
			if (lines.size())
			{
				row++;
				if (row > lines.rbegin()->first)
					row--;
				virtualCol();
			}
			break;

		case SDLK_END:
			col=lines[row].length();
			break;

		case SDLK_HOME:
			col=0;
			break;

		case SDLK_RETURN:
			{
				lines[row].length();
				string indent;
				if (autoindent)
				{
					indent=lines[row];
					auto nspc=indent.find_first_not_of(' ');
					if (nspc!=string::npos)
						indent.erase(nspc);
					if (col>=1 && lines[row][col-1]=='{')
						indent+="  ";
				}
				if (!caret.isInsertMode())
				{
					row++;
					col=0;
				}
				else
				{
					string pre;
					int last_row = lines.rbegin()->first;
					if (caret.isInsertMode())
					{
						pre=indent+lines[row].substr(col, string::npos);
						lines[row].erase(col, string::npos);
					}
					row++;
					int current=row;
					while(current <= last_row)
					{
						std::swap(lines[current],pre);
						current++;
					}
					lines[current]=pre;
					col=indent.length();
				}
			}
			break;

		default:
			char c=(char)event.keysym.unicode;

			if (c)
			{
				if (autoindent && (c=='}' && col>=2 && lines[row].substr(col-2,2)=="  "))
				{
					lines[row].erase(col-2,2);
					col -=2;
				}
				if (caret.isInsertMode())
				{
					lines[row].insert(col,1,c);
				}
				else
				{
					lines[row].replace(col,1,1,c);
				}
				col++;
			}
			// cout << event.keysym.unicode << ", [" << (char)event.keysym.unicode << "]" << endl;
			break;
	}
	updateCaret();
	updateOgol();
}

void Editor::updateCaret()
{
	ensureCaretVisible();
	int x,y;
	x=mRect.x1()+font_w*(col-left_col);
	y=mRect.y1()+font_h*(row-top_row);
	caret.setPos(x,y);
}

void Editor::ensureCaretVisible()
{
	while (col >= left_col+window_cols) left_col++;
	while (col < left_col) left_col--;
	if (left_col<0) left_col=0;

	while (row >= top_row + window_rows) top_row ++;
	while (row < top_row) top_row --;
	if (top_row<0) top_row=0;
}

void Editor::onFocus()
{
	SDL_EnableKeyRepeat(delay, interval);
	unicode = SDL_EnableUNICODE(1);
}

void Editor::onLeaveFocus()
{
	SDL_EnableKeyRepeat(0,0);
	SDL_EnableUNICODE(unicode);
}

void Editor::virtualCol()
{
	if ((size_t)col<=lines[row].length()) return;
	if (virtual_col>=0 && (size_t)virtual_col<=lines[row].length())
	{
		col=virtual_col;
		virtual_col=-1;
	}
	else
	{
		virtual_col=col;
		col=lines[row].length();
	}
}

void Editor::onMouse(const SDL_Event& event, const coord& relative)
{
	if (event.type==SDL_MOUSEBUTTONDOWN)
	{
		col=relative.x() / font_w + left_col;
		row=relative.y() / font_h + top_row;

		if ((size_t)col > lines[row].length())
			col=lines[row].length();
		updateCaret();
	}
}

void Editor::updateOgol()
{
	string s=getText();
	{
		ofstream f(tmp);
		f << s;
	}
	CFileParser parser(tmp, true, true);


	mWalkers.clear();
	try
	{
		while(parser.good())
		{
			try
			{
				parser.checkExpectedString("walker");

				walkerBase* pWalker=walkerBase::readOneFromDef(&parser);
				mWalkers.push_back(pWalker);
			}
			catch(CSException *p)
			{
				break;
			}
		}
	}
	catch(CSException *p)
	{
		cerr << p->getCompleteError();
	}
	cout << "Walkers : " << mWalkers.size() << endl;
}

string Editor::getText() const
{
	stringstream s;
	for(const auto& [line, str] : lines)
	{
		s << str << endl;
	}
	return s.str();
}

void Editor::readFile(string sFileName)
{
	ifstream f(tmp);
	lines.clear();
	int row=1;
	while(f.good())
	{
		getline(f, lines[row++]);
	}
}
