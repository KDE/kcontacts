/* the Configuration Database library, Version II

   the KDE addressbook

   $ Author: Mirko Boehm $
   $ Copyright: (C) 1996-2001, Mirko Boehm $
   $ Contact: mirko@kde.org
         http://www.kde.org $
   $ License: GPL with the following explicit clarification:
         This code may be linked against any version of the Qt toolkit
         from Troll Tech, Norway. $
*/

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
}

// #include <qstring.h>
#include <QTextStream>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <QFileInfo>

#include <kdebug.h>

#include "qconfigDB.h"
#include "qconfigDB.moc"

#ifdef KAB_KDEBUG_AREA
#undef KAB_KDEBUG_AREA
#endif

#define KAB_KDEBUG_AREA 800

static bool isComment(const QByteArray &line)
{
  // ############################################################################
  QByteArray tline = line.trimmed();
  if (tline.isEmpty()) {
    return false; // line is empty but not a comment
  } else  {
    return(tline[0]=='#');
  }
  // ############################################################################
}

static void tokenize(list<QByteArray>& res, const QByteArray& text, char tr, bool strict=false)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "tokenize: called." << endl;
  int eins=0, zwei=0;
  QByteArray teil;
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "tokenize: partening -->%" << text.data() << "<--." << endl;
  res.erase(res.begin(), res.end());
  // -----
  if (text.isEmpty())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "tokenize: text is an empty string, done." << endl;
      return;
    }
  while (zwei!=-1)
    {
      teil="";
      zwei=text.indexOf(tr, eins);
      if (zwei!=-1)
	{
	  teil=text.mid(eins, zwei-eins);
	  res.push_back(teil);
	} else { // last element
	  if (!strict) // nur wenn dazwischen Zeichen sind
	    {
	      teil=text.mid(eins, text.length()-eins);
	      res.push_back(teil);
	    }
	}
      eins=zwei+1;
      // if ((unsigned)eins>=text.length()) break;
    }
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "tokenize: partened in "
				  << res.size() << " parts.\n";
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "tokenize: done." << endl;
  // ############################################################################
}

// QCString AuthorEmailAddress; // assign your email address to this string

static QByteArray ReadLineFromStream(QTextStream& stream)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "ReadLineFromStream:: reading line." << endl;
  QByteArray line;
  // -----
  while (!stream.atEnd())
    {
      line=stream.readLine().toAscii();
      if (!line.isEmpty())
	{
	  if (isComment(line))
	    {
	      line="";
	      continue;
	    }
	}
      break;
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "ReadLineFromStream:: line \"" << line.data() << "\" read.\n";
  return line;
  // ############################################################################
}

// class implementations:

list<QString> QConfigDB::LockFiles; // the lockfiles created by this session

KeyValueMap::KeyValueMap()
  : data(new StringStringMap)
{
  // ###########################################################################
  // ###########################################################################
}

KeyValueMap::KeyValueMap(const KeyValueMap& orig)
  : data(new StringStringMap(*orig.data))
{
  // ###########################################################################
  // ###########################################################################
}

KeyValueMap::~KeyValueMap()
{
  // ###########################################################################
  delete data;
  // ###########################################################################
}

bool KeyValueMap::invariant()
{
  return true;
}

StringStringMap::iterator KeyValueMap::begin()
{
  return data->begin();
}

StringStringMap::iterator KeyValueMap::end()
{
  return data->end();
}

unsigned int
KeyValueMap::size() const
{
  // ###########################################################################
  return data->size();
  // ###########################################################################
}

void
KeyValueMap::clear()
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
	     "KeyValueMap::clear: erasing map contents ... " << endl;
  // -----
  if (!data->empty()) // erase fails on empty containers!
    {
      data->erase(data->begin(), data->end());
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "done." << endl;
  // ###########################################################################
}

bool
KeyValueMap::fill(const QString& filename, bool force, bool relax)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  QFile file(filename);
  QByteArray line;
  // -----
  if (file.open(QIODevice::ReadOnly))
    {
      QTextStream stream(&file);
      // We read/write utf8 strings, so we don't want that QTextStream uses local8bit
      // Latin1 means : no conversion, when giving char*s to a QTextStream. (DF)
      stream.setCodec( "ISO 8859-1" );
      // -----
      while (!stream.atEnd())
	{
	  line=stream.readLine().toAscii();
	  if (!line.isEmpty() /* && !stream.eof() */ && !isComment(line))
	    {
	      if (!insertLine(line, force, relax, false))
		{
		    kDebug(GUARD, KAB_KDEBUG_AREA) <<
			"KeyValueMap::fill: could not insert line "
						    << line << ".\n"; // ignore this case further
		}
	    }
	}
      file.close();
      // -----
      return true;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
	  "KeyValueMap::fill: cannot open file " <<
	  filename << endl;
      return false;
    }
  // ###########################################################################
}

bool
KeyValueMap::save(const QString& filename, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "KeyValueMap::save: saving data to -->" <<
      filename << "<--.\n";
  StringStringMap::iterator pos;
  QFile file(filename);
  // ----- open file, regarding existence:
  if (!force)
    {
      if (file.exists())
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "KeyValueMap::save: file exists but may not." << endl;
	  return false;
	}
    }
  if (file.open(QIODevice::WriteOnly))
    {
      QTextStream stream(&file);
      stream.setCodec( "ISO 8859-1" ); // no conversion
      stream << "# saved by KeyValueMap object ($Revision$)" << endl;
      for (pos=data->begin(); pos!=data->end(); ++pos)
	{ // values do not get coded here
	  stream << (*pos).first << '=' << (*pos).second << endl;
	}
      file.close();
    } else {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<
	    "KeyValueMap::save: could not open file -->%s<-- for saving." <<
	    filename.toUtf8() << endl;
      return false;
    }
  // ###########################################################################
  return true;
}

bool
KeyValueMap::save(QTextStream& file, int count)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
	     "KeyValueMap::save: saving data to given output stream." << endl;
  StringStringMap::iterator pos;
  bool ret=true;
  char* prefix=new char[count+1];
  memset(prefix, ' ', count);
  prefix[count]=0;
  // -----
  for (pos=data->begin(); pos!=data->end(); ++pos)
    {
      file << prefix << (*pos).first << '=' << (*pos).second << endl;
    }
  delete [] prefix;
  // -----
  return ret;
  // ###########################################################################
}


bool
KeyValueMap::erase(const QByteArray& key)
{
  // ###########################################################################
  bool rc=(data->erase(key)>0);
  return rc;
  // ###########################################################################
}


bool
KeyValueMap::empty()
{
  // ###########################################################################
  return data->empty();
  // ###########################################################################
}

