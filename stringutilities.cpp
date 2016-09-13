#include "stringutilities.h"
#include <algorithm>
#include <type_traits>
#include <regex>
#include <iomanip>

namespace xiamiapi
{
    namespace StringUtilities
    {


        void split(const std::string &text, char sep, std::vector<std::string> & result) {
          std::size_t start = 0, end = 0;
          while ((end = text.find(sep, start)) != std::string::npos) {
            result.push_back(text.substr(start, end - start));
            start = end + 1;
          }
          result.push_back(text.substr(start));
        }

        std::string trim(const std::string & text)
        {
            auto first_non_space_pos = text.find_first_not_of(' ');
            auto last_non_space_pos = text.find_last_not_of(' ');
            return text.substr(first_non_space_pos, last_non_space_pos - first_non_space_pos);
        }

        std::string trim(const char * text)
        {
            int64_t trimlen = 0;
            int64_t startlen = -1;
            for(int64_t i = 0; text[i] != '\0'; i++)
            {
                if (text[i] != ' ')
                {
                    if (startlen == -1)
                    {
                        startlen = i;
                    }
                    trimlen++;
                }
            }
            return std::string(text + startlen, static_cast<std::size_t>(trimlen));
        }

        std::string string_format(const std::string & fmt, const std::initializer_list<string_format_pair> & args)
        {
            std::string ret(fmt);
            for (auto pair : args)
            {
                std::string r_str = "(?!%)\\{" + pair.key + "\\}";
                std::regex r(r_str, std::regex_constants::ECMAScript);
                ret = std::regex_replace(ret, r, pair.value);

            }
            return ret;
        }

        std::string url_escape(const std::string & str, const bool & form_encode)
        {
            std::ostringstream escaped;
            escaped.fill('0');
            escaped << std::hex;

            for (auto c : str)
            {

                // Keep alphanumeric and other accepted characters intact
                if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                    escaped << c;
                    continue;
                }

                else if (c == ' ' && form_encode)
                {
                    escaped << '+';
                    continue;
                }

                // Any other characters are percent-encoded
                escaped << std::uppercase;
                escaped << '%' << std::setw(2) << int((unsigned char) c);
                escaped << std::nouppercase;
            }

            return escaped.str();

        }

        std::string url_unescape(const std::string & str)
        {
            std::string url_decoded(str);
            char a, b;
            auto str_i = str.begin();
            auto url_decoded_i = url_decoded.begin();
            std::size_t reserve_len = 0;
            for (; str_i != str.end();)
            {
                if ((str_i[0] == '%') && ((a = str_i[1]) && (b = str_i[2])) && (isxdigit(a) && isxdigit(b)))
                {
                    if (a >= 'a') a -= 'a'-'A';
                    if (a >= 'A') a -= ('A' - 10);
                    else a -= '0';
                    if (b >= 'a') b -= 'a'-'A';
                    if (b >= 'A') b -= ('A' - 10);
                    else b -= '0';
                    *url_decoded_i++ = 16*a+b;
                    if (*url_decoded_i == '^')
                    {
                        *url_decoded_i = '0';
                    }
                    str_i += 3;
                }
                else if (*str_i == '+')
                {
                    *url_decoded_i++ = ' ';
                    str_i++;
                }
                else
                {
                    *url_decoded_i++ = *str_i++;

                }
                reserve_len++;
            }
            url_decoded.resize(reserve_len);
            return url_decoded;
        }
    }
}
