//
// The MIT License (MIT)
//
// Copyright (c) 2016 by ChriD
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef RAUMSERVERINSTALLER_CONVERTER_H
#define RAUMSERVERINSTALLER_CONVERTER_H

#include <string>
#include <string>
#include <vector>
#include <locale>
#include <functional>
#include <iostream>


#ifdef _WIN32
    #include <codecvt>
//#elif 
//    #include <locale>
#endif
 //   */


// Put this class in your personal toolbox...
template<class E,
class T = std::char_traits<E>,
class A = std::allocator<E> >

class Widen : public std::unary_function<
    const std::string&, std::basic_string<E, T, A> >
{
    std::locale loc_;
    const std::ctype<E>* pCType_;

    // No copy-constructor, no assignment operator...
    Widen(const Widen&);
    Widen& operator= (const Widen&);

public:
    // Constructor...
    Widen(const std::locale& loc = std::locale()) : loc_(loc)
    {
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6.0...
        using namespace std;
        pCType_ = &_USE(loc, ctype<E>);
#else
        pCType_ = &std::use_facet<std::ctype<E> >(loc);
#endif
    }

    // Conversion...
    std::basic_string<E, T, A> operator() (const std::string& str) const
    {
        typename std::basic_string<E, T, A>::size_type srcLen =
            str.length();
        const char* pSrcBeg = str.c_str();
        std::vector<E> tmp(srcLen);

        pCType_->widen(pSrcBeg, pSrcBeg + srcLen, &tmp[0]);
        return std::basic_string<E, T, A>(&tmp[0], srcLen);
    }
};


// http://www.codeproject.com/Articles/17573/Convert-Between-std-string-and-std-wstring-UTF-a

// https://github.com/JuliaLang/utf8proc
//https://github.com/udp/json-parser/issues/13

//http://stackoverflow.com/questions/3019977/convert-wchar-t-to-char
//https://bytes.com/topic/c/answers/688683-how-convert-wchar_t-char

// http://forums.codeguru.com/showthread.php?336106-WCHAR*-to-CHAR*


class Converter
{
    public:
        Converter();
        ~Converter();

        
        static std::wstring string2wstring(const std::string& _str)
        {
            Widen<wchar_t> to_wstring;
            return to_wstring(_str);
            //return utf8_decode(_str);
        }


        static std::string wstring2string(const std::wstring& _wstr)
        {
            //return ""; // TOO:@@@
            //return utf8_encode(_wstr);
            std::string dest;
            dest.resize(_wstr.length());
            std::transform(_wstr.begin(), _wstr.end(), dest.begin(), wide_to_narrow);
            return dest;
        }

        static CHAR wide_to_narrow(WCHAR w)
        {
            // simple typecast
            // works because UNICODE incorporates ASCII into itself
            return CHAR(w);
        }
       
        /*

        // Convert a wide Unicode string to an UTF8 string
        static std::string utf8_encode(const std::wstring &wstr)
        {
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
            std::string strTo(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
            return strTo;
        }

        // Convert an UTF8 string to a wide Unicode String
        static std::wstring utf8_decode(const std::string &str)
        {
            int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
            return wstrTo;
        }

        // Convert an wide Unicode string to ANSI string
        static std::string unicode2ansi(const std::wstring &wstr)
        {
            int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
            std::string strTo(size_needed, 0);
            WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
            return strTo;
        }

        // Convert an ANSI string to a wide Unicode String
        static std::wstring ansi2unicode(const std::string &str)
        {
            int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
            return wstrTo;
        }
        */

        //http://www.cplusplus.com/reference/cstdlib/wcstombs/

        /*
        static std::wstring string2wstring(const std::string& str)
        {
            #ifdef _WIN32
                typedef std::codecvt_utf8<wchar_t> convert_typeX;
                std::wstring_convert<convert_typeX, wchar_t> converterX;
                return converterX.from_bytes(str);                
            #elif     
                // not noce but...
                std::wstring wsTmp(s.begin(), s.end());
                return wsTmp;
            #endif                   
        }


        static std::string wstring2string(const std::wstring& wstr)
        {            
            #ifdef _WIN32
                typedef std::codecvt_utf8<wchar_t> convert_typeX;
                std::wstring_convert<convert_typeX, wchar_t> converterX;
                return converterX.to_bytes(wstr);               
            #elif
                return "" // TODO: @@@
            #endif   
            
        }  
        */

};



#endif