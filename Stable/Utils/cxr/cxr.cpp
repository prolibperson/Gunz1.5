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

// CXR.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>

#include "CmdLine.h"
#include "Tokenizer.h"
#include "Stream.h"

#define ASSERT(x) (x)

#ifdef _WIN32
   #define snprintf _snprintf
#endif

/////////////////////////////////////////////////////////////////

static int ProcessCmd_Encode(std::string& csInFile, std::string& csOutFile, std::string& csPassword);
static bool         ProcessFile(std::istream &in, std::ostream &out, const std::string inName, const std::string &outName, const std::string& _csPassword);
static void			WriteDecoder(const std::string & csPassword);
static bool         AddDecode(const std::string & csPassword, std::ostream &out);
static std::string  Encrypt(const std::string &csIn, const char *pPass, bool hexEncode= true);
static std::string  Decrypt(const char *pIn, const char *pPass);
static bool         ExpandOctal(const std::string &csIn, std::string &csOut, int &iConsumed);
static std::string TranslateString(const std::string &csIn);
static bool         ExpandHex(const std::string &csIn, std::string &csOut, int &iConsumed);
static std::string  HexEncodeString(const char *pIn);

// these can be adjusted in the range 1 to 239
const int basechar1 = 128;
const int basechar2 = 128;

/////////////////////////////////////////////////////////////////

std::string reverse(const std::string &str) {
   std::string rev= str;
   reverse(rev.begin(), rev.end());
   return rev;
}

int main(int argc, char* argv[])
{
   int nRetCode = 0;

   srand(time(NULL));


   cerr << "Starting CXR, the literal string encryptor. Copyright 2002, Smaller Animals Software Inc.\n";

   if ((basechar1 == 0) || (basechar2 == 0) || (basechar1 > 239) || (basechar2 > 239))
   {
      cerr << "CXR basechar values out of acceptable range. Aborting\n";
      nRetCode = 1;
      return nRetCode;
   }

   CCmdLine cmd;
   if (cmd.SplitLine(argc, argv) >= 3)
   {
      std::string csInFile = cmd.GetSafeArgument("-i", 0, "");
      std::string csOutFile = cmd.GetSafeArgument("-o", 0, "");
	  std::string csPassword = cmd.GetSafeArgument("-p", 0, "");
      if (!csInFile.empty() && !csOutFile.empty())
      {
         ProcessCmd_Encode(csInFile, csOutFile, csPassword);
      }
      else
      {
         cerr << "Not enough parameters" << endl;
         nRetCode = 1;
      }
   }      
   else
   {
      cerr << "Not enough parameters" << endl;
      nRetCode = 1;
   }

   return nRetCode;
}

/////////////////////////////////////////////////////////////////
int ProcessCmd_Encode(std::string& csInFile, std::string& csOutFile, std::string& csPassword)
{
	int nRetCode = 0;

	// open the input file
	std::ifstream fileIn;
	fileIn.open(csInFile.c_str()); 

	if (fileIn.good())
	{
		// open the output file
		std::ofstream fileOut;
		fileOut.open(csOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);      
		if (fileOut.good())
		{
			// TODO: have full path to infile, but just filename for outfile
			if (!ProcessFile(fileIn, fileOut, csInFile, csOutFile, csPassword))
			{
				cerr << "CXR failed\n";
				nRetCode = 1;
			}

			// 디코더를 별도 파일에 출력
			WriteDecoder(csPassword);
		}
		else
		{
			cerr << "Unable to open output file: " << csOutFile << endl;
			nRetCode = 1;
		}
	}
	else
	{
		cerr << "Unable to open input file: " << csInFile << endl;
		nRetCode = 1;
	}

	if (nRetCode==0)
	{
		cerr << "CXR created: " << csOutFile << "\n";
	}

	return nRetCode;
}

/////////////////////////////////////////////////////////////////

