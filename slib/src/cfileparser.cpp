#include <cfileparser.hpp>
#include <stdlib.h>


using namespace std;

list<string>	CFileParser::mlstSearchPaths;

CFileParser::CFileParser(const string sFileName, bool bIgnoreEol, bool bAutoTrim)
	:
	mbIncluded			(false),
	mbIgnoreEol			(bIgnoreEol),
	msStringDelimiter	("\"'"),
	mbDefaultComments	(false),
	mbAutoTrim			(bAutoTrim),
	miRow				(0),
	miCol				(0),
	mpIncludedFile		(0),
	msFileName			(sFileName)
{
	mmappCommentsStart			= new map<const string,bool>;
	mmappCommentsMiddleStart	= new map<const string,bool>;
	addCommentLineStart("#");
	addCommentLineStart("//");
	mbDefaultComments=true;
/*
	map<const string,bool>::const_iterator oit=mmappCommentsStart->begin();
	while(oit!=mmappCommentsStart->end())
	{
		cout << "str:" << oit->first << endl;
		oit++;
	}
	*/
	open(sFileName);
}

CFileParser::CFileParser(const string sFileName, const CFileParser* poParent)
{
	mbIncluded					= true;
	mbIgnoreEol					= poParent->mbIgnoreEol;
	msStringDelimiter			= poParent->msStringDelimiter;
	mbDefaultComments			= false;	// Do not erase map (this shouldn't occur anyway)
	mbAutoTrim					= poParent->mbAutoTrim;
	miRow						= 0;
	miCol						= 0;
	mpIncludedFile				= 0;
	msFileName					= sFileName;
	mmappCommentsStart			= poParent->mmappCommentsStart;
	mmappCommentsMiddleStart 	= poParent->mmappCommentsMiddleStart;


	open(sFileName);
}

CFileParser::~CFileParser()
{
	if (!mbIncluded)
	{
		delete mmappCommentsMiddleStart;
		delete mmappCommentsStart;
	}
}

void CFileParser::open(const string &sFileName)
{
	string sErr="";
	string sCompleteFileName=sFileName;

	mpfInput=new ifstream(sCompleteFileName.c_str(),ios::binary);
	if (!mpfInput->is_open())
	{
		list<string>::const_iterator oit=mlstSearchPaths.begin();
		while(oit!=mlstSearchPaths.end())
		{
			// cout << "unable to open '" << sCompleteFileName << "',";
			delete mpfInput;
			sCompleteFileName=(*oit++)+sFileName;
			// cout << "trying '" << sCompleteFileName << "'" << endl;
			mpfInput=new ifstream(sCompleteFileName.c_str(),ios::binary);
			if (mpfInput->is_open())
				break;
		}
	}
	else
	{
		sErr=" (no search path defined)";
	}
	// FIXME This is a copy/paste of the previous constructor
	if (!mpfInput->is_open())
		throw new CSException("CFileParser","Unable to open file ["+sFileName+"]"+sErr);
	else
		msFileName=sCompleteFileName;
}

void CFileParser::addSearchPath(const string& sSearchPath)
{
	string s=sSearchPath;
	// FIXME not windows compatable.
	if (s[s.length()-1]!='/')
	{
		cerr << "Warning: addSearchPath added a / (not windows compatible)" << endl;
		s+='/';
	}
	list<string>::const_iterator oit=mlstSearchPaths.begin();
	while(oit!=mlstSearchPaths.end())
	{
		if ((*oit++)==s)
			return;
	}
	mlstSearchPaths.push_front(s);
}

void CFileParser::addCommentLineStart(const string sCommentStart)
{
	if (mbDefaultComments==true)
	{
		mmappCommentsStart->clear();
		mbDefaultComments=false;
	}

	(*mmappCommentsStart)[sCommentStart]=true;
}

void CFileParser::addMiddleCommentStart(const string sCommentStart)
{
	(*mmappCommentsMiddleStart)[sCommentStart]=true;
}
 
char CFileParser::getNextChar(bool bTrim, bool bConvertTabs)
{
	char c=0;
	bool bSelf=true;
	
	if (mpIncludedFile)
	{
		if (!mpIncludedFile->good())
			deleteIncluded();
		else
		{
			try
			{
				bSelf=false;
				c=mpIncludedFile->getNextChar(bTrim);
				if (c==0)
				{
					deleteIncluded();
					bSelf=true;
				}
			}
			catch(CSException*p)
			{
				throw_(addRowCol(p->getCompleteError()));
			}
		}
	}
	if (bSelf)
	{
		do
		{
			if (eol())
			{
				if (mbIgnoreEol)
				{
					if (mpfInput->good())
					{	readNextRow();
						c=getNextChar(bTrim,bConvertTabs);
					}
					else
						c=0;
					break;
				}
				else
				{
					c=0;
				}
			}
			else
			{
				if (msCurrentRow.length())
				{
					c=msCurrentRow[miCol++];
					if (bConvertTabs && (c=='\t'))
						c=' ';
				}
				else
					c=0;
			}
		} while (bTrim==true and c==' ');
	}

	return c;
}

