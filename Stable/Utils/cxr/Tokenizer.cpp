/*********************************************************************

   Copyright (C) 2002 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

**********************************************************************/

// Tokenizer.cpp: implementation of the CTokenizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tokenizer.h"
#include <string>
#include <string.h>

#ifdef _WIN32
   #define strncasecmp strnicmp
#endif

// CXR does not have comments
#undef ALLOW_COMMENTS

// strip quotes on strings, please
#undef RETURN_QUOTED_STRINGS

#ifdef ALLOW_COMMENTS
#define COMMENT_START   "/*"
#define COMMENT_STOP    "*/"
#endif

#define TRACE fprintf(stderr, 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTokenizer::CTokenizer(const char **pKeyWords, 
                       int iKeyWords,
                       const char *pMetaChars,
                       int iMetaChars)
{
   m_pKeyWords = pKeyWords;
   m_iKeyWords = iKeyWords;
   m_MetaChars = pMetaChars;
   m_iMetaChars = iMetaChars;
}

//////////////////////////////////////////////////////////////////////
// bye bye
CTokenizer::~CTokenizer()
{

}

//////////////////////////////////////////////////////////////////////
// remove tokens
void CTokenizer::Clear()
{
   m_tokens.clear();
}

void CTokenizer::Init()
{
   Clear();
}

//////////////////////////////////////////////////////////////////////
// extract an array of tokens from this input text
int CTokenizer::Tokenize(const char *pInputLine)
{
   int err = eErrorNone;

   Init();

   int iLen = strlen(pInputLine);

   bool bInComment = false;

   for (int i=0; i < iLen; i++)
   {
      std::string curTokenString;
      bool bQuotedString = false;

      int iConsumed = GetToken(pInputLine + i, curTokenString, bQuotedString);

      if (iConsumed > 0)
      {
         //if (curTokenString.length() > 0)
         {
#ifdef ALLOW_COMMENTS
            if (curTokenString == COMMENT_START)
            {
               bInComment = true;
               i+=iConsumed;
               continue;
            }
            
            if (curTokenString == COMMENT_STOP)
            {
               if (!bInComment)
               {
                  err = eErrorSyntax;
                  break;
               }

               bInComment = false;

               i+=iConsumed;
               continue;
            }
#endif

            if (!bInComment)
            {
               int iStart = i;
               int iStop = i + iConsumed - 1;

               CSAToken curToken(curTokenString.c_str(), iStart, iStop, bQuotedString);

               m_tokens.push_back(curToken);
            }
         }
         
         i+=iConsumed;

         i--; // back up! the for iteration will increase i
         continue;
      }
      else
      {
         if (IsMetaChar(*(pInputLine + i)))
         {
            if (!isspace(*(pInputLine + i)))
            {
               curTokenString.clear();
               curTokenString+=*(pInputLine + i);

               CSAToken curToken(curTokenString.c_str(), i, curTokenString.length() - 1, bQuotedString);

               m_tokens.push_back(curToken);
            }
         }
      }
   }

   return err;
}

//////////////////////////////////////////////////////////////////////
// is this text a keyword?
bool CTokenizer::IsKeyWord(std::string &str)
{
   return IsKeyWord(str.c_str());
}

//////////////////////////////////////////////////////////////////////

bool CTokenizer::IsKeyWord(const char *pInput)
{
   if (m_pKeyWords==NULL)
   {
      return false;
   }

   int istrlen = strlen(pInput);

   int iBestMatch = -1;

   for (int i=0; i < m_iKeyWords; i++)
   {
      int iCurKWLen = strlen(m_pKeyWords[i]);

      if (istrlen <= iCurKWLen)
      {
         if (strncasecmp(m_pKeyWords[i], pInput, iCurKWLen)==0)
         {
            iBestMatch = i;
         }
      }
   }

   if (iBestMatch==-1)
   {
#ifdef ALLOW_COMMENTS
      if (CSAScrUtil::sa_strnicmp(COMMENT_START, pInput, strlen(COMMENT_START))==0)
      {
         iBestMatch = m_iKeyWords + 1;
      }

      if (CSAScrUtil::sa_strnicmp(COMMENT_STOP, pInput, strlen(COMMENT_STOP))==0)
      {
         iBestMatch = m_iKeyWords + 2;
      }
#endif
   }
   return (iBestMatch != -1);
  
}

//////////////////////////////////////////////////////////////////////
// find the next token in the string

