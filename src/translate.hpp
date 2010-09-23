/*
 * translate.hpp
 *
 *  Created on: 3 août 2010
 *      Author: saturn
 */

#ifndef TRANSLATE_HPP_
#define TRANSLATE_HPP_
#include <map>
#include <string>

using namespace std;

class translate
{
public:
	static void init();
	static map<string, string>	*mapTranslation;

	static const string& _T(const string&);
};

inline const string& _T(const string& s)
 {
	 return translate::_T(s);
 }
#endif /* TRANSLATE_HPP_ */