bool CFileParser::eol()
{
	return (msCurrentRow.length()<=miCol);
}

string CFileParser::getNextWord()
{
	char c;
	string sWord;
	while((c=getNextChar())==' ');
	
	while((c!=0) && (c!=' '))
	{
		sWord+=c;
		c=getNextChar();
	}
	if (mmappCommentsStart->find(sWord)!=mmappCommentsStart->end())
	{
		_setCurrentRow("");
		sWord=getNextWord();
	}

	return sWord;
}

string CFileParser::getCurrentRow()
{
	if (mpIncludedFile)
		return mpIncludedFile->getCurrentRow();
	else
	{
		string s=msCurrentRow;
		readNextRow();
		return s;
	}
}

bool CFileParser::readNextRow()
{
	if (mpIncludedFile)
	{
		mpIncludedFile->readNextRow();
		if (!mpIncludedFile->good())
			deleteIncluded();
	}
	
	if (mpIncludedFile==0)
	{
		miCol=0;
		msCurrentRow="";
		if (!mpfInput->is_open())
			throw new CSException("CFileParser","No file opened");

		while (msCurrentRow.length()==0 && mpfInput->good())
		{
			getline(*mpfInput,msCurrentRow);
	//		mfInput >> msCurrentRow;
			miRow++;

			// Check if the line is a comment
			if (msCurrentRow.length())
			{

				map<const string,bool>::const_iterator oit=mmappCommentsStart->begin();
				while(oit!=mmappCommentsStart->end())
				{
					const string sCommentStart=oit->first;

					if (msCurrentRow.substr(0,sCommentStart.length())==sCommentStart)
					{
						msCurrentRow="";
						break;
					}
					oit++;
				}
			}
		}
		if (msCurrentRow.length())
			msCurrentRow+=' ';

		if (msCurrentRow.substr(0,7)=="include")
		{
			string sFile;
			miCol=8;
			trim();

			if (peekChar()=='"' || peekChar()=='\'')
				sFile=getNextString();
			else
				sFile=getNextWord();
			CFileParser* poInclude=new CFileParser(sFile,this);
			addInclude(poInclude);
			readNextRow();
		}
	}
	return mpfInput->good();
}

void CFileParser::trim()
{
	while((tryChar(' ') || tryChar('\t'))) ;
}

string CFileParser::getNextIdentifier(string sType)
{
	if (mbAutoTrim) trim();

	char c=getNextChar();
	string sId;
		
	if (isalpha(c))
	{
		bool bLoop=false;
		sId+=c;

		do
		{
			c=peekChar(false);
			if (isalnum(c)|(c=='_'))
			{
				bLoop=true;
				getNextChar(false);
				sId+=c;
			}
			else
				bLoop=false;
			
		} while(bLoop);
	}
	if (sId.length()==0)
	{
		string s;
		if (c==0)
			s="Nothing";
		else
			s=(char)c;
		throw_(sType+" expected, ["+s+"] got instead.");
	}

	return sId;
}

void CFileParser::throw_(const string sClassName,const string sError)
{
	
	//if (mpIncludedFile)
	
	throw new CSException(
		sClassName,
		addRowCol(sError));
}

const string CFileParser::addRowCol(const string sError) const
{
	string sComplete;
	string sSep="\n     ";
	string sCurFilePos="in ("+msFileName+") at ";
	if (miCol==0)
		sCurFilePos+="end of line "+_to_string(miRow-1);
	else
		sCurFilePos+="line "+_to_string(miRow)+", col "+_to_string(miCol);
	//string sFile="In file "
	if (mpIncludedFile)
		sComplete=mpIncludedFile->addRowCol(sError)+","+sSep+"included "+sCurFilePos;
	else
		sComplete=sError+sSep+sCurFilePos;
	
	sComplete+=". Current row was ("+msCurrentRow+")";
	return sComplete;
}
	

void CFileParser::throw_(const string sError)
{
	throw_("CFileParser",sError);
}

string CFileParser::getNextString(const string sType)
{
	if (mbAutoTrim) trim();

	char cQuote=getExpectedChar(msStringDelimiter);
	string s;

	if (cQuote!=0)
	{
		char c;
		do
		{
			c=getNextChar(false,false);
			if ((c!=cQuote)&&(c!=0))
				s+=c;
		}
		while((c!=cQuote) && (c!=0));
	}
	else
		throw_(sType+" not found.");
	return s;
}

char CFileParser::getExpectedChar(string sCharList)
{
	char c=getNextChar(true);
		
	unsigned int i(0);

	while(i<sCharList.length())
		if (sCharList[i]==c)
			break;
		else
			i++;

	if (sCharList[i]==0)
	{
		throw_(	"(132) Expected character '"+sCharList+"'");
	}
			
	return c;
}