bool ProcessFile(std::istream &in, std::ostream &out, const std::string inName, const std::string &outName, const std::string& _csPassword)
{
   enum 
   {
      eStateWantPassword,
      eStateHavePassword,
   };

   //int iState = eStateWantPassword;
   int iState = eStateHavePassword;

   std::string csPassword = _csPassword;
   std::string line;

   const char *pMetachars = "/\\=();'";
   const char *pKeyWords[3] = {"//", "_CXR", "CXRP"};
   
   CTokenizer tokenizer(pKeyWords, 3, pMetachars, strlen(pMetachars));
   int iErr = CTokenizer::eErrorNone;
   bool ok = true;

   out << "/////////////////////////////////////////////////////////////\n";
   out << "// ";
   out << outName;
   out << "\n//\n";
   out << "// This file was generated by CXR, the literal string encryptor.\n";
   out << "// CXR, Copyright 2002, Smaller Animals Software, Inc., all rights reserved.\n";
   out << "//\n";
   out << "// Please do not edit this file. Any changes here will be overwritten on the next compile.\n// If you wish to make changes to a string, please edit:\n//     ";
   out << inName;
   out << "\n//\n";
   out << "\n/////////////////////////////////////////////////////////////\n\n";
   out << "#pragma once\n";
   out << "#include \"cxr_inc.h\"\n\n";
   //out << "#define BYTE unsigned char \n";

   bool bFoundCXR = false;

   do
   {
      std::getline(in, line);
      
      switch (iState)
      {
      case eStateWantPassword:
		  // 이 state는 무시된다. 패스워드를 커맨드라인에서 받아들이도록 변경했기 때문.
         iErr = tokenizer.Tokenize(line.c_str());
         if (iErr == CTokenizer::eErrorNone)
         {
            if (tokenizer.GetTokenCount() >= 4)
            {
               // password declaration always looks like : // CXRP = "Password"
               if ((tokenizer.GetToken(0).csToken == "//") && 
                  (tokenizer.GetToken(1).csToken == "CXRP") && 
                  (tokenizer.GetToken(2).csToken == "=") && 
                  (tokenizer.GetToken(3).bIsQuotedString))
               {
                  // we'll use the password from the file, literally. it's not treated as
                  // a C string-literal, just as a section of a text file. when we
                  // go to write the decoder, we'll have to fix it up to make sure
                  // the compiler gets the same text by adding any necessary escapes.
                  csPassword = tokenizer.GetToken(3).csToken;

                  if (csPassword.empty())
                  {
                     cerr << "Invalid CXR password: \"" << csPassword << "\"" << endl;
                     ASSERT(0);
                     break;
                  }

                  // encrypt the password itself (using its reversal as the encoding password)
                  // so that it won't show up in the binary either
                  csPassword= Encrypt(csPassword, reverse(csPassword).c_str(), false);

                  iState = eStateHavePassword;
                  continue;
               }
            }
         }
         break;
      case eStateHavePassword:
         bFoundCXR = false;
         iErr = tokenizer.Tokenize(line.c_str());
         if (iErr == CTokenizer::eErrorNone)
         {
            if (tokenizer.GetTokenCount() > 4)
            {
               for (int i=0;i<tokenizer.GetTokenCount() - 3; i++)
               {
                  // looking for _CXR ( "..." )
                  if (
                     (tokenizer.GetToken(i).csToken == "_CXR") && !tokenizer.GetToken(i).bIsQuotedString &&
                     (tokenizer.GetToken(i + 1).csToken == "(") && !tokenizer.GetToken(i + 1).bIsQuotedString &&
                     (tokenizer.GetToken(i + 2).bIsQuotedString) &&
                     (tokenizer.GetToken(i + 3).csToken == ")") && !tokenizer.GetToken(i + 3).bIsQuotedString
                     )
                  {
                     std::string csTrans = TranslateString(tokenizer.GetToken(i + 2).csToken);
                     std::string csEnc = Encrypt(csTrans, csPassword.c_str());
                     //std::string csDec = Decrypt(csEnc, csPassword);

                     out << "///////////////////////////\n#ifdef _USING_CXR\n";

                     /*
                     out << "//";
                     out << csDec;
                     out << "\n";
                     */

                     // output up to _CXR
                     out << line.substr(0, tokenizer.GetToken(i).iStart);

					 out << "\t";

					 // '_CXR('을 붙여주자
					 out << "_CXR(";

                     // encrypted stuff
                     out << "\"";
                     out << csEnc;
                     out << "\"";

					 // '_CXR('의 짝인 ')'을 붙여주자
					 out << ")";

                     // to the end of the line
					 if (tokenizer.IsValidTokenIndex(i + 4))
						out << line.substr(tokenizer.GetToken(i + 4).iStart);

                     out << "\n";
 
                     out << "#else\n";
                     out << line;
                     out << "\n#endif\n\n";

                     bFoundCXR = true;

                     break;

                  } // found a good string ?
                  
               } // loop over tokens

            }  // > 4 tokens

         } // tokenizer OK

         if (bFoundCXR)
         {
            continue;
         }
      
         break; // switch
      }

      // done with it
      out << line;
      out << "\n";

   } while (in.good());

   if (iState == eStateWantPassword)
   {
      cerr << "No password line found in input file\n";
      return false;
   }

   ASSERT(iState==eStateHavePassword);

   // add the decoder functions
   //AddDecode(csPassword, out);

   return true;
}

/////////////////////////////////////////////////////////////////

void AddEncByte(BYTE c, std::string &csOut)
{
  
   char buf[4];

   BYTE b1 = c >> 4;
   BYTE b2 = c & 0x0f;

   snprintf(buf, 3, "%x", b1 + basechar1);
   csOut+="\\x";
   csOut+=buf;

   snprintf(buf, 3, "%x", b2 + basechar1);
   csOut+="\\x";
   csOut+=buf;
}

/////////////////////////////////////////////////////////////////

std::string Encrypt(const std::string &csIn, const char *pPass, bool hexEncode)
{
   std::string csOut;

   // initialize out 
   CCXRIntEnc sap((const BYTE*)pPass, strlen(pPass));

   /* 
      start each string with a random char.
      because this is a stream cipher, the ciphertext of a
      string like "ABC" will be the same as the first 3 bytes
      of the ciphertext for "ABCDEFG". 

      by starting with a random value, the cipher will be in a 
      different state (255:1) when it starts the real text. the
      decoder will simply discard the first plaintext byte.
   */ 
   BYTE seed = rand() % 256;
   BYTE c = sap.ProcessByte((BYTE)(seed));
   if (hexEncode)
      AddEncByte(c, csOut);
   else
      csOut+= c;

   // encrypt and convert to hex string
   for (int i=0; i < csIn.length(); i++)
   {
      char t = csIn.at(i);
      BYTE c = sap.ProcessByte((BYTE)(t));
      if (hexEncode)
         AddEncByte(c, csOut);
      else
         csOut+= c;
   }

   return csOut;
}


/////////////////////////////////////////////////////////////////

std::string Decrypt(const char *pIn, const char *pPass)
{
   std::string csOut;

   CCXRIntDec sap((const BYTE *)pPass, strlen(pPass));

   int iLen = strlen(pIn);

   if (iLen > 2)
   {
      int iBufLen = strlen(pIn);
      if (iBufLen & 0x01)
      {
         cerr << "Illegal string length in Decrypt\n";
         return pIn;
      }

      iBufLen/=2;

      for (int i=0;i<iBufLen;i++)
      {
         int b1 = pIn[i * 2] - basechar1;
         int b2 = pIn[i * 2 + 1] - basechar2;
         int c = (b1 << 4) | b2;

         BYTE bc = sap.ProcessByte((BYTE)(c));

         if (i>0) csOut+=(char)bc;
      }
   }

   return csOut;
}

/////////////////////////////////////////////////////////////////

void WriteDecoder(const std::string & csPassword)
{
	std::ofstream fileOut;
	fileOut.open("cxr_decoder.cpp", std::ofstream::out | std::ofstream::trunc);      
	if (fileOut.good())
	{
		AddDecode(csPassword, fileOut);
	}
}

