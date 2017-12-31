// Common.hpp --- CodeReverse common header
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#ifndef CODEREVERSE_COMMON_HPP
#define CODEREVERSE_COMMON_HPP

#define _CRT_SECURE_NO_WARNINGS

/////////////////////////////////////////////////////////////////////////

#include <string>       // for std::string
#include <vector>       // for std::vector
#include <sstream>      // for std::stringstream
#include <cstring>      // for std::strlen, std::strtol, ...
#include <cassert>      // for assert macro
#include <cstdarg>      // for va_list

#ifndef _countof
    #define _countof(array)  sizeof(array) / sizeof(array[0])
#endif

/////////////////////////////////////////////////////////////////////////

namespace CodeReverse
{
    /////////////////////////////////////////////////////////////////////////
    // basic types

    typedef std::string                 string_type;
    typedef string_type::value_type     char_type;
    typedef std::stringstream           os_type;

    /////////////////////////////////////////////////////////////////////////
    // Position

    struct Position
    {
        string_type m_file;
        size_t m_line;
        size_t m_column;

        Position()
            : m_file("(anonymous)"), m_line(0), m_column(0)
        {
        }
        Position(const char_type *fname)
            : m_file(fname), m_line(0), m_column(0)
        {
        }
        Position(const char_type *fname, int line)
            : m_file(fname), m_line(line - 1), m_column(0)
        {
        }
        Position(const char_type *fname, int line, int column)
            : m_file(fname), m_line(line - 1), m_column(column - 1)
        {
        }
        void assign(const char_type *fname, int line, int column)
        {
            m_file = fname;
            m_line = line;
            m_column = column;
        }
        Position& operator+=(size_t skip)
        {
            m_column += skip;
            return *this;
        }
        Position& operator++()
        {
            ++m_column;
            return *this;
        }
        Position operator++(int)
        {
            Position pos(*this);
            ++m_column;
            return pos;
        }
        void newline()
        {
            m_column = 0;
            ++m_line;
        }
        const string_type& file() const
        {
            return m_file;
        }
        size_t line() const
        {
            return m_line;
        }
        size_t column() const
        {
            return m_column;
        }
    };

    template <class CharT, class Traits>
    inline std::basic_ostream<CharT,Traits>&
    operator<<(std::basic_ostream<CharT,Traits>& os, const Position& pos)
    {
        os << pos.file() << ':' << int(pos.line() + 1) << ':' << int(pos.column() + 1);
        return os;
    }

    /////////////////////////////////////////////////////////////////////////
    // AuxItem and AuxInfo

    struct AuxItem
    {
        std::string m_text;
        Position    m_pos;
    };

    struct AuxInfo
    {
        std::vector<AuxItem>    m_errors;
        std::vector<AuxItem>    m_warnings;

        void add_error(const Position& pos, const char *fmt, ...)
        {
            va_list va;
            char buf[512];
            va_start(va, fmt);
            std::vsprintf(buf, fmt, va);
            va_end(va);

            AuxItem item;
            item.m_pos = pos;
            item.m_text = buf;
            m_errors.push_back(item);
        }
        void add_warning(const Position& pos, const char *fmt, ...)
        {
            va_list va;
            char buf[512];
            va_start(va, fmt);
            std::vsprintf(buf, fmt, va);
            va_end(va);

            AuxItem item;
            item.m_pos = pos;
            item.m_text = buf;
            m_warnings.push_back(item);
        }
        void clear()
        {
            m_errors.clear();
            m_warnings.clear();
        }

        void err_out(os_type& os) const;
    };

    /////////////////////////////////////////////////////////////////////////
    // AuxInfo inlines

    inline void AuxInfo::err_out(os_type& os) const
    {
        for (size_t i = 0; i < m_errors.size(); ++i)
        {
            const AuxItem& item = m_errors[i];
            os << item.m_pos << ": error: " << item.m_text << std::endl;
        }
        for (size_t i = 0; i < m_warnings.size(); ++i)
        {
            const AuxItem& item = m_warnings[i];
            os << item.m_pos << ": warning: " << item.m_text << std::endl;
        }
    }
} // namespace CodeReverse

/////////////////////////////////////////////////////////////////////////

#endif  // ndef CODEREVERSE_COMMON_HPP