char CFileParser::tryChars(string sCharList, bool bTrim)
{
	char c=peekChar(bTrim);
	unsigned int i(0);
	
	while(i<sCharList.length())
		if (sCharList[i]==c)
			break;
		else
			i++;
	if (sCharList[i]==0)
		c=0;
	else
		getNextChar();
			
	return c;
}
char CFileParser::peekChar(bool bTrim)
{
	char c=getNextChar(bTrim);
	prevCol();
	return c;
}

bool CFileParser::tryString(string sTry, bool bSpaceAfter)
{
	bool bRet=true;			// Assume result is true by default
	if (mpIncludedFile)
	{
		bRet=mpIncludedFile->tryString(sTry,bSpaceAfter);
	}
	else
	{
		peekChar(mbAutoTrim);	// Assure l'autoTrim et la lecture �ventuelle du stream

		unsigned int j=0;
		if (bSpaceAfter)
			sTry+=' ';

		while(j<sTry.length())
		{
			if (sTry[j]==msCurrentRow[miCol+j])
				j++;
			else
			{
				bRet=false;
				break;
			}
		}
		if (bRet)
		{
			for(j=0;j<sTry.length();j++)
			{
				getNextChar();
			}
			if (bSpaceAfter)
				getNextChar();	// FIXME, this should test either space or CR
		}
	}
	return bRet;
}

bool CFileParser::tryChar(char cTry)
{
	bool bRet=true;
	char c=getNextChar(true);
	if (c!=cTry)
	{
		bRet=false;
		prevCol();
	}
	return bRet;
}

long CFileParser::getNextLong(const string sInfo)
{
	if (mbAutoTrim) trim();

	bool bOk=false;
	long lResult=0;
	string sWord;

	if (peekChar()=='-')
		sWord+=getNextChar(false);

	while(isdigit(peekChar(false)))
	{
		bOk=true;
		sWord+=getNextChar(false);
	}
		
	if (bOk)
	{
		lResult=atol(sWord.c_str());
		while(tryChar(' '));
		bOk=true;
	}
		
	if (!bOk)
	{
		if (sInfo.length())
			throw_("integer expected ("+sInfo+").");
		else
			throw_("integer expected.");
	}
	
	return lResult;
}

bool CFileParser::good()
{
	if (mpIncludedFile)
	{
		if (mpIncludedFile->good())
			return true;
		else
			deleteIncluded();
	}
	if (msCurrentRow.length()>miCol)
		return true;
	return mpfInput->good();
}

const string CFileParser::_getCurrentRow() const
{
	if (mpIncludedFile)
	{
		return mpIncludedFile->_getCurrentRow();
	}
	else
		return msCurrentRow;
}

void CFileParser::_setCurrentRow(const string sRow)
{
	if (mpIncludedFile)
		mpIncludedFile->_setCurrentRow(sRow);
	else
		msCurrentRow=sRow;
}

void CFileParser::prevCol()
{
	if (mpIncludedFile)
		mpIncludedFile->prevCol();
	else if (miCol)
		miCol--;
}

void CFileParser::nextCol()
{
	if (mpIncludedFile)
		mpIncludedFile->nextCol();
	else if (miCol)
		miCol++;
}

void CFileParser::addInclude(CFileParser* poInclude)
{
	if (mpIncludedFile)
		mpIncludedFile->addInclude(poInclude);
	else
		mpIncludedFile=poInclude;
}

void CFileParser::deleteIncluded()
{
	if (mpIncludedFile)
	{
		delete mpIncludedFile;
		mpIncludedFile=0;
	}
}

const string CFileParser::getParsedFile () const
{
	if (mpIncludedFile)
		return mpIncludedFile->getParsedFile();
	else
		return getFileName();
}

unsigned int    CFileParser::getParsedCol  () const
{
	if (mpIncludedFile)
		return mpIncludedFile->getParsedCol();
	else
		return getCurrentCol();
}

unsigned int    CFileParser::getParsedLine () const
{
	if (mpIncludedFile)
		return mpIncludedFile->getParsedLine();
	else
		return getCurrentLine();
}

void CFileParser::zapComments()
{
	// FIXME
}


bool CFileParser::checkExpectedString(const string &sString, bool bSpaceAfter, bool bThrow)
{
	bool   bRet=false;
	string sGot;

	// FIXME if 1st char matches, it is removed
	// all 1st matched chars are removed, even if the string is not found
	if (peekChar(true)==sString[0])
	{
		sGot=getNextChar(true);
		while ((sGot.length()<sString.length()) and !eol())
		{
			sGot+=getNextChar(false,false);
		}
		if (sGot==sString)
		{
			if (bSpaceAfter)
			{
				char c=getNextChar(false,true);
				if (c==' ')
					bRet=true;
			}
			else
				bRet=true;
		}
	}
	if ((bRet==false) && (bThrow==true))
	{
		throw_('['+sString+"] expected.");
	}
	return bRet;
}

void CFileParser::setParsedCol(unsigned int iCol)
{
	miCol=iCol;
}
