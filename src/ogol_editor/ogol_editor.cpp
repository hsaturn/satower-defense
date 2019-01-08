#include "ogol_editor.hpp"
#include <Editor.hpp>
#include <Canvas.hpp>

bool OgolEditor::readTheme(string &sItem, CFileParser *pDef)
{
	// TODO editor & canvas are generic widget, not related to ogol_editor
	if (sItem == "editor")
	{
		Editor* editor = new Editor(pDef);
		return true;
	}
	else if (sItem == "canvas")
	{
		Canvas* canvas = new Canvas(pDef);
		return true;
	}
	return false;
}
