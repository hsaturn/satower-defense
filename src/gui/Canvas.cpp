#include <Canvas.hpp>
#include <cfileparser.hpp>

Canvas::Canvas(CFileParser *poDef) : Widget(poDef)
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