bool
KeyValueMap::parseComplexString
(const QByteArray& orig,
 int index, // first char to parse
 QByteArray& result, // string without leading and trailing ".."
 int& noOfChars) // no of chars that represented the
 const           // complex string in the original
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  int first;
  QByteArray temp;
  temp.reserve(2*orig.length());
  QByteArray mod;
  int count=1;
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
    "KeyValueMap::parseComplexString: parsing the string -->"
				  << orig << "<--.\n";
  // -----
  if (orig.isEmpty())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::parseComplexString: string is empty.\n"
		 "                                 "
		 "This is no valid complex string." << endl;
      return false;
    }
  // ----- prepare the string:
  temp=orig.mid(index, orig.length()-index); // remove everything before index
  mod=temp.trimmed(); // remove leading and trailing white spaces
  // ----- test some conditions:
  if (mod.length()<2)
    {
      kDebug() << "KeyValueMap::parseComplexString: no pair of brackets " << endl;
      return false;
    }
  if (mod[0]!='"')
    {
      kDebug() << "KeyValueMap::parseComplexString: no opening bracket." << endl;
      return false;
    }
  // ----- now parse it:
  first=1; // first character after opening bracket
  temp="";
  for (;;)
    {
      if (mod[first]=='\\')
	{ // handle special characters
	  ++first;
	  kDebug(GUARD, KAB_KDEBUG_AREA).form("KeyValueMap::parseComplexString: found "
					       "a special character \"%c\".", mod.data()[first]) << endl;
	  if (first==mod.length())
	    {
		kDebug(GUARD, KAB_KDEBUG_AREA) <<
		    "KeyValueMap::parseComplexString: "
		    "string lacks the closing \".\n          "
		    "                       This is no valid "
		    "complex string." << endl;
	      return false;
	    }
	  switch(mod[first])
	    {
	    case 't': temp+='\t'; break;
	    case 'n': temp+='\n'; break;
	    case '"': temp+='"'; break;
	    case 'e': temp+="\\e"; break;
	    case '\\': temp+='\\'; break;
	    default:
	      // WORK_TO_DO: implement octal coding here!
		kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::parseComplexString: "
		    "invalid control character.\n            "
		    "                     This is no valid complex string." << endl;
	      return false;
	    }
	  count+=2; // this took 2 characters
	  ++first;
	} else { // it is a character
	  ++count;
	  if (mod[first]=='"') // end of coded string?
	    {
	      break;
	    }
	  temp+=mod[first];
	  ++first;
	}
      if (first>=mod.length())
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::parseComplexString: "
	    "string lacks the closing \".\n              "
	    "                   This is no valid complex string.\n";
	  return false;
	}
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA).form(
	     "KeyValueMap::parseComplexString: finished parsing, no errors, "
	     "%i characters, %i in string.", count, temp.length()) << endl;
  noOfChars=count;
  result=temp;
  // ###########################################################################
  return true;
}

QByteArray
KeyValueMap::makeComplexString(const QByteArray& orig)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "KeyValueMap::makeComplexString: coding the string\n           -->"
				  << orig <<
      "<--\n                                into a complex string.\n";
  QByteArray temp;
  temp.reserve(2*orig.length());
  int count;
  // -----
  temp+='"'; // opening bracket
  for (count=0; count<orig.length(); count++)
    {
      switch(orig[count])
	{ // catch all special characters:
	case '"':
	kDebug(GUARD, KAB_KDEBUG_AREA).form("KeyValueMap::makeComplexString: "
	"found the special char \"%c\".", orig[count]) << endl;
	  temp+='\\';
	  temp+='"';
	  break;
	case '\n':
	kDebug(GUARD, KAB_KDEBUG_AREA).form("KeyValueMap::makeComplexString: "
	"found the special char \"%c\".", orig[count]) << endl;
	  temp+='\\';
	  temp+='n';
	  break;
	case '\t':
	  kDebug(GUARD, KAB_KDEBUG_AREA).form("KeyValueMap::makeComplexString: "
	  "found the special char \"%c\".", orig[count]) << endl;
	  temp+='\\';
	  temp+='t';
	  break;
	case '\\':
	kDebug(GUARD, KAB_KDEBUG_AREA).form("KeyValueMap::makeComplexString: "
	"found the special char \"%c\".", orig[count]) << endl;
	  temp+='\\';
	  temp+='\\';
	  break;
	default: temp+=orig[count];
	}
    }
  temp+='"'; // closing bracket
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "KeyValueMap::makeComplexString: result is\n           -->"
				  <<temp<<"<--.\n";
  return temp;
  // ###########################################################################
}

bool
KeyValueMap::getRaw(const QByteArray& key, QByteArray& value) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "KeyValueMap::getRaw: trying to get raw value for key \"" << key << "\" ...\n";
  StringStringMap::iterator pos=data->find(key);
  // -----
  if (pos==data->end())
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "not in KeyValueMap." << endl;
      return false;
    } else {
      value=(*pos).second;
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "in KeyValueMap, value is "
				      << value << endl;
      return true;
    }
  // ###########################################################################
}

bool
KeyValueMap::insertRaw(const QByteArray& key, const QByteArray& value, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "KeyValueMap::insertRaw: inserting uncoded value "
				  << value <<
      " for key " << key << endl;
  int n=0;
  // -----
  if (key.isEmpty()) // empty KEYS are errors:
    {
      kDebug() << "KeyValueMap::insertRaw: tried to insert empty key." << endl;
      return false;
    }
  if (force) // entry will be replaced
    {
      n=data->erase(key);
    }
  if (data->insert(StringStringMap::value_type(key, value)).second)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insertRaw: success"
				      << (n==0 ? "" : " (forced)") << endl;
      return true;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insertRaw: failed, "
	  "key already in KeyValueMap." << endl;
      return false;
    }
  // ###########################################################################
}


// -----------------------------------------------------------------------------
// HUGE SEPARATOR BETWEEN INTERNAL LOGIC AND EXTENDABLE PAIRS OF GET- AND INSERT
// -METHODS.
// EXTENDABLE MEANS: OTHER DATATYPES CAN BE ADDED HERE.
// -----------------------------------------------------------------------------

/* The following functions are the pairs of insert-get-methods for different
 * data types. See keyvaluemap.h for the declarations.  */

// ascii strings:

bool
KeyValueMap::insert(const QByteArray& key, const QByteArray& value, bool force)
{
    register bool GUARD; GUARD=false;
    // ###########################################################################
    kDebug(GUARD, KAB_KDEBUG_AREA) <<
	"KeyValueMap::insert: inserting value\n           -->"
				    << value <<
	"<-- \""
	"                     for key\n           -->"
				    << key <<
	"<--.\n";
	return insertRaw(key, makeComplexString(value), force);
    // ###########################################################################
}

/* Attention:
 * This is another insert function that partens lines like "key=value"!
 * It is used for reading files and command line parameters easily.
 */