/*bool AddDecode(const std::string & csPassword, std::ostream &out)
{
   out << "\n\n/////////////////////////////////////////////////////////////\n";
   out << "// CXR-generated decoder follows\n\n";
   out << "#include <iostream>\n";
   out << "#include <algorithm>\n";
   out << "#include <string>\n";
   out << "#include <stdexcept>\n";
   out << "const unsigned char __pCXRPassword[] = {";  

   // encode the password as an unsigned char array using "0xNN" hex notation
   out << HexEncodeString(csPassword.c_str());

   out << "};" << std::endl;
   out << "const int __iCXRDecBase1 = " << (int)basechar1 << ";\nconst int __iCXRDecBase2 = " << basechar2 << ";\n\n";
                            
   // the high-level decoding function
const char *pDec1 = 
"std::string __CXRDecrypt(const char *pIn)\n"\
"{\n"\
"   std::string x;char b[3];b[2]=0;\n"\
"   CXRD sap(__pCXRPassword, sizeof(__pCXRPassword));\n"\
"   int iLen = strlen(pIn);\n"\
"   if (iLen > 2)\n"\
"   {\n"\
"      int ibl=strlen(pIn);\n"\
"      if (ibl&0x01)\n"\
"      {\n"\
"         throw new std::runtime_error(\"\");\n"\
"         return pIn;\n"\
"      }\n"\
"      ibl/=2;\n"\
"      for (int i=0;i<ibl;i++)\n"\
"      {\n"\
"         int b1 =pIn[i*2]-__iCXRDecBase1;int b2=pIn[i*2+1]-__iCXRDecBase2;\n"\
"         int c = (b1 << 4) | b2; char ch =(char)(sap.pb((BYTE)(c)));\n"\
"         if (i>0) x+=ch;\n"\
"      }\n"\
"   }\n"\
"   return x;\n"\
"}\n";
 
   // the stream cipher
   const char *pStr1 =
"class CCXR\n" \
"{\n" \
"protected:\n" \
"   CCXR(const BYTE *key, unsigned int ks)\n" \
"   {\n" \
"      int i;BYTE rs;unsigned kp;\n" \
"      for(i=0;i<256;i++)c[i]=i;kp=0;rs=0;for(i=255;i;i--)std::swap(c[i],c[kr(i,key,ks,&rs,&kp)]);r2=c[1];r1=c[3];av=c[5];lp=c[7];lc=c[rs];rs=0;kp=0;\n" \
"   }\n" \
"  inline void SC(){BYTE st=c[lc];r1+=c[r2++];c[lc]=c[r1];c[r1]=c[lp];c[lp]=c[r2];c[r2]=st;av+=c[st];}\n" \
"  BYTE c[256],r2,r1,av,lp,lc;    \n" \
"\n" \
"   BYTE kr(unsigned int lm, const BYTE *uk, BYTE ks, BYTE *rs, unsigned *kp)\n" \
"   {\n" \
"      unsigned rl=0,mk=1,u;while(mk<lm)mk=(mk<<1)+1;do{*rs=c[*rs]+uk[(*kp)++];if(*kp>=ks){*kp=0;*rs+=ks;}u=mk&*rs;if(++rl>11)u%=lm;}while(u>lm);return u;\n" \
"   }\n" \
"};\n" \
"struct CXRD:CCXR\n" \
"{\n" \
"  CXRD(const BYTE *userKey, unsigned int keyLength=16) : CCXR(userKey, keyLength) {}\n" \
"  inline BYTE pb(BYTE b){SC();lp=b^c[(c[r1]+c[r2])&0xFF]^c[c[(c[lp]+c[lc]+c[av])&0xFF]];lc=b;return lp;}\n" \
"};\n";

   out << pStr1;
   out << pDec1;

   return true;
}*/
bool AddDecode(const std::string & csPassword, std::ostream &fout)
{
	std::ostringstream out;

	out << "/////////////////////////////////////////////////////////////\n";
	out << "// CXR-generated decoder follows\n\n";
	out << "#include <iostream>\n";
	out << "#include <algorithm>\n";
	out << "#include <string>\n";
	out << "#include <stdexcept>\n";

	// the stream cipher
	out << 
		"#define BYTE unsigned char \n"\
		"\n"\
		"class CCXR\n" \
		"{\n" \
		"protected:\n" \
		"   CCXR(const BYTE *key, unsigned int ks)\n" \
		"   {\n" \
		"      int i;BYTE rs;unsigned kp;\n" \
		"      for(i=0;i<256;i++)c[i]=i;kp=0;rs=0;for(i=255;i;i--)std::swap(c[i],c[kr(i,key,ks,&rs,&kp)]);r2=c[1];r1=c[3];av=c[5];lp=c[7];lc=c[rs];rs=0;kp=0;\n" \
		"   }\n" \
		"  inline void SC(){BYTE st=c[lc];r1+=c[r2++];c[lc]=c[r1];c[r1]=c[lp];c[lp]=c[r2];c[r2]=st;av+=c[st];}\n" \
		"  BYTE c[256],r2,r1,av,lp,lc;    \n" \
		"\n" \
		"   BYTE kr(unsigned int lm, const BYTE *uk, BYTE ks, BYTE *rs, unsigned *kp)\n" \
		"   {\n" \
		"      unsigned rl=0,mk=1,u;while(mk<lm)mk=(mk<<1)+1;do{*rs=c[*rs]+uk[(*kp)++];if(*kp>=ks){*kp=0;*rs+=ks;}u=mk&*rs;if(++rl>11)u%=lm;}while(u>lm);return u;\n" \
		"   }\n" \
		"};\n" \
		"struct CXRD:CCXR\n" \
		"{\n" \
		"  CXRD(const BYTE *userKey, unsigned int keyLength=16) : CCXR(userKey, keyLength) {}\n" \
		"  inline BYTE pb(BYTE b){SC();lp=b^c[(c[r1]+c[r2])&0xFF]^c[c[(c[lp]+c[lc]+c[av])&0xFF]];lc=b;return lp;}\n" \
		"};\n";

	// the high-level decoding function
	out << 
		"std::string __CXRDecrypt(const char *pIn)\n"\
		"{\n"\
		"   std::string x;char b[3];b[2]=0;\n"\
		"   const unsigned char __pCXRPassword[] = {" << HexEncodeString(csPassword.c_str()) << "};\n";
	out << 	
		"   CXRD sap(__pCXRPassword, sizeof(__pCXRPassword));\n"\
		"   int iLen = strlen(pIn);\n"\
		"   if (iLen > 2)\n"\
		"   {\n";
	out <<
		"      const int __iCXRDecBase1=" << (int)basechar1 << "; const int __iCXRDecBase2=" << basechar2 <<";\n\n";
	out <<
		"      int ibl=strlen(pIn);\n"\
		"      if (ibl&0x01)\n"\
		"      {\n"\
		"         throw new std::runtime_error(\"\");\n"\
		"         return pIn;\n"\
		"      }\n"\
		"      ibl/=2;\n"\
		"      for (int i=0;i<ibl;i++)\n"\
		"      {\n"\
		"         int b1 =pIn[i*2]-__iCXRDecBase1;int b2=pIn[i*2+1]-__iCXRDecBase2;\n"\
		"         int c = (b1 << 4) | b2; char ch =(char)(sap.pb((BYTE)(c)));\n"\
		"         if (i>0) x+=ch;\n"\
		"      }\n"\
		"   }\n"\
		"   return x;\n"\
		"}\n";

	fout << out.str().c_str();
	return true;
}

