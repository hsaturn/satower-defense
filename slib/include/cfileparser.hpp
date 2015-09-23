#ifndef CFILEPARSER_HPP_
#define CFILEPARSER_HPP_

#include <csexception.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <list>

template <class T>
string _to_string(const T t)
{
	stringstream s;
	s << t;
	return s.str();
}

using namespace std;

/**
 * Generic file parser class that allows included files to be transparently
 * managed by the class.
 *
 * Commented line starts by # (default), this can be overriden with addCommentedLineStart
 * Commented end of lines (none by default) can be set with addMiddleCommendStart
 *
 * End of lines
 *
 * By default, the file is parsed with no end of line consideration. Thus inserting carriage return
 * in the parsed file does not affect the way the 'client' see the file.
 * This can be overriden in the constructor. If eol are not ignored, then one must call
 * readNextRow() once eol() is true in order to go to the next line,
 * forgetting that point may enter in a no ending loop with the parser returning empty strings forever.
 *
 * /!\ one *must* call readNextRow before using other methods when eol are not ignored.
 * This operation cannot be done (yet?) automatically due to the fact that at the time the class is
 * instanciated, the comment line starts are not known.
 *
 * Yet, commenting blocks is not allowed
 *
 */
class CFileParser
{
	public:
		/**
		 * @param string sFileName Name of the file to parse
		 * @param bool   bIgnoreEol    false if the parser halts on eol (see class documentation /!\)
		 * @param bool   bAutoTrim     each getXXX method will call trim() prior to read content, except getNextChar[s]
		 *
		 */ 
		CFileParser(const string sFileName, bool bIgnoreEol=true, bool bAutoTrim=false);
		
		virtual ~CFileParser();

		const string getFileName   () const { return msFileName; };
		unsigned int getCurrentCol () const { return miCol; };
		unsigned int getCurrentLine() const { return miRow; };
		
		// These method returns the real parsed line, taking in account the
		// included files if any.
		const string getParsedFile () const;

		/**
		 * Returns the current col beeing parsed.
		 * @return unsigned int
		 */
		unsigned int    getParsedCol  () const;

		/**
		 * Returns the current line number beeing parsed
		 * @return unsigned int
		 */
		unsigned int    getParsedLine () const;
		
		/**
		 * Change the current column to parse.
		 * This method may not be used when bIgnoreEol=true.
		 * Note : this method may throw sooner or later if bIgnoreEol=true
		 */
		void setParsedCol(unsigned int iCol);

		/**
		 * remove first spaces of the stream
		 */
		void trim();

		/**
		 * In fact, this method returns the next lexem, not the next word.
		 * So any printable character is consired as part of string.
		 *
		 * @return string
		 **/
		string getNextWord();
		
		/**
		 * Get the next identifier.
		 *
		 * An identifier is a string that starts
		 * with a letter, the have any letter,number or _
		 *
		 * If the current input is not an identifier, an exception is thrown with
		 * the message 'sType expected', sType is Identifier by default.
		 *
		 * @input string sType Exception message modification
		 * @output string identifier.
		 **/
		string getNextIdentifier(string sType="Identifier"); // throw(CSException)
		
		/**
		 * Gets the next string. A string starts by ' or " and ends by ' or ".
		 * @return string.
		 **/
		string getNextString(const string sType="string");		// throw(CSException)
		
		/**
		 * read the next char that must be in sCharList
		 * If not, a CSException is thrown
		 *
		 * @param string sCharList a list of expected char
		 * @return char the found char */
		char getExpectedChar(string sCharList); // throw(CSException)
		
		/**
		 * Check if the next string belongs to lstStringList
		 * and return it if it has been found.
		 * Note that lstString is ordered, and the first found string
		 * is returned. Other won't then be checked.
		 *
		 * @param list<const string> lstStringList
		 * @return string      a found string or an empty string.
		 */
		string getExpectedString(const list<const string> &lstStringList);

		/**
		 * Check if the next string is sString and throw (if asked)
		 * first spaces are ignored.
		 *
		 * @param string sString the string to be checked
		 * @param bool   bSpaceAfter the string must be followed by a space
		 * @param bool   bThrow      true if throw in case of failure.
		 * @return bool true if success
		 *
		 */
		bool checkExpectedString(const string &sString, bool bSpaceAfter=true, bool bThrow=true);

		/**
		 * Get the current row. regardless of what it contains.
		 * Note : the whole row is returned, even what has already
		 * been parsed.
		 * @return string
		 */
		string getCurrentRow();