bool
KeyValueMap::insertLine(QByteArray line, bool force, bool relax, bool encode)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "KeyValueMap::insertLine: inserting line -->"<<line<<"<--.\n";
  int index;
  QByteArray key;
  QByteArray value;
  // ----- is the line empty or does it contain only whitespaces?
  uint len = line.length();
  for (index=0; isspace(line[index]) && (unsigned)index<len; ++index);
  if (line.isEmpty() || (unsigned)index==len)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
	  "KeyValueMap::insertLine: line is empty." << endl;
      return false;
    }
  // -----
  index=line.indexOf('=');
  if (index==-1)  // not found
      {
	  kDebug() << "KeyValueMap::insertLine: no \"=\" found in \""<<line<<"\".\n";
	  return false;
    }
  // -----
  key=line.mid(0, index); // copy from start to '='
  value=line.mid(index+1, line.length()-1-index); // copy the rest
  // ----- only alphanumerical characters are allowed in the keys:
  for (index=key.length()-1; index>-1; /* nothing */)
    {
      if (!(isalnum(key[index]) || ispunct(key[index])))
	 {
	   key=key.remove(index, 1); // WORK_TO_DO: optimize this (very slow)!
	 }
      --index;
    }
  // ----- now insert it if key is still valid:
  if (!key.isEmpty() && (relax==true ? 1 : !value.isEmpty() ) )
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insertLine: done." << endl;
      if (encode)
	{ // the usual way:
	  return insert(key, value, force);
	} else { // while loading from a already coded file:
	  return insertRaw(key, value, force);
	}
    } else {
      kDebug() << "KeyValueMap::insertLine: key " << (relax ? "" : "or value ") << " is empty." << endl;
      return false;
    }
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, QByteArray& value) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[string]: "
      "trying to get value for key \"" << key << "\" ... " << endl;
  QByteArray raw;
  QByteArray temp;
  // -----
  if (!getRaw(key, raw))
    {
      return false; // key does not exist
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[string]: checking "
		 "wether this is a complex string." << endl;
      {
	int count;
	if (parseComplexString(raw, 0, temp, count))
	  {
	    kDebug(GUARD, KAB_KDEBUG_AREA) <<
		       "KeyValueMap::get[string]: complex string found." << endl;
	    value=temp;
	  } else {
	    kDebug(GUARD, KAB_KDEBUG_AREA) <<
		       "KeyValueMap::get[string]: this is no complex string." << endl;
	    // disabled this strong check:
	    // CHECK(false); // kill debug version
	    return false;
	  }
      }
      // ^^^^^^
      return true;
    }
  // ###########################################################################
}

// (^^^ ascii strings)
// UNICODE strings:

bool
KeyValueMap::insert(const QByteArray& key, const QString& value, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  QByteArray v;
  // -----
  v=value.toUtf8();
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[QString]: trying to "
      "insert \"" << (!value.isNull() ? "true" : "false")
				  << "\" for key\n           -->"
				  << v
				  << "<--.\n";
  return insert(key, v, force);
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, QString& value) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QString]: trying to get "
      "a QString value for key " << key << endl;
  QByteArray v;
  // ----- get string representation:
  if (!get(key, v))
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QString]: key "
					<< key << " not in KeyValueMap.\n";
	return false;
    }
  // ----- find its state:
  value=QString::fromUtf8(v); // is there a better way?
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QString]: success, value"
	     " (in UTF8) is " << v << endl;
  return true;
  // ###########################################################################
}

// (^^^ UNICODE strings)
// bool:

bool
KeyValueMap::insert(const QByteArray& key, const bool& value, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[bool]: trying to "
      "insert \""
				  << (value==true ? "true" : "false")
				  <<"\" for key\n           -->"
				  << key << "<--.\n";
  return insert(key, value==true ? "true" : "false", force);
  // ###########################################################################
}


bool
KeyValueMap::get(const QByteArray& key, bool& value) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[bool]: trying to get "
      "BOOL value for key " << key << endl;
  QByteArray v;
  // ----- get string representation:
  if (!get(key, v))
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[bool]: key "
					<< key << " not in KeyValueMap.";
      return false;
    }
  // ----- find its state:
  v=v.trimmed();
  if (v=="true")
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[bool]: success, "
	    "value is true." << endl;
      value=true;
      return true;
    }
  if (v=="false")
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[bool]: success, "
      "value is false." << endl;
      value=false;
      return true;
    }
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
	     "KeyValueMap::get[bool]: failure, unknown value." << endl;
  // -----
  return false;
  // ###########################################################################
}

// (^^^ bool)
// long:

bool
KeyValueMap::insert(const QByteArray& key, const long& value, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[int]: trying to "
      "insert value \""<<value << "\" for key\n           -->"<<key<<"<--.\n";
  QByteArray temp;
  // -----
  temp.setNum((qlonglong)value);
  return insert(key, temp, force);
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, long& value) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[int]: trying to get "
      "INTEGER value for key " << key << endl;
  QByteArray v;
  bool ok;
  long temp;
  // -----
  if (!get(key, v))
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[int]: key "
					<< key <<" not in KeyValueMap.\n";
      return false;
    }
  // -----
  temp=v.toLong(&ok);
  if (ok)
    {
      value=temp;
      return true;
    } else {
      return false;
    }
  // ###########################################################################
}

// (^^^ long)
// long int lists:

bool
KeyValueMap::insert(const QByteArray& key, const list<long>& values, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[long int list]: "
	     "trying to insert long int list into map." << endl;
  QByteArray temp;
  QByteArray value;
  list<long>::const_iterator pos;
  // -----
  for (pos=values.begin(); pos!=values.end(); ++pos)
    {
      temp.setNum((qlonglong)*pos);
      value=value+temp+", ";
    }
  if (!value.isEmpty())
    { // remove last comma and space:
      value.remove(value.length()-2, 2);
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[long int list]: "
  "constructed string value is " << value << endl;
  return insert(key, value, force);
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, list<long>& values) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[long int list]: trying "
      "to decode int list for key " << key << endl;
  kDebug(!values.empty(), KAB_KDEBUG_AREA) << "KeyValueMap::get[long int list]"
      ": attention - list should be empty but is not.\n";
  QByteArray value;
  list<QByteArray> tokens;
  list<QByteArray>::iterator pos;
  long temp;
  bool ok;
  // -----
  if (!get(key, value))
  {
    kDebug(GUARD, KAB_KDEBUG_AREA) <<
	       "KeyValueMap::get[long int list]: no such key." << endl;
    return false;
  }
  tokenize(tokens, value, ',');
  if (tokens.empty())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::get[long int list]: no tokens." << endl;
      return false;
    }
  // -----
  for (pos=tokens.begin(); pos!=tokens.end(); ++pos)
    {
      temp=(*pos).toLong(&ok);
      if (ok)
	{
	  values.push_back(temp);
	} else {
	  kDebug() << "KeyValueMap::get[long int list]: conversion error for " << endl;
	}
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[long int list]: done." << endl;
  // ###########################################################################
  return true;
}

// (^^^ long int lists)
// int lists:

bool
KeyValueMap::insert(const QByteArray& key, const list<int>& values, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[int list]: trying to "
	     "insert int list into map." << endl;
  QByteArray temp;
  QByteArray value;
  list<int>::const_iterator pos;
  // -----
  for (pos=values.begin(); pos!=values.end(); ++pos)
    {
      temp.setNum(*pos);
      value=value+temp+", ";
    }
  if (!value.isEmpty())
    { // remove last comma and space:
      value.remove(value.length()-2, 2);
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[int list]: "
      "constructed string value is " << value << endl;
  return insert(key, value, force);
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, list<int>& values) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[int list]: trying to "
      "decode int list for key " << key << endl;
  kDebug(!values.empty(), KAB_KDEBUG_AREA) << "KeyValueMap::get[int list]: "
	     "attention - list should be empty but is not.\n";
  QByteArray value;
  list<QByteArray> tokens;
  list<QByteArray>::iterator pos;
  int temp;
  bool ok;
  // -----
  if (!get(key, value))
  {
    kDebug(GUARD, KAB_KDEBUG_AREA) <<
	       "KeyValueMap::get[int list]: no such key." << endl;
    return false;
  }
  tokenize(tokens, value, ',');
  if (tokens.empty())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::get[int list]: no tokens." << endl;
      return false;
    }
  // -----
  for (pos=tokens.begin(); pos!=tokens.end(); ++pos)
    {
      temp=(*pos).toInt(&ok);
      if (ok)
	{
	  values.push_back(temp);
	} else {
	    kDebug() << "KeyValueMap::get[int list]: conversion error for " << *pos << endl;
	}
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[long int list]: done." << endl;
  // ###########################################################################
  return true;
}

