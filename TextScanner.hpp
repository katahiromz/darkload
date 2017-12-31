// TextScanner.hpp --- CodeReverse TextScanner
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#ifndef CODEREVERSE_TEXT_SCANNER_HPP
#define CODEREVERSE_TEXT_SCANNER_HPP

#include "Common.hpp"

/////////////////////////////////////////////////////////////////////////

namespace CodeReverse
{
    /////////////////////////////////////////////////////////////////////////
    // character classification

    inline bool is_digit(char_type ch)
    {
        return '0' <= ch && ch <= '9';
    }
    inline bool is_xdigit(char_type ch)
    {
        return is_digit(ch) || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F');
    }
    inline bool is_octal(char_type ch)
    {
        return '0' <= ch && ch <= '7';
    }
    inline bool is_lower(char_type ch)
    {
        return 'a' <= ch && ch <= 'z';
    }
    inline bool is_upper(char_type ch)
    {
        return 'A' <= ch && ch <= 'Z';
    }
    inline bool is_alpha(char_type ch)
    {
        return is_lower(ch) || is_upper(ch);
    }
    inline bool is_alnum(char_type ch)
    {
        return is_alpha(ch) || is_digit(ch);
    }
    inline bool is_space(char_type ch)
    {
        return strchr(" \t\n\r\f\v", ch) && ch;
    }

    /////////////////////////////////////////////////////////////////////////
    // TextScanner

    class TextScanner
    {
    public:
        TextScanner(const string_type& str);

        char_type getch();
        void nextch();
        void ungetch();
        const char_type *peek() const;
        char_type peekch() const;
        void skip(size_t count);

        bool match_peek(const char_type *psz) const;
        bool match_peek(const char_type *psz, string_type& str) const;

        bool match_get(const char_type *psz);
        bool match_get(const char_type *psz, string_type& str);

        size_t index() const;
        const string_type file() const;
        size_t line() const;
        size_t column() const;
        Position pos() const;
        void pos(const Position& pos_);

    protected:
        string_type     m_str;
        size_t          m_index;
        Position        m_pos;

        Position index2pos(size_t index) const;
    };

    /////////////////////////////////////////////////////////////////////////
    // TextScanner inlines

    inline TextScanner::TextScanner(const string_type& str)
        : m_str(str), m_index(0)
    {
    }
    inline char_type TextScanner::getch()
    {
        if (m_index < m_str.size())
        {
            if (m_str[m_index] == '\n')
                m_pos.newline();
            else
                ++m_pos;
            return m_str[m_index++];
        }
        return 0;
    }
    inline void TextScanner::nextch()
    {
        if (m_index < m_str.size())
        {
            if (m_str[m_index] == '\n')
                m_pos.newline();
            else
                ++m_pos;
            ++m_index;
        }
    }
    inline void TextScanner::ungetch()
    {
        if (m_index > 0)
        {
            --m_index;
            if (m_str[m_index] == '\n')
            {
                size_t i, column = 0;
                for (i = m_index; i > 0;)
                {
                    --i;
                    if (m_str[i] == '\n')
                        break;
                    ++column;
                }
                m_pos.m_column = column;
                assert(m_pos.m_line > 0);
                --(m_pos.m_line);
            }
            else
            {
                assert(m_pos.m_column > 0);
                --(m_pos.m_column);
            }
        }
    }
    inline const char_type *TextScanner::peek() const
    {
        return &m_str.c_str()[m_index];
    }
    inline char_type TextScanner::peekch() const
    {
        return *peek();
    }
    inline bool TextScanner::match_peek(const char_type *psz) const
    {
        const size_t len = strlen(psz);
        if (memcmp(peek(), psz, len) == 0)
        {
            assert(string_type(psz).find('\n') == string_type::npos);
            return true;
        }
        return false;
    }
    inline bool TextScanner::match_peek(const char_type *psz, string_type& str) const
    {
        const size_t len = strlen(psz);
        if (memcmp(peek(), psz, len) == 0)
        {
            str = psz;
            assert(str.find('\n') == string_type::npos);
            return true;
        }
        return false;
    }
    inline bool TextScanner::match_get(const char_type *psz)
    {
        const size_t len = strlen(psz);
        if (memcmp(peek(), psz, len) == 0)
        {
            assert(string_type(psz).find('\n') == string_type::npos);
            skip(len);
            return true;
        }
        return false;
    }
    inline bool TextScanner::match_get(const char_type *psz, string_type& str)
    {
        const size_t len = strlen(psz);
        if (memcmp(peek(), psz, len) == 0)
        {
            str = psz;
            assert(str.find('\n') == string_type::npos);
            skip(len);
            return true;
        }
        return false;
    }
    inline void TextScanner::skip(size_t count)
    {
        assert(m_str.substr(m_index, count).find('\n') == string_type::npos);
        if (m_index + count <= m_str.size())
        {
            m_index += count;
            m_pos += count;
        }
        else
        {
            m_index = m_str.size();
            m_pos = index2pos(m_index);
        }
    }
    inline size_t TextScanner::index() const
    {
        return m_index;
    }
    inline const string_type TextScanner::file() const
    {
        return m_pos.file();
    }
    inline size_t TextScanner::line() const
    {
        return m_pos.line();
    }
    inline size_t TextScanner::column() const
    {
        return m_pos.column();
    }
    inline Position TextScanner::pos() const
    {
        return m_pos;
    }
    inline void TextScanner::pos(const Position& pos_)
    {
        m_pos = pos_;
    }
    inline Position TextScanner::index2pos(size_t index) const
    {
        Position pos;
        for (size_t i = 0; i < index; ++i)
        {
            if (m_str[i] == '\n')
                pos.newline();
            else
                ++pos;
        }
        return pos;
    }
} // namespace CodeReverse

/////////////////////////////////////////////////////////////////////////

#endif  // ndef CODEREVERSE_TEXT_SCANNER_HPP
