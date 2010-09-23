/*
 * SimpleXml.cpp
 *
 *  Created on: 7 ao�t 2010
 *      Author: saturn
 */

#include "SimpleXml.hpp"
#include <stdlib.h>


template <class T>
string to_string(const T t)
{
	stringstream s;
	s << t;
	return s.str();
}

using namespace std;

SimpleXml::SimpleXml(CFileParser *poXml, const SimpleXml* parent)
	: mParent(parent)
{
	while(poXml->good())
	{
		poXml->trim();
		if (poXml->tryString("<?",false))	// Remove ?xml string with no more check
		{
			msKey="root";
			// cout << "root" << endl;
			while(poXml->getNextChar()!='>')
				if (!poXml->good())
					poXml->throw_("Unterminated xml ");

			poXml->trim();
		}
		if (poXml->good())
		{
			if (msKey=="")
			{
				poXml->getExpectedChar("<");
				msKey=poXml->getNextIdentifier("xml node");
				// cout << "key=" << msKey << endl;
				poXml->trim();
				while(poXml->peekChar(false)!='>' && poXml->peekChar(false)!='/')
				{
					string msProperty=poXml->getNextIdentifier();
					if (poXml->getExpectedChar("="))
					{
						mmapProperties[msProperty]=poXml->getNextString();
						// cout << msProperty << '=' << mmapProperties[msProperty] << endl;
					}
					poXml->trim();
				}
				if (poXml->peekChar(false)=='>')
					poXml->getExpectedChar(">");
				else
				{
					poXml->getExpectedChar("/");
					poXml->getExpectedChar(">");
					return;
				}
			}
			else if (poXml->tryString("</"+msKey+'>',false))
			{
				poXml->trim();
				return;
				//poXml->checkExpectedString(msKey+'>',false,true);
			}
			else if (poXml->peekChar()=='<')
			{
				// cout << "new child " << poXml->peekChar() << endl;
				SimpleXml*	poChild=new SimpleXml(poXml,this);
				// cout << "end of new child" << poXml->peekChar() << endl;
				//cout << poChild->key() << ' ' << poChild->children().size() << endl;
				mlstChildren.push_back(poChild);
			}
			else
			{
				msValue+=poXml->getNextChar(false);
			}
		}
		else
		{
			// cout << "??" << endl;
		}
	}
}

SimpleXml::~SimpleXml()
{
	list<SimpleXml*>::iterator oit=mlstChildren.begin();
	while(oit!=mlstChildren.end())
	{
		delete *oit++;
	}
}

long SimpleXml::getLongProperty(const string& sName, long lDefault) const
{
	string sLong=getTextProperty(sName,to_string(lDefault));
	// TODO Check long ?
	long lResult=atol(sLong.c_str());
	return lResult;
}

string SimpleXml::getTextProperty(const string& sName, const string &sDefault) const
{
	map<string,string>::const_iterator oit=mmapProperties.find(sName);
	if (oit!=mmapProperties.end())
	{
		return oit->second;
	}
	else if (sDefault==NO_STRING)
		throw new CSException("SimpleXml","Property ["+sName+"] not found in node ["+msKey+"]");
	else
		return sDefault;
}

const SimpleXml* SimpleXml::getNode(const string& sName) const
{
	SimpleXml* pFound=0;
	list<SimpleXml*>::const_iterator	oit=mlstChildren.begin();
	while(oit!=mlstChildren.end())
	{
		SimpleXml* p=*oit++;
		if (p->key()==sName)
		{
			pFound=p;
			break;
		}
	}
	return pFound;
}

bool SimpleXml::hasProperty(const string &sName) const
{
	map<string,string>::const_iterator oit=mmapProperties.find(sName);
	return oit!=mmapProperties.end();
}