// (^^^ int lists)
// doubles:

bool
KeyValueMap::insert(const QByteArray& key, const double& value, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA).form("KeyValueMap::insert[double]: trying to "
				       "insert value \"%f\" for key\n           -->", value) << key << "<--.\n";
  QByteArray temp;
  // -----
  temp.setNum(value);
  return insert(key, temp, force);
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, double& value) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[double]: trying to get "
      "FLOAT value for key " << key << endl;
  QByteArray v;
  bool ok;
  double temp;
  // -----
  if (!get(key, v))
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[int]: key "
					<<key<<" not in "
		 "KeyValueMap." << endl;
      return false;
    }
  // -----
  temp=v.toDouble(&ok);
  if (ok)
    {
      value=temp;
      return true;
    } else {
      return false;
    }
  // ###########################################################################
}

// (^^^ doubles)
// lists of strings:

bool
KeyValueMap::get(const QByteArray& key, list<QByteArray>& values) const
{
  register bool GUARD; GUARD=false;
  kDebug(!values.empty(), KAB_KDEBUG_AREA) << "KeyValueMap::get[string list]: "
      "attention!\n             \"values\" list reference is not "
      "empty!" << endl;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[string list]: trying to "
      "decode string list for key " << key << endl;
  QByteArray raw, part, value;
  int first=1, second=1, i;
  // ----- get the string value as a whole:
  if (!getRaw(key, raw))
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[list<string>]: key "
				      << key << " not in KeyValueMap." << endl;
      return false;
    }
  // -----
  for (;;)
    { // ----- parten the string down into a list, find special characters:
      second=first;
      for (;;)
	{
	  second=raw.indexOf('\\', second);
	  // ----- this may never be the last and also not the second last
	  //       character in a complex string:
	  if (second!=-1)
	    { // ----- check for string end:
	      // we use "\e" as token for the string-delimiter
	      if (raw[second+1]=='e' // the right character
		 && raw[second-1]!='\\') // not escaped
		{
		  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get"
			     "[list<string>]: found string end at pos " <<
			     second << endl;
		  break;
		} else {
		  ++second;
		}
	    } else {
	      break;
	    }
	}
      if (second!=-1)
	{
	  // ----- now second points to the end of the substring:
	  part="\""+raw.mid(first, second-first)+"\"";
	  // ----- insert decoded value into the list:
	  if (parseComplexString(part, 0, value, i))
	    {
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get"
		  "[list<string>]: found item " << value << endl;
	      values.push_back(value);
	    } else {
	      kDebug() << "KeyValueMap::get[list<string>]: parse error." << endl;
	      return false;
	    }
	  if (second<raw.length()-3)
	    { // ----- there may be another string
	      first=second+2;
	    } else { // ----- we are completely finished
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<
			 "KeyValueMap::get[list<string>]: list end found." << endl;
	      break;
	    }
	} else { // ----- finished:
	  break;
	}
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[list<string>]: done." << endl;
  return true;
  // ###########################################################################
}

bool
KeyValueMap::insert(const QByteArray& key, const list<QByteArray>& values, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[string list]: "
	     "coding string list." << endl;
  QByteArray value="\"";
  QByteArray temp;
  list<QByteArray>::const_iterator pos;
  // ----- create coded string list:
  for (pos=values.begin();
      pos!=values.end();
      pos++)
    { // create strings like "abc\efgh\eijk":
      temp=makeComplexString(*pos);
      temp.remove(0, 1); // remove the leading "\""
      temp.remove(temp.length()-1, 1); // the trailing "\""
      value+=temp;
      value+="\\e";
    }
  value+="\""; // finish the string
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[string list]: result "
      "of coding is " << value << endl;
  // ----- insert it without coding:
  return insertRaw(key, value, force);
  // ###########################################################################
}

// (^^^ lists of strings)
// QStrList-s:

bool
KeyValueMap::get(const QByteArray& key, QList<QByteArray>& values) const
{
  register bool GUARD; GUARD=false;
  kDebug(!values.isEmpty(), KAB_KDEBUG_AREA) << "KeyValueMap::get[QStrList]: "
      "attention!\n             \"values\" list reference is not "
      "empty!" << endl;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStrList]: trying to "
      "decode string list for key " << key << endl;
  QByteArray raw, part, value;
  int first=1, second=1, i;
  // ----- get the string value as a whole:
  if (!getRaw(key, raw))
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStrList]: key "
					<< key <<" not in KeyValueMap." << endl;
      return false;
    }
  // -----
  for (;;)
    { // ----- parten the string down into a list, find special characters:
      second=first;
      for (;;)
	{
	  second=raw.indexOf('\\', second);
	  // ----- this may never be the last and also not the second last
	  //       character in a complex string:
	  if (second!=-1)
	    { // ----- check for string end:
	      // we use "\e" as token for the string-delimiter
	      if (raw[second+1]=='e' // the right character
		 && raw[second-1]!='\\') // not escaped
		{
		  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStrList]:"
		      " found string end at pos %i." << second << endl;
		  break;
		} else {
		  ++second;
		}
	    } else {
	      break;
	    }
	}
      if (second!=-1)
	{
	  // ----- now second points to the end of the substring:
	  part="\""+raw.mid(first, second-first)+"\"";
	  // ----- insert decoded value into the list:
	  if (parseComplexString(part, 0, value, i))
	    {
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStrList]: "
		  "found item " << value << endl;
	      values.append(value);
	    } else {
		kDebug() << "KeyValueMap::get[QStrList]: parse error." << endl;
	      return false;
	    }
	  if (second<raw.length()-3)
	    { // ----- there may be another string
	      first=second+2;
	    } else { // ----- we are completely finished
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStrList]: "
			 "list end found." << endl;
	      break;
	    }
	} else { // ----- finished:
	  break;
	}
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStrList]: done." << endl;
  return true;
  // ###########################################################################
}

bool
KeyValueMap::insert(const QByteArray& key, const QList<QByteArray>& values, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
	     "KeyValueMap::insert[QStrList]: coding string list." << endl;
  QByteArray value="\"";
  QByteArray temp;
  // ----- create coded string list:
  for (int count=0; count<values.count(); ++count)
    { // create strings like "abc\efgh\eijk":
      temp=makeComplexString(values.at(count));
      temp.remove(0, 1); // remove the leading "\""
      temp.remove(temp.length()-1, 1); // the trailing "\""
      value+=temp;
      value+="\\e";
    }
  value+="\""; // finish the string
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "KeyValueMap::insert[QStrList]: result of coding is %s." <<
      value << endl;
  // ----- insert it without coding:
  return insertRaw(key, value, force);
  // ###########################################################################
}

// (^^^ QStrList-s)
// QStringList-s:

bool
KeyValueMap::get(const QByteArray& key, QStringList& values) const
{
  register bool GUARD; GUARD=false;
  kDebug(!values.isEmpty(), KAB_KDEBUG_AREA) << "KeyValueMap::get"
      "[QStringList]: attention!\n             \"values\" list reference"
      " is not empty!" << endl;
  // ###########################################################################
  /* The values are stored in a utf8-coded set of QCStrings.
     This list is retrieved and converted back to Unicode strings. */
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStringList]: trying to "
      "decode QStringList for key " << key << endl;
  QList<QByteArray> temp;
  // ----- get the plain C strings:
  if (!get(key, temp))
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStringList]: key "
				      << key <<
	  " not in KeyValueMap." << endl;
      return false;
    }
  // ----- do the conversion:
  for (int count=0; count<temp.count(); ++count)
    {
      values.append(QString::fromUtf8(temp.at(count)));
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QStringList]: done." << endl;
  return true;
  // ###########################################################################
}