/////////////////////////////////////////////////////////////////

std::string TranslateString(const std::string &csIn)
{
   // translate C-style string escapes as documented in K&R 2nd, A2.5.2

   std::string csOut;

   for (int i=0;i<csIn.length(); i++)
   {
      int c = csIn.at(i);
      switch (c)
      {
      default:
         // normal text
         csOut+=c;
         break;
         // c-style escape
      case '\\':
         if (i < csIn.length() - 1)
         {
            c = csIn.at(i + 1);
            switch (c)
            {
            case 'n':
               csOut+='\n';
               break;
            case 't':
               csOut+='\t';
               break;
            case 'v':
               csOut+='\v';
               break;
            case 'b':
               csOut+='\b';
               break;
            case 'r':
               csOut+='\r';
               break;
            case 'f':
               csOut+='\f';
               break;
            case 'a':
               csOut+='\a';
               break;
            case '\\':
               csOut+='\\';
               break;
            case '?':
               csOut+='?';
               break;
            case '\'':
               csOut+='\'';
               break;
            case '\"':
               csOut+='\"';
               break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
               {
                  // expand octal
                  int iConsumed = 0;
                  if (!ExpandOctal(csIn.substr(i), csOut, iConsumed))
                  {
                     cerr << "Invalid octal sequence: " << '\"' << csIn << '\"' << endl;
                     csOut = csIn;
                     break;
                  }

                  i+=iConsumed - 1;
               }
               break;
            case 'x':
               { 
                  // expand hex
                  int iConsumed = 0;
                  if (!ExpandHex(csIn.substr(i), csOut, iConsumed))
                  {
                     cerr << "Invalid hex sequence: " << '\"' << csIn << '\"' << endl;
                     csOut = csIn;
                     break;
                  }

                  i+=iConsumed - 1;

               }
               break;
            }

            i++;
            continue;
         }
         else
         {
            cerr << "Invalid escape sequence: " << '\"' << csIn << '\"' << endl;
            csOut = csIn;
            break;
         }
         break;
      }
   }

   return csOut;
}