		/**
		 * /!\ BUGGY, all first chars equal to the string are removed from stream.
		 * Verify if the next chars in the stream are exactly sTry
		 * @param bSpaceAfter a space must follow the string
		 * @return true if the string matches (removed from stream), else false (nothing removed)
		 */
		bool tryString(string sTry, bool bSpaceAfter=true);

		/** Same as getExpectedChar except that if the
		 * char is not found in sCharList, 0 is returned instead
		 * of throwing a CSException
		 *
		 * @param string sCharList list of expected char
		 * @param bool bTrim trim spaces
		 * @return char the found char or 0 if not found */
		char tryChars(string sCharList,bool bTrim=true);
		
		bool good();
		
		void throw_(const string sError);
		void throw_(const string sClass,const string sError);
		
		/** get the next char
		 * spaces and tabs are replaced by ' '
		 *
		 * @param bool bTrim spaces are ignored
		 * @param bool bConvertTabs tabs are converted to space (and thus can be ignored if bTrim)
		 * @return char
		 **/
		char   getNextChar(bool bTrim=false, bool bConvertTabs=true);
		
		/**
		 * Add a string that will disable a whole line.
		 * The line must start by sCommentStart, precedeed by one or more spaces.
		 * When used, the default comment (#) will not further be checked
		 *
		 * @param string sCommentStart
		 * @return void
		 */
		void addCommentLineStart(const string sCommentStart);

		/**
		 * Add a string that will comment the end of a line
		 * This may be overriden by the parsing itself. For
		 * example, getNextString will not be halted by occurence of sCommentStart
		 *
		 * @param string sCommentStart
		 * @return void
		 * @see zapComments
		 */
		void addMiddleCommentStart(const string sCommentStart);

		/**
		 * Check if current string is a middle line comment
		 * If true, then the rest of the row is ignored.
		 * The parser usually does this job, but it may be usefull to
		 * do that in certain circumstances.
		 *
		 * @return void
		 * @see addMiddleCommentStart
		 */
		void zapComments();

		/**
		 * read a long number, or throw a CSException.
		 *
		 * @param string sInfo more info when an exception is thrown
		 * @return long the value */
		long getNextLong(string sInfo=""); // throw(CSException)
		
		/** read the next char, without removing it from input
		 * the input may be trimed before peeking the char.
		 *
		 * @param bool bTrim spaces are ignored
		 * @return char */
		char  peekChar(bool bTrim=true);
		
		/** If the next char to read is equal to c, the parser goes to the
		 * next one, and returns true. Else, the next char won't be skipped and
		 * false is returned.
		 *
		 * @param char c  expected char
		 * @return bool true if the next char is c (skipped then) else, false. */
		bool tryChar(char c);
		
		/** Ignore the current row and read the next one.
		 * @return bool false when no more row is available.
		 */
		bool readNextRow();

		/**
		 * ROW MODE ONLY
		 *
		 * Returns true when no more char is available in the current row.
		 * @return bool
		 */
		bool eol();

		/**
		 * Override the default string delimiters.
		 * Default is single quote and double quote ("\"'")
		 *
		 * @param string sStringDelimiter new string delimiter
		 * @return void
		 */
		void setStringDelimiter(const string sStringDelimiter);
		
		/**
		 * Add a search path for include statements
		 * @param sSearchPath a path (/ terminated)
		 * @return void
		 */
		static void addSearchPath(const string& sSearchPath);

	private:
		// This constructor allows to handler include files, keeping the parser
		// parameters
		CFileParser(const string sFileName, const CFileParser *parent);
		
		void open(const string &sFileName);
		
		const string addRowCol(const string sError) const;
		const string _getCurrentRow() const;
		void _setCurrentRow(const string sRow);
		void prevCol();
		void nextCol();
		void addInclude(CFileParser*);
		void deleteIncluded();
		map<const string,bool>*	mmappCommentsStart;			// List of commented line starts
		map<const string,bool>*	mmappCommentsMiddleStart;	// List of middle comment starts
		
	private:
		bool			mbIncluded;						// true if the private ctor has been used (also: do not delete maps contents)
		bool			mbIgnoreEol;					// True if end of line are not considered while parsing the file
		string			msStringDelimiter;
		bool			mbDefaultComments;				// True when mmapCommentsStart contains the default comment strings
		bool			mbAutoTrim;
		unsigned int	miRow;
		unsigned int	miCol;
		ifstream*		mpfInput;
		CFileParser*	mpIncludedFile;
		string			msCurrentRow;
		ifstream		mfStream;
		string			msFileName;
		
		static list<string>	mlstSearchPaths;					// List of search path for include

};

#endif /*CFILEPARSER_HPP_*/