bool
KeyValueMap::insert(const QByteArray& key, const QStringList& values, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
	     "KeyValueMap::insert[QStringList]: coding QStringList." << endl;
  // The method simply creates a list of utf8-coded strings and inserts it.
  QList<QByteArray> utf8strings;
  int count;
  // ----- create QCString list:
  for (count=0; count<values.count(); ++count)
    {
      utf8strings.append(values.at(count).toUtf8());
    }
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[QStringList]: done." << endl;
  return insert(key, utf8strings, force);
  // ###########################################################################
}

// (^^^ QStringList-s)
// lists of doubles:

bool
KeyValueMap::insert(const QByteArray& key, const list<double>& values, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[double list]: trying "
	     "to insert double list into map." << endl;
  QByteArray buffer;
  // QCString value(30*values.size()); // not usable with Qt 2
  QByteArray value; // WORK_TO_DO: how to reserve enough space to avoid growing?
  list<double>::const_iterator pos;
  // -----
  for (pos=values.begin(); pos!=values.end(); ++pos)
    {
      buffer.setNum(*pos);
      value=value+buffer+", ";
    }
  if (!value.isEmpty())
    { // remove last comma and space:
      value.remove(value.length()-2, 2);
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[double list]: "
      "constructed string value is " << value << endl;
  return insert(key, value, force);
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, list<double>& values) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[double list]: trying to "
      "decode double list for key " << key << endl;
  kDebug(!values.empty(), KAB_KDEBUG_AREA) << "KeyValueMap::get[double list]: "
      "attention - list should be empty but is not." << endl;
  QByteArray value;
  list<QByteArray> tokens;
  list<QByteArray>::iterator pos;
  double temp;
  bool ok;
  // -----
  if (!get(key, value))
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::get[double list]: no such key." << endl;
      return false;
    }
  // -----
  tokenize(tokens, value, ',');
  for (pos=tokens.begin(); pos!=tokens.end(); ++pos)
    {
      temp=(*pos).toDouble(&ok);
      if (ok)
	{
	  values.push_back(temp);
	} else {
	    kDebug() << "KeyValueMap::get[double list]: conversion error for "
		      << *pos << endl;
	}
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[int list]: done." << endl;
  // ###########################################################################
  return true;
}

// (^^^ lists of doubles)
// QDates:

bool
KeyValueMap::insert(const QByteArray& key, const QDate& value, bool force)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[QDate]: trying to "
	     "insert QDate into map." << endl;
  list<long> values;
  // -----
  if (!value.isValid())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::insert[QDate]: invalid "
		 "date, inserting a null date." << endl;
      for (int i=0; i<3; ++i) values.push_back(0);
    } else {
      values.push_back(value.year());
      values.push_back(value.month());
      values.push_back(value.day());
    }
  // -----
  return insert(key, values, force);
  // ###########################################################################
}

bool
KeyValueMap::get(const QByteArray& key, QDate& date) const
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "KeyValueMap::get[QDate]: trying to decode"
      " QDate for key " << key << endl;
  list<long> values;
  long y, m, d;
  QDate temp;
  // -----
  if (!get(key, values))
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::get[QDate]: no such key." << endl;
      return false;
    }
  if (values.size()!=3)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::get[QDate]: more or less than 3 values." << endl;
      return false;
    }
  y=values.front(); values.pop_front();
  m=values.front(); values.pop_front();
  d=values.front();
  // -----
  if (y!=0 || m!=0 || d!=0) temp.setYMD(y, m, d); // avoid QDate messages
  if (!temp.isValid() && !temp.isNull())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::get[QDate]: no valid date." << endl;
      return false;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "KeyValueMap::get[QDate]: done." << endl;
      date=temp;
      return true;
    }
  // ###########################################################################
}

// (^^^ QDates)
// Section class:

const int Section::indent_width=2;

Section::Section()
{
  // ###########################################################################
  // ###########################################################################
}

Section::Section(const KeyValueMap& contents)
{
  // ###########################################################################
  keys=contents;
  // ###########################################################################
}

bool
Section::add(const QByteArray& name)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::add: adding section \""
				  <<name<<"\" to "
      "this section ..." << endl;
  Section* section;
  bool rc;
  // -----
  if (name.isEmpty())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::add: empty key." << endl;
      return false;
    }
  section=new Section; // create an empty section
  if (section==0)
    {
	kDebug() << "Section::add: out of memory." << endl;
      return false;
    }
  rc=add(name, section);
  if (!rc)
    {
	kDebug(GUARD && !rc, KAB_KDEBUG_AREA) << " failed.\n";
	delete section;
	return false;
    } else {
	kDebug(GUARD && rc, KAB_KDEBUG_AREA) << " success.\n";
	return true;
    }
  // ###########################################################################
}

bool
Section::add(const QByteArray& name, Section* section)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  if (sections.insert(StringSectionMap::value_type(name, section)).second)
    {
	kDebug(GUARD, KAB_KDEBUG_AREA) <<
	    "Section::add: added section "<<name<<" successfully.\n";
      return true;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::add: failed to add section "
				      << name << ", section already exists.\n";
      return false;
    }
  // ###########################################################################
}

bool
Section::find(const QByteArray& name, StringSectionMap::iterator& result)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::find: trying to get section "
      "\""<<name<<"\" ... \n";
  StringSectionMap::iterator pos;
  // -----
  pos=sections.find(name);
  if (pos==sections.end())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "failed, no such section." << endl;
      return false;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "success." << endl;
      result=pos;
      return true;
    }
  // ###########################################################################
}

bool
Section::remove(const QByteArray& name)
{
  // ###########################################################################
  StringSectionMap::iterator pos;
  // -----
  if (!find(name, pos))
    {
      return false; // no such section
    } else {
      sections.erase(pos);
      return true;
    }
  // ###########################################################################
}

bool
Section::find(const QByteArray& name, Section*& section)
{
  // ###########################################################################
  StringSectionMap::iterator pos;
  // -----
  if (!find(name, pos))
    {
      return false;
    } else {
      section=(*pos).second;
      return true;
    }
  // ###########################################################################
}

KeyValueMap*
Section::getKeys()
{
  // ###########################################################################
  return &keys;
  // ###########################################################################
}

void
Section::insertIndentSpace(QTextStream& file, int level)
{
  // ###########################################################################
  int i, j;
  // -----
  for (i=0; i<level; i++)
    {
      for (j=0; j<indent_width; j++)
	file << ' ';
    }
  // ###########################################################################
}