/////////////////////////////////////////////////////////////////

bool ExpandOctal(const std::string &csIn, std::string &csOut, int &iConsumed)
{
   // staring with the escape, we need at least one more char
   if (csIn.length() < 2)
   {
      return false;
   }

   if (csIn.at(0) != '\\')
   {
      return false;
   }

   int iStart = 1;
   int iCur = iStart;

   std::string digits;
   int c = csIn.at(iCur);
   while ((c >= '0') && (c <= '7'))
   {
      digits+=c;

      // an escape can't hold more that 3 octal digits (K&R 2nd A2.5.2)
      if (iCur == 3)
      {
         break;
      }
         
      iCur++;
      c = csIn.at(iCur);
   }

   char *end;
   int octval = (char)strtol(digits.c_str(), &end, 8);

   iConsumed = digits.length();

   csOut+=octval;

   return true;
}

/////////////////////////////////////////////////////////////////

bool ExpandHex(const std::string &csIn, std::string &csOut, int &iConsumed)
{
   // staring with the escape and the 'x', we need at least one more char
   if (csIn.length() < 3)
   {
      return false;
   }

   if ((csIn.at(0) != '\\') || (csIn.at(1) != 'x'))
   {
      return false;
   }

   int iStart = 2;
   int iCur = iStart;

   std::string digits;
   int c = csIn.at(iCur);
   while (isdigit(c))
   {
      digits+=c;

      iCur++;
      c = csIn.at(iCur);
   }

   char *end;

   // "There is no limit on the number of digits, but the behavior is undefined
   // if the resulting character value exceeds that of the largest character"
   // (K&R 2nd A2.5.2)
   int hex = (char)strtol(digits.c_str(), &end, 16);

   iConsumed = digits.length();

   iConsumed++; // count the "x"

   csOut+=hex;

   return true;
}

/////////////////////////////////////////////////////////////////

std::string HexEncodeString(const char *pIn)
{
   std::string csOut;
   char buf[5];

   int iLen = strlen(pIn);

   for (int i=0;i<iLen;i++)
   {
      snprintf(buf, sizeof(buf), "0x%02x", (unsigned char)pIn[i]);      
      csOut+= buf;
      csOut+= ",";
   }

   csOut.erase(csOut.end()-1);

   return csOut;
}

/////////////////////////////////////////////////////////////////
