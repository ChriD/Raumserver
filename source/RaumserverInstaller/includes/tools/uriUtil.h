//
// The MIT License (MIT)
//
// Copyright (c) 2015 by ChriD
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
#ifndef RAUMKERNEL_URIUTIL_H
#define RAUMKERNEL_URIUTIL_H

#include <stdio.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <raumkernel/tools/stringUtil.h>
#include <raumkernel/tools/urlParser.h>

namespace Raumkernel
{
    namespace Tools
    {
        class UriUtil
        {
            public:       
                // DECODE URI
                // ESCAPE URI
                // UNESCPAE URI


                /**
                * Encodes the given string to be a valid string for an uri as RFC3986
                */
                static std::string encodeUriPart(const std::string _string)
                {
                    return encodeUri(_string, false, "");
                }


                /**
                * Encodes the given value to be a valid value for an uri query as RFC3986
                */
                static std::string encodeValue(const std::string _value)
                {
                    return encodeUri(_value, false, "/");
                }


                /**
                * returns the query from the uri
                */
                static std::string getQueryFromUrl(const std::string _url)
                {
                    auto url = LUrlParser::clParseURL::ParseURL(_url);                   
                    return url.m_Query;                    
                }


                /**
                * Encodes the given uri to be a valid uri as RFC3986                
                */
                static std::string encodeUri(const std::string _uri, bool _ecodeFromQuestionMark, std::string _additionalReserved)
                {
                    std::uint32_t startEncodePos = 0;

                    if (_ecodeFromQuestionMark)
                        startEncodePos = _uri.find("?");

                    //RFC 3986 section 2.3 Unreserved Characters (January 2005)
                    const std::string unreserved = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~" + _additionalReserved;

                    std::string escaped = "";
                    for (size_t i = startEncodePos; i<_uri.length(); i++)
                    {
                        if (unreserved.find_first_of(_uri[i]) != std::string::npos)
                        {
                            escaped.push_back(_uri[i]);
                        }
                        else
                        {
                            std::uint16_t charValue = (std::uint16_t)(unsigned char)_uri[i];
                            if (charValue > 127)
                            {
                                std::ostringstream converted;
                                std::uint8_t level = (std::uint8_t)floor((charValue - 128) / 64);
                                converted << '%' << std::hex << std::uppercase << 194 + level;
                                converted << '%' << std::hex << std::uppercase << (charValue)-64;
                                escaped.append(converted.str());
                            }
                            else
                            {
                                std::ostringstream converted;
                                converted << '%' << std::hex << std::uppercase << (std::int32_t)(unsigned char)_uri[i];
                                escaped.append(converted.str());
                            }
                        }
                    }
                    return escaped;
                }


                /**
                * Returns a key/value map for the given query string
                */
                static std::unordered_map<std::string, std::string> parseQueryString(std::string _queryString, bool _unescape = true, bool _tolowerKeyValues = true)
                {
                    std::unordered_map<std::string, std::string> queryValuesMap;
                    std::vector<std::string> idValueParts;

                    idValueParts = StringUtil::explodeString(_queryString, "&");
                    for (auto &idValuePart : idValueParts)
                    {
                        std::vector<std::string> valueAndPart;
                        valueAndPart = StringUtil::explodeString(idValuePart, "=");
                        if (valueAndPart.size() < 2)
                        {
                            if (_unescape)
                                valueAndPart[0] = unescape(valueAndPart[0]);
                            if (_tolowerKeyValues)
                                std::transform(valueAndPart[0].begin(), valueAndPart[0].end(), valueAndPart[0].begin(), ::tolower);                                
                            // we may have only an identifier with no value
                            // example: '?sid=AAA&a&b&c'
                            queryValuesMap.insert(std::make_pair(valueAndPart[0], ""));
                        }
                        else
                        {
                            // example: '?sid=AAA&a=1&b=2&c=3'
                            if (_unescape)
                            {
                                valueAndPart[0] = unescape(valueAndPart[0]);
                                valueAndPart[1] = unescape(valueAndPart[1]);
                            }
                            if (_tolowerKeyValues)
                            {
                                std::transform(valueAndPart[0].begin(), valueAndPart[0].end(), valueAndPart[0].begin(), ::tolower);                                
                            }
                            queryValuesMap.insert(std::make_pair(valueAndPart[0], valueAndPart[1]));
                        }
                    }

                    return queryValuesMap;
                }