bool
Section::save(QTextStream& stream, int level)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  StringSectionMap::iterator pos;
  // -----
  if (!sections.empty())
    { // ----- insert a comment:
      insertIndentSpace(stream, level);
      stream << "# subsections:" << endl;
    }
  for (pos=sections.begin(); pos!=sections.end(); ++pos)
    {
      insertIndentSpace(stream, level);
      stream << '[' << (*pos).first << ']' << endl;
      if (!(*pos).second->save(stream, level+1))
	{
	  kDebug() << "Section::save: error saving child section \"" << (*pos).first.data() << "\"." << endl;
	  return false;
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::save: saved section \""
					  << (*pos).first
					  << "\".\n";
	}
      insertIndentSpace(stream, level);
      stream << "[END " << (*pos).first << ']' << endl;
    }
  if (!keys.empty())
    {
      insertIndentSpace(stream, level);
      stream << "# key-value-pairs:" << endl;
      if (!keys.save(stream, level*indent_width))
	{
	  kDebug() << "Section::save: error saving key-value-pairs." << endl;
	  return false;
	}
    }
  // -----
  return true;
  // ###########################################################################
}

bool
Section::readSection(QTextStream& file, bool finish)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::readSection: reading section." << endl;
  QByteArray line;
  QByteArray name;
  Section* temp;
  // -----
  for (;;)
    {
      line="";
      line=ReadLineFromStream(file);
      if (isEndOfSection(line))
	{ // ----- eof does not matter:
	  return true;
	} else { // ----- verify it:
	  if (file.atEnd())
	    {
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		  "Section::readSection: EOF, line is \""<<line<<"\".\n";
	      if (!line.isEmpty())
		{
		  if (!keys.insertLine(line, false, true, false))
		    {
		      kWarning() << "Attention: unable to parse key-value-pair "
			   << endl << "\t\"" << line << "\"," << endl
			   << "ignoring and continuing (maybe duplicate "
			   << "declaration of the key)."
			   << endl;
		    }
		}
	      if (finish==true)
		{
		  kDebug() << "Section::readSection: missing end of section." << endl;
		  return false;
		} else {
		  kDebug(GUARD, KAB_KDEBUG_AREA) <<
			     "Section::readSection: EOF (no error)." << endl;
		  return true;
		}
	    }
	}
      if (isBeginOfSection(line))
	{
	  name=nameOfSection(line);
	  add(name);
	  find(name, temp);
	  if (!temp->readSection(file))
	    {
		kDebug() << "Section::readSection: unable to read "
		    "subsection \"" << name << "\".\n";
	      return false;
	    }
	} else { // ----- it has to be a key-value-pair:
	  if (!keys.insertLine(line, false, true, false))
	    {
	      kWarning() << "Attention: unable to parse key-value-pair " << endl
		   << "\t\"" << line << "\"," << endl
		   << "ignoring and continuing (maybe duplicate declaration of"
		   << " the key)."
		   << endl;
	    }
	}
    }
  // ###########################################################################
}

bool
Section::isBeginOfSection(QByteArray line)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  line=line.simplified();
  if (line.isEmpty() || line.length()<2)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::isBeginOfSection: too short "
		 "or empty line." << endl;
      return false;
    }
  if (line[0]!='[' || line[line.length()-1]!=']')
    {
      return false;
    }
  // -----
  if (line.contains("END"))
    {
      return false;
    } else {
      return true;
    }
  // ###########################################################################
}

bool
Section::isEndOfSection(QByteArray line)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::isEndOfSection: is "
				  << line <<" the end of"
	     " a section?" << endl;
  int first=1, second;
  QByteArray temp;
  // -----
  line=line.simplified();
  if (line.isEmpty() || line.length()<2)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "Section::isBeginOfSection: too short "
		 "or empty line." << endl;
      return false;
    }
  if (line[0]!='[' || line[line.length()-1]!=']')
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "Section::isBeginOfSection: does not match." << endl;
      return false;
    }
  // ----- find the word inside the brackets:
  for (first=1; line[first]==' '; ++first); // find first non-whitespace character
  for (second=first; line[second]!=' ' && line[second]!=']'; ++second);
  temp=line.mid(first, second-first);
  if (temp=="END")
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "Section::isBeginOfSection: yes, it is." << endl;
      return true;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "Section::isBeginOfSection: no, it is not." << endl;
      return false;
    }
  // ###########################################################################
}

QByteArray
Section::nameOfSection(const QByteArray& line)
{
  register bool GUARD; GUARD=false;
  // ###########################################################################
  int first=1, second;
  QByteArray temp;
  // -----
  temp=line.simplified();
  if (temp.isEmpty() || temp.length()<=2)
    { // empty section names are not allowed
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "Section::isBeginOfSection: too short or empty line." << endl;
      return "";
    }
  if (temp[0]!='[' || temp[temp.length()-1]!=']')
    {
      return "";
    }
  // ----- find the word inside the brackets:
  for (first=1; temp[first]==' '; ++first); // find first non-whitespace character
  for (second=first; temp[second]!=' ' && temp[second]!=']'; ++second);
  temp=temp.mid(first, second-first);
  if (temp=="END")
    {
      return "";
    } else {
      return temp;
    }
  // ###########################################################################
}

bool
Section::clear()
{
  // ###########################################################################
  StringSectionMap::iterator pos;
  // -----
  for (pos=sections.begin(); pos!=sections.end(); ++pos)
    {
      if (!(*pos).second->clear()) return false;
      delete(*pos).second;
    }
  // sections.clear(); // seems to be not implemented
  sections.erase(sections.begin(), sections.end());
  keys.clear();
  // -----
  return true;
  // ###########################################################################
}

bool
Section::empty()
{
  // ###########################################################################
  return keys.empty() && sections.empty();
  // ###########################################################################
}

Section::StringSectionMap::iterator
Section::sectionsBegin()
{
  // ###########################################################################
  return sections.begin();
  // ###########################################################################
}

Section::StringSectionMap::iterator
Section::sectionsEnd()
{
  // ###########################################################################
  return sections.end();
  // ###########################################################################
}

unsigned int
Section::noOfSections()
{
  // ###########################################################################
  return sections.size();
  // ###########################################################################
}

QConfigDB::QConfigDB(QWidget* parent)
  : QWidget(parent),
    timer(0),
    readonly(true),
    locked(false),
    mtime(new QDateTime)
{
  // ###########################################################################
  hide();
  // ###########################################################################
}


QConfigDB::~QConfigDB()
{
  // ############################################################################
  // disconnect();
  // -----
  if (timer!=0)
    {
      delete timer; timer=0;
    }
  if (!clear()) // this will emit changed() a last time
    {
      kDebug() << "QConfigDB destructor: cannot remove me." << endl;
    }
  if (locked)
    {
      unlock();
    }
  // ############################################################################
}

bool QConfigDB::invariant()
{
  return true;
}

bool
QConfigDB::get(const list<QByteArray>& key, KeyValueMap*& map)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::get: trying to get keys ... " << endl;
  Section* section=&top;
  list<QByteArray>::const_iterator pos;
  // -----
  if (key.empty())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "ConfigDB::get: path is empty, returning toplevel section." << endl;
      map=top.getKeys();
      return true;
    }
  for (pos=key.begin(); pos!=key.end(); ++pos)
    {
      if (!section->find(*pos, section))
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "failed,\n               at least the element \""
					  << *pos <<
	      "\" of "
		     "the key-list is not declared." << endl;
	  return false;
	}
    }
  // -----
  map=section->getKeys();
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "success." << endl;
  return true;
  // ############################################################################
}

KeyValueMap*
QConfigDB::get()
{
  // ############################################################################
  return top.getKeys();
  // ############################################################################
}

