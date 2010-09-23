/*
 * xml.hpp
 *
 *  Created on: 7 august 2010
 *      Author: saturn
 */

#ifndef XML_HPP_
#define XML_HPP_

#include <cfileparser.hpp>
#include <string>
#include <limits.h>

using namespace std;

#define NO_STRING "!~&/*$=%"
class SimpleXml
{
public:
	// CFileParser(file,true,true) !!!
	SimpleXml(CFileParser* , const SimpleXml *parent=0);
	~SimpleXml();

	const list<SimpleXml*>* children() const { return &mlstChildren; }
	const SimpleXml* parent() const { return mParent; };

	// Return the first child that has the given name
	const SimpleXml* getNode(const string& sName) const;
	const string value() const { return msValue; }
	const string key() const { return msKey; };
	const map<string,string>*	properties() const { return &mmapProperties; };

	/**
	 * Read a node property, when no default is given the method may throw.
	 * @param string sName name of the property
	 * @param long lDefault value if property does not exists
	 * @return long value (or throw)
	 */
	long   getLongProperty(const string& sName, long lDefault=LONG_MAX) const;
	string getTextProperty(const string& sName, const string& sDefault=NO_STRING) const;

	bool hasProperty(const string &sName) const;

	// Check and throw an exception if property not found.
private:
	list<SimpleXml*>	mlstChildren;
	map<string,string>	mmapProperties;
	string msKey;
	const SimpleXml* mParent;
	string msValue;

};

#endif /* XML_HPP_ */
