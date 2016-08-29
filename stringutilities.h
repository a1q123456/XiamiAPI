#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <string>
#include <vector>
#include <cstring>
#include <typeinfo>

namespace xiamiapi
{
    namespace StringUtilities
    {
        void split(const std::string &text, char sep, std::vector<std::string> & result);
        std::string trim(const std::string & text);
        std::string trim(const char * text);
        template <class T>
        typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, std::string>::type
        to_string(T val)
        {
            return std::to_string(val);
        }

        template <class T>
        typename std::enable_if<std::is_same<T, std::string>::value, std::string>::type
        to_string(T val)
        {
            return val;
        }

        template <class T>
        typename std::enable_if<!std::is_same<T, std::string>::value && !std::is_integral<T>::value || std::is_floating_point<T>::value, std::string>::type
        to_string(T val)
        {
            return val.to_string();
        }

        template <class T>
        typename std::enable_if<std::is_same<char, T>::value, std::string>::type
        to_string(const T * val)
        {
            return std::string(val);
        }

        struct string_format_pair
        {
            template <class Tv>
            string_format_pair(const std::string & key, const Tv & value) :
                key(key),
                value(to_string(value)) {}

            std::string key;
            std::string value;
        };

        std::string string_format(const std::string & fmt, const std::initializer_list<string_format_pair> & args);
        std::string url_escape(const std::string & str, const bool & form_encode = true);
        std::string url_unescape(const std::string & str);
    };
}
#endif // STRINGUTILITIES_H