bool
QConfigDB::createSection(const list<QByteArray>& key)
{
  // ############################################################################
  Section* section=&top;
  unsigned int index;
  list<QByteArray>::const_iterator pos;
  Section* thenewone;
  bool rc;
  // -----
  pos=key.begin();
  for (index=0; index<key.size()-1; index++)
    {
      if (!section->find(*pos, section))
	{ // this section is not declared
	  Section* temp=new Section; // WORK_TO_DO: memory hole?
	  if (section->add(*pos, temp))
	    {
	      section=temp;
	    } else {
	      delete temp;
	      return false;
	    }
	}
      ++pos;
    }
  // pos now points to the last element of key,
  // section to the parent of the section that will be inserted
  thenewone=new Section;
  rc=section->add(*pos, thenewone);
  return rc; // missing error report! WORK_TO_DO
  // ############################################################################
}

bool
QConfigDB::clear()
{
  // ############################################################################
  bool rc=top.clear();
  emit(changed(this));
  return rc;
  // ############################################################################
}

bool
QConfigDB::empty()
{
  // ############################################################################
  return top.empty();
  // ############################################################################
}

bool
QConfigDB::createSection(const QByteArray& desc)
{
  // ############################################################################
  return createSection(stringToKeylist(desc));
  // ############################################################################
}

bool
QConfigDB::get(const QByteArray& key, KeyValueMap*& map)
{
  // ############################################################################
  return get(stringToKeylist(key), map);
  // ############################################################################
}

list<QByteArray>
QConfigDB::stringToKeylist(const QByteArray& desc)
{
  register bool GUARD; GUARD=false;
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::stringToKeylist: parsing path " << desc << endl;
  // ############################################################################
  list<QByteArray> key;
  int first=0, second;
  QByteArray temp;
  // -----
  if (desc.isEmpty())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "QConfigDB::stringToKeylist: path is empty." << endl;
      return key;
    }
  for (;;)
    {
      second=desc.indexOf('/', first);
      if (second==-1)
	{
	  if (first<desc.length()+1)
	    {
	      temp=desc.mid(first, desc.length()-first);
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<
			 "QConfigDB::stringToKeylist: found last part "
					      << temp << endl;
	      key.push_back(temp);
	    }
	  break;
	}
      temp=desc.mid(first, second-first);
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
	  "QConfigDB::stringToKeylist: found part " << temp << endl;
      key.push_back(temp);
      first=second+1;
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::stringToKeylist: done." << endl;
  return key;
  // ############################################################################
}


bool
QConfigDB::get(const QByteArray& key, Section*& section)
{
  // ############################################################################
  return get(stringToKeylist(key), section);
  // ############################################################################
}

bool
QConfigDB::get(const list<QByteArray>& key, Section*& section)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::get: searching section ... " << endl;
  Section* temp=&top;
  list<QByteArray>::const_iterator pos;
  // -----
  for (pos=key.begin(); pos!=key.end(); ++pos)
    {
      if (!temp->find(*pos, temp))
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "failure, no such section.";
	  return false;
	}
    }
  // -----
  section=temp;
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "success, section found." << endl;
  return true;
  // ############################################################################
}

bool
QConfigDB::isRO()
{
  // ############################################################################
  return readonly;
  // ############################################################################
}

int
QConfigDB::IsLocked(const QString& file)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  QString lockfile=file+".lock";
  int pid=-1;
  // -----
  if (access(QFile::encodeName(lockfile), F_OK)==0)
    {
      QFile f(lockfile);
      // -----
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::IsLocked: the file\n"
				      << file <<
	  "\nhas a lockfile.\n";
      if (f.open(QIODevice::ReadOnly))
	{
	  QTextStream stream(&f);
	  stream.setCodec( "ISO 8859-1" ); // no conversion
	  // -----
	  stream >> pid;
	  if (pid==-1)
	    {
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::IsLocked: the file "
			 "does not contain the ID\n        of the process that "
			 "created it." << endl;
	      return -1;
	    }
	  f.close();
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::IsLocked: cannot open the lockfile." << endl;
	  return -1;
	}
      return pid;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::IsLocked: the file\n"
				      << file << "\nhas no lockfile.\n";
      return 0;
    }
  // ############################################################################
}

bool
QConfigDB::lock()
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  if (locked)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::lock (current file): file "
		 "is already locked by this object." << endl;
      return false;
    }
  if (lock(filename))
    {
      locked=true;
      return true;
    } else {
      return false;
    }
  // ############################################################################
}

bool
QConfigDB::lock(const QString& file)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::lock: locking the file "
				  << file << endl;
  QString lockfile=file+".lock";
  QFile f(lockfile);
  // -----
  if (access(QFile::encodeName(lockfile), F_OK)==0)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::lock: the file is locked by"
		 " another process." << endl;
      return false;
    } else {
      if (f.open(QIODevice::WriteOnly))
	{
	  QTextStream stream(&f);
	  stream.setCodec( "ISO 8859-1" ); // no conversion
	  // -----
	  stream << getpid() << endl;
	  f.close();
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::lock: unable to create"
		     " lockfile." << endl;
	  return false;
	}
    }
  // -----
  LockFiles.push_back(lockfile);
  return true;
  // ############################################################################
}

bool
QConfigDB::unlock()
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::unlock: unlocking the file "
				  << filename << endl;
  QString lockfile=filename+".lock";
  list<QString>::iterator pos;
  // -----
  if (!locked)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::unlock: this app did not "
		 "lock the file!" << endl;
      return false;
    }
  if (access(QFile::encodeName(lockfile), F_OK | W_OK)==0)
    {
      if (::remove(QFile::encodeName(lockfile))==0)
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::unlock: lockfile deleted." << endl;
	  for (pos=LockFiles.begin(); pos!=LockFiles.end(); ++pos)
	    {
	      if ((*pos)==lockfile) break;
	    }
	  if (pos!=LockFiles.end())
	    {
	      pos = LockFiles.erase(pos);
	    } else {
	      kDebug() << "QConfigDB::unlock: file not mentioned in lockfile" << endl;
	    }
	  locked=false;
	  return true;
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::unlock: unable to "
		     "delete lockfile.n" << endl;
	  return false;
	}
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::unlock: the file is not"
		 " locked or permission has been denied." << endl;
      return false;
    }
  // ############################################################################
}

void
QConfigDB::CleanLockFiles(int)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  list<QString>::iterator pos;
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA).form("QConfigDB::CleanLockFiles: removing %i "
	     "remaining lockfiles.", LockFiles.size()) << endl;
  for (pos=LockFiles.begin(); pos!=LockFiles.end(); ++pos)
    {
      if (::remove(QFile::encodeName(*pos))==0)
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
	      "                          " << *pos << " removed.\n";
	  pos = LockFiles.erase(pos);
	} else {
	    kDebug() << "                          could not remove  " << *pos << endl;
	}
    }
  // -----
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::CleanLockFiles: done." << endl;
  // ############################################################################
}

void
QConfigDB::watch(bool state)
{
  // ############################################################################
  if (state)
    { // start timer
      if (timer==0)
	{
	  timer=new QTimer(this);
	  connect(timer, SIGNAL(timeout()), SLOT(checkFileChanged()));
	}
      timer->start(1000);
    } else { // stop timer
      if (timer!=0)
	{
	  timer->stop();
	}
    }
  // ############################################################################
}