                /**
                * unescapes the given string
                */
                static std::string unescape(const std::string& _string)
                {
                    // avoid copying if there's nothing to unescape
                    if (_string.empty() || _string.find('%') == std::string::npos)
                        return _string;
                    std::string result;
                    result.reserve(_string.size());
                    for (std::string::const_iterator i = _string.begin(); i != _string.end(); ++i) {
                        if (*i == '%' && (i + 1) != _string.end() && (i + 2) != _string.end()
                            && isxdigit(*(i + 1)) && isxdigit(*(i + 2)) //
                            && ((*(i + 1)) != 0 || (*(i + 2)) != 0)) { // can't unescape null

                            char c = x2digits_to_num(*(i + 1), *(i + 2));
                            result += c;
                            i += 2; // increment the iterator
                        }
                        else
                            result += (*i);
                    }
                    result.reserve(); // shrink to fit
                    return result;
                }


                /**
                * only for internal use
                */
                static char x2digits_to_num(char c1, char c2) 
                {
                    if (!isxdigit(c1) || !isxdigit(c2))
                        throw std::runtime_error("not an hex digit");
                    return((xdigit_to_num(c1) << 4) + xdigit_to_num(c2));
                }


                /**
                * only for internal use
                */
                static char xdigit_to_num(char c) 
                {
                    if (!isxdigit(c))
                        throw std::runtime_error("not an hex digit");
                    return(c < 'A' ? c - '0' : toupper(c) - 'A' + 10);
                }
              


                /**
                * create a bendable or setable avTransportUri for a container
                */
                EXPORT static std::string createAVTransportUriForContainer(const std::string &_mediaServerUDN, const std::string &_containerId, const std::int32_t &_trackIndex = -1)
                {
                    auto uri = Tools::UriUtil::encodeUriPart(_mediaServerUDN) + "?sid=" + Tools::UriUtil::encodeUriPart("urn:upnp-org:serviceId:ContentDirectory") + "&cid=" + Tools::UriUtil::encodeUriPart(_containerId) + "&md=0";

                    if (_trackIndex >= 0)
                        uri += "&fii=" + Tools::UriUtil::encodeUriPart(std::to_string(_trackIndex));

                    uri = "dlna-playcontainer://" + uri;

                    // a valid transport uri looks like this!
                    //dlna-playcontainer://uuid%3Aed3bd3db-17b1-4dbe-82df-5201c78e632c?sid=urn%3Aupnp-org%3AserviceId%3AContentDirectory&cid=0%2FPlaylists%2FMyPlaylists%2FTest&md=0	
                    return uri;
                }

                /**
                * create a bendable or setable avTransportUri for a single item
                */
                EXPORT static std::string createAVTransportUriForSingle(const std::string &_mediaServerUDN, const std::string &_singleId)
                {
                    auto uri = Tools::UriUtil::encodeUriPart(_mediaServerUDN) + "?sid=" + Tools::UriUtil::encodeUriPart("urn:upnp-org:serviceId:ContentDirectory") + "&iid=" + Tools::UriUtil::encodeUriPart(_singleId);

                    uri = "dlna-playsingle://" + uri;

                    // a valid transport uri looks like this!
                    //dlna-playsingle://uuid%3Aed3bd3db-17b1-4dbe-82df-5201c78e632c?sid=urn%3Aupnp-org%3AserviceId%3AContentDirectory&iid=0%2FRadioTime%2FLocalRadio%2Fs-s68932
                    return uri;
                }

            
        };

    }
}


#endif