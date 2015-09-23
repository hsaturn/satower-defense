#ifndef CSEXCEPTION_HPP_
#define CSEXCEPTION_HPP_
#ifndef CPHANTOM_EXCEPTION_HPP_
#define CPHANTOM_EXCEPTION_HPP_

#include <iostream>
#include <string>

using namespace std;

/**
 * 															Exception Manager 
 **/

class CSException
{
public:
	/**
	 * @param string sClassName Name of the class that throws the exception
	 * @param string sError     Error message
	 **/
	CSException(const string sClassName,const string sError) : msClassName(sClassName),msError(sError){};
		
	friend  ostream& operator<< (ostream& out, const CSException &oException)
	{
		out << oException.getCompleteError() << endl;
		return out;
	}
	
	const string getCompleteError() const { 	return "Error ["+msClassName+"]: "+msError;	 }
	
private:
		string msClassName;
		string msError;
};

#endif /*CPHANTOM_EXCEPTION_HPP_*/

#endif /*CSEXCEPTION_HPP_*/