bool
QConfigDB::CheckLockFile(const QString& file)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::CheckLockFile: called." << endl;
  int pid;
  // -----
  pid=IsLocked(file);
  if (pid==0)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::CheckLockFile: the file is "
		 "not locked." << endl;
      return false;
    }
  if (pid>0)
    {
      if (kill(pid, 0)!=0)
	{ // ----- no such process, we may remove the lockfile:
	  return false;
	}
    }
  if (pid<0)
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::CheckLockFile: the file has "
		 "not been created by QConfigDB::lock." << endl;
    }
  // ----- check system time and creation time of lockfile:
  // WORK_TO_DO: not implemented
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::CheckLockFile: done." << endl;
  return true;
  // ############################################################################
}

bool
QConfigDB::checkFileChanged()
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  // kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::checkFileChanged: called." << endl;
  if (filename.isEmpty())
    { // ----- false, as file does not exist and thus may be stored anyway
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::checkFileChanged: no filename." << endl;
      return false;
    }
  QFileInfo file(filename);
  // -----
  if (file.exists())
    {
      if (file.lastModified() > *mtime)
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::checkFileChanged: file has been changed.n" << endl;
	  emit(fileChanged());
	  return true;
	} else {
	  return false;
	}
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::checkFileChanged: could "
		 "not stat file, file does not exist." << endl;
      if (!mtime->isValid())
	{ // the file did never exist for us:
	  return false; // ... so it has not changed
	} else { // it existed, and now it does no more
	  emit(fileChanged());
	  return true;
	}
    }
  // ############################################################################
}

bool
QConfigDB::storeFileAge()
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::storeFileAge: called." << endl;
  QFileInfo file(filename);
  // -----
  if (file.exists())
    {
      *mtime=file.lastModified();
      return true;
    } else {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::save: could not stat file." << endl;
      *mtime=QDateTime(); // a null date
      return false;
    }
  // ############################################################################
}


bool
QConfigDB::setFileName(const QString& filename_, bool mustexist, bool readonly_)
{
  register bool GUARD; GUARD=false;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::setFileName: setting filename "
      "to \""
				  << filename_ <<"\"" << (readonly_ ? " (read only)" : "") << endl;
  // ----- remove previous lock:
  if (locked)
    {
      if (!unlock())
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::setFileName: cannot "
		     "release previous lock." << endl;
	  return false;
	}
    }
  // ----- remove possible stale lockfile:
  if (IsLocked(filename_)!=0 && !CheckLockFile(filename_))
    { // ----- it is stale:
      if (::remove(QFile::encodeName(filename_+".lock"))==0)
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::setFileName: removed stale lockfile." << endl;
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::setFileName: cannot remove stale lockfile." << endl;
	  return false;
	}
    }
  // -----
  if (mustexist)
    {
      if (access(QFile::encodeName(filename_), readonly_==true ? R_OK : W_OK | R_OK)==0)
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::setFileName: permission granted." << endl;
	  if (!readonly_)
	    { //       we need r/w access:
	      if (lock(filename_))
		{
		  locked=true;
		} else {
		  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::setFileName: "
		     "could not lock the file." << endl;
		  return false;
		}
	    }
	  readonly=readonly_;
	  filename=filename_;
	  storeFileAge();
	  return true;
	} else {
	  kDebug() << "QConfigDB::setFileName: permission denied, " << endl;
	  return false;
	}
    } else {
      if (access(QFile::encodeName(filename_), F_OK)==0)
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::setFileName: file exists." << endl;
	  if (access(QFile::encodeName(filename_), W_OK | R_OK)==0)
	    {
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<
			 "QConfigDB::setFileName: permission granted." << endl;
	      if (!readonly_)
		{ //       we need r/w access:
		  if (lock(filename_))
		    {
		      locked=true;
		    } else {
		      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::setFileName: "
		      "could not lock the file." << endl;
		      return false;
		    }
		}
	      readonly=readonly_;
	      filename=filename_;
	      storeFileAge();
	      return true;
	    } else {
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::setFileName: "
	      "permission denied, filename not set." << endl;
	      return false;
	    }
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
	  "QConfigDB::setFileName: permission granted, new file." << endl;
	  readonly=readonly_;
	  filename=filename_;
	  if (!readonly)
	    {
	      if (!lock())
		{
		  kDebug(GUARD, KAB_KDEBUG_AREA) <<
			     "QConfigDB::setFileName: could not lock the file." << endl;
		  return false;
		}
	    }
	  storeFileAge();
	  return true;
	}
    }
  // ############################################################################
}

QString
QConfigDB::fileName()
{
  // ############################################################################
  return filename;
  // ############################################################################
}

bool
QConfigDB::save(const char* header, bool force)
{
  register bool GUARD; GUARD=true;
  // ############################################################################
  kDebug(GUARD, KAB_KDEBUG_AREA) <<
      "QConfigDB::save: saving database -->" << filename << "<--.\n";
  bool wasRO=false;
  bool rc;
  // -----
  if (checkFileChanged())
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<
		 "QConfigDB::save: file is newer, not saving." << endl;
      return false;
    }
  if (force && isRO())
    {
      if (setFileName(fileName(), true, false))
	{
	  wasRO=true;
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::save: switched to (forced) r/w mode." << endl;
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::save: cannot switch to (forced) r/w mode." << endl;
	  return false;
	}
    }
  // ----- now save it:
  if (!isRO())
    {
      QFile file(filename);
      if (file.open(QIODevice::WriteOnly))
	{
	  QTextStream stream(&file);
	  stream.setCodec( "ISO 8859-1" ); // no conversion
	  // -----
	  if (header!=0)
	    {
	      stream << "# " << header << endl;
	    }
	  stream << '#' << " [File created by QConfigDB object "
		 << version() << "]" << endl;
	  if (!top.save(stream)) // traverse tree
	    {
	      kDebug(GUARD, KAB_KDEBUG_AREA) <<
			 "QConfigDB::save: error saving subsections." << endl;
	    }
	  storeFileAge();
	  file.close();
	  rc=true;
	} else {
	    kDebug() << "QConfigDB::save: error opening file \""
		      << filename <<
		"\" for writing.\n";
	  rc=false;
	}
    } else {
      rc=false;
    }
  // ----- reset r/o mode:
  if (wasRO) // only true if we switched to forced r/w mode here
    {
      if (setFileName(fileName(), false, true))
	{
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::save: reset (forced) r/w mode." << endl;
	} else {
	  kDebug(GUARD, KAB_KDEBUG_AREA) <<
		     "QConfigDB::save: cannot reset (forced) r/w mode." << endl;
	  rc=false;
	}
    }
  // -----
  return rc;
  // ############################################################################
}

bool
QConfigDB::load()
{
  register bool GUARD; GUARD=false ;
  // ############################################################################
  QFile file(filename);
  // -----
  if (file.open(QIODevice::ReadOnly))
    {
      kDebug(GUARD, KAB_KDEBUG_AREA) <<  "QConfigDB::load: file access OK." << endl;
      QTextStream stream(&file);
      stream.setCodec( "ISO 8859-1" ); // no conversion
      // -----
      clear();
      bool rc=top.readSection(stream, false);
      storeFileAge();
      file.close();
      emit(changed(this));
      kDebug(GUARD, KAB_KDEBUG_AREA) << "QConfigDB::load: done." << endl;
      return rc;
    } else {
      kDebug() << "QConfigDB::load: error opening file \"" << filename << "\" for reading." << endl;
      return false;
    }
  // ############################################################################
}