int CTokenizer::GetToken(const char *pInput, std::string &out, bool &bQuotedString)
{
   int istrlen = strlen(pInput);

   bool bFoundChars = false;

   bool bScanningMetaKW = false;

   bool bInQuotes = false;

   bQuotedString = false;

   char c;
   int iWS, i;

   for (iWS = 0; iWS < istrlen; iWS++)
   {
      c = *(pInput + iWS);

	  if (c<0) continue;	// 한글의 후위바이트의 경우 음수가 나올 수 있다. isspace()에서 assert가 걸리므로 넘겨버렸다

      if (!isspace(c))
         break;
      if (!iscntrl(c))
         break;
   }

   for (i=iWS; i<istrlen; i++)
   {
      c = *(pInput + i);

      // not in quotes?
      // open quote
      if ((c=='"') && (!bInQuotes))
      {
         // not in quotes, but we found a ", and we've already found chars?
         if (bFoundChars)
         {
            break;
         }

         bInQuotes = true;
#ifdef RETURN_QUOTED_STRINGS
         out+=c;
#endif
         continue;
      }

      // close quote
      if ((c=='"') && (bInQuotes))
      {
         bInQuotes = false;
#ifdef RETURN_QUOTED_STRINGS
         out+=c;
#endif
         i+=1;

         bQuotedString = true;
         break;
      }

      if (bInQuotes)
      {
         // escape!
         if (c=='\\')
         {
            //out+=c;
            if (i < istrlen - 1)
            {
               switch (*(pInput + i + 1))
               {
                  // if we see \" in a quoted string, don't treat it as a string end!
               case '"':
                  out+= "\\\""; // output the slash and the quote
                  i++; // skip the quote in the string
                  continue;
                  break;
               
               default:
                  // don't touch the other escapes.
                  out+=c;
                  continue;
                  break;
               }
            }
         }

         out+=c;
         continue;
      }

      // break non-quoted strings on ctrl or white space
      if (iscntrl(c) || isspace(c))
      {
         break;
      }

      // metachars...
      if (IsMetaChar(c))
      {
         if (iscntrl(c) || isspace(c))
         {
            break;
         }

         if (!bFoundChars)
         {
            bScanningMetaKW = true;

            bFoundChars = true;

            // in case some KWs start with metachars
				if (i < istrlen - 2)
				{
					char buf[3];
					buf[0]=c;
					buf[1]=*(pInput + i + 1);
					buf[2]=0;

					if (IsKeyWord(buf))
					{
						out+=c;
						out+=*(pInput + i + 1);

						i+=2;

						break;
					}
					else
					{
						buf[1] = 0;
						if (IsKeyWord(buf))
						{
							out+=c;

							i+=1;

							break;
						}
					}
				}

				out+=c;
         }
         else
         {
            break;
         }
      }
      else
      {
         if (!bFoundChars)
         {
            bScanningMetaKW = false;

            bFoundChars = true;

            out+=c;
         }
         else
         {
            if (bScanningMetaKW)
            {
               break;
            }
            else
            {
               out+=c;
            }
         }
      }
   }

   return i;
}

//////////////////////////////////////////////////////////////////////

bool CTokenizer::IsMetaChar(const char c)
{
   for (int i=0; i < m_iMetaChars; i++)
   {
      if (c==m_MetaChars[i])
      {

         return true;
      }
   }

   return false;
}

//////////////////////////////////////////////////////////////////////

bool CTokenizer::IsMetaChar(std::string &str)
{
   if (str.length() > 1)
   {
      return false;
   }

   char c = str.at(0);

   for (int i=0; i < m_iMetaChars; i++)
   {
      if (c==m_MetaChars[i])
      {
         return true;
      }
   }

   return false;
}

//////////////////////////////////////////////////////////////////////

void CTokenizer::Dump()
{
   CSATokenVector::iterator theIterator;

   int i=0;
   for (theIterator = m_tokens.begin(); theIterator < m_tokens.end(); theIterator++)
   {
      TRACE("%d [%d-%d] : \"%s\"\n", i++, (*theIterator).iStart, (*theIterator).iStop, ((*theIterator).csToken)).c_str());
   }
}

//////////////////////////////////////////////////////////////////////

bool operator < (const CSAToken &a, const CSAToken &b)
{
   return (a.iStart < b.iStart);
}

bool operator == (const CSAToken &a, const CSAToken &b)
{
   return (a.iStart==b.iStart);
}
