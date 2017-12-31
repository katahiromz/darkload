// Lexer.hpp --- CodeReverse lexical analysis
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#ifndef CODEREVERSE_LEXER_HPP
#define CODEREVERSE_LEXER_HPP

#include "TextScanner.hpp"
#include <set>     // for std::set
#include <map>     // for std::multimap
#include <stack>   // for std::stack
#ifndef NDEBUG
    #include <iostream>
#endif

/////////////////////////////////////////////////////////////////////////

namespace CodeReverse
{
    /////////////////////////////////////////////////////////////////////////
    // TokenType and Token

    enum TokenType
    {
        TK_EOF = 0,
        TK_SYMBOL,
        TK_KEYWORD,
        TK_IDENTIFIER,
        TK_CHARACTER_LITERAL,
        TK_INTEGER_LITERAL,
        TK_STRING_LITERAL,
        TK_FLOATING_LITERAL,
    };

    struct Token
    {
        std::string     m_str;
        Position        m_pos;
        TokenType       m_type;
        std::string     m_fix;  // prefix or suffix

        Token(const Position& pos, TokenType type) : m_pos(pos), m_type(type)
        {
        }
    };
    typedef std::vector<Token> TokensType;

    template <class CharT, class Traits>
    inline std::basic_ostream<CharT,Traits>&
    operator<<(std::basic_ostream<CharT,Traits>& os, const Token& tok)
    {
        os << tok.m_pos << ": " << tok.m_str << " (" << tok.m_type << ")\n";
        return os;
    }

    /////////////////////////////////////////////////////////////////////////
    // Lexer

    class Lexer
    {
    public:
        Lexer(TextScanner& scanner, AuxInfo& aux);
        Lexer(TextScanner& scanner, const TokensType& tokens, AuxInfo& aux);

        bool do_lex();
        void fixup();

        bool empty() const;
        size_t size() const;
              Token& operator[](size_t i);
        const Token& operator[](size_t i) const;
        void push_back(const Token& t);
        void clear();

              Token& token();
        const Token& token() const;
        void next();
        void prev();
        bool eof() const;

        TokenType type() const;
        string_type str() const;
        string_type fix() const;

        size_t index() const;
        void index(size_t pos);
        Position pos() const;
        void pos(const Position& pos_);

        bool scan_number();
        bool scan_string_or_character();
        bool scan_identifier();
        bool scan_symbol();
        bool do_floating_exponent(Token& token);
        bool do_floating_suffix(Token& token);
        bool do_integer_suffix(Token& token);
        bool do_directive(const string_type& str);
        bool do_line(const TokensType& tokens);
        bool do_pragma(const TokensType& tokens);

        size_t paren_close(size_t i) const;
        size_t brace_close(size_t i) const;

    protected:
        TextScanner& m_text;
        AuxInfo& m_aux;
        size_t m_index;
        int m_pack;
        size_t m_pragma_begin;
        size_t m_pragma_paren;
        TokensType m_tokens;
        typedef std::multimap<char_type, string_type> symbol_map_type;
        symbol_map_type m_symbol_map;
        typedef std::set<string_type> keyword_set_type;
        keyword_set_type m_keywords;
        typedef std::stack<int> pack_stack_type;
        pack_stack_type m_pack_stack;

        friend class CParser;

        char_type getch();
        char_type peekch() const;
        void skip(size_t count);
        void ungetch();
        void nextch();
        bool match_peek(const char_type *psz) const;
        bool match_peek(const char_type *psz, string_type& str) const;
        bool match_get(const char_type *psz);
        bool match_get(const char_type *psz, string_type& str);
        void push_pack(int pack);
        void pop_pack();

        void fixup_gnu_extensions();
    };

    template <class CharT, class Traits>
    inline std::basic_ostream<CharT,Traits>&
    operator<<(std::basic_ostream<CharT,Traits>& os, const Lexer& lexer)
    {
        for (size_t i = 0; i < lexer.size(); ++i)
        {
            os << lexer[i];
        }
        return os;
    }

    /////////////////////////////////////////////////////////////////////////
    // escape/unescape

    std::string str_quote(const std::string& str, char quote);
    std::string wstr_quote(const std::wstring& str, wchar_t quote);
    std::string str_unquote(const std::string& str, char quote);
    std::wstring wstr_unquote(const std::string& str, char quote);

    void str_trim(std::string& str, const char *spaces = " \t\r\n");
    void str_trim(std::wstring& str, const wchar_t *spaces = L" \t\r\n");

    template <typename T_STR>
    bool str_replace_all(T_STR& str, const T_STR& from, const T_STR& to);
    template <typename T_STR>
    bool str_replace_all(T_STR& str,
                         const typename T_STR::value_type *from,
                         const typename T_STR::value_type *to);

    /////////////////////////////////////////////////////////////////////////

    inline const char **get_keywords(size_t& count)
    {
        static const char *s_keywords[] =
        {
            "_Alignas",
            "_Alignof",
            "_Atomic",
            "_Bool",
            "_Complex",
            "_Generic",
            "_Imaginary",
            "_Noreturn",
            "_Static_assert",
            "_Thread_local",
            "__asm",
            "__asm__",
            "__attribute__",
            "__cdecl",
            "__declspec",
            "__fastcall",
            "__float128",
            "__float80",
            "__forceinline",
            "__inline",
            "__inline__",
            "__int128",
            "__int64",
            "__pragma",
            "__ptr64",
            "__restrict__",
            "__stdcall",
            "__volatile__",
            "auto",
            "break",
            "case",
            "char",
            "const",
            "continue",
            "default",
            "do",
            "double",
            "else",
            "enum",
            "extern",
            "float",
            "for",
            "goto",
            "if",
            "inline",
            "int",
            "long",
            "register",
            "restrict",
            "return",
            "short ",
            "signed",
            "sizeof",
            "static",
            "struct",
            "switch",
            "typedef",
            "union",
            "unsigned",
            "void",
            "volatile",
            "while",
        };
        count = _countof(s_keywords);
        return s_keywords;
    }

    /////////////////////////////////////////////////////////////////////////

    inline const char **get_symbols(size_t& count)
    {
        static const char *s_symbols[] =
        {
            "!",
            "!=",
            "#",
            "##",
            "%",
            "%=",
            "&",
            "&&",
            "&=",
            "(",
            ")",
            "*",
            "*=",
            "+",
            "++",
            "+=",
            ",",
            "-",
            "--",
            "-=",
            "->",
            ".",
            "...",
            "/",
            "/=",
            ":",
            ";",
            "<",
            "<<",
            "<<=",
            "<=",
            "=",
            "==",
            ">",
            ">=",
            ">>",
            ">>=",
            "?",
            "[",
            "]",
            "^",
            "^=",
            "{",
            "|",
            "|=",
            "||",
            "}",
            "~",
        };
        count = _countof(s_symbols);
        return s_symbols;
    }

    /////////////////////////////////////////////////////////////////////////
    // Lexer inlines

    inline Lexer::Lexer(TextScanner& text, AuxInfo& aux)
        : m_text(text), m_aux(aux), m_index(0), m_pack(1),
          m_pragma_begin(-1), m_pragma_paren(0)
    {
        size_t count;
        const char **symbols = get_symbols(count);
        for (size_t i = 0; i < count; ++i)
        {
            m_symbol_map.insert(std::make_pair(*symbols[i], symbols[i]));
        }

        const char **keywords = get_keywords(count);
        for (size_t i = 0; i < count; ++i)
        {
            m_keywords.insert(keywords[i]);
        }
    }
    inline Lexer::Lexer(TextScanner& scanner, const TokensType& tokens, AuxInfo& aux)
        : m_text(scanner), m_aux(aux), m_index(0), m_pack(1),
          m_pragma_begin(-1), m_pragma_paren(0), m_tokens(tokens)
    {
    }
    inline Token& Lexer::token()
    {
        return m_tokens[m_index];
    }
    inline const Token& Lexer::token() const
    {
        return m_tokens[m_index];
    }
    inline void Lexer::next()
    {
        if (m_index + 1 <= size())
            ++m_index;
    }
    inline void Lexer::prev()
    {
        if (m_index > 0)
            --m_index;
    }
    inline bool Lexer::eof() const
    {
        return type() == TK_EOF;
    }
    inline TokenType Lexer::type() const
    {
        return token().m_type;
    }
    inline string_type Lexer::str() const
    {
        return token().m_str;
    }
    inline string_type Lexer::fix() const
    {
        return token().m_fix;
    }
    inline size_t Lexer::index() const
    {
        return m_index;
    }
    inline void Lexer::index(size_t pos)
    {
        m_index = pos;
    }
    inline Position Lexer::pos() const
    {
        return m_text.pos();
    }
    inline void Lexer::pos(const Position& pos_)
    {
        m_text.pos(pos_);
    }
    inline bool Lexer::empty() const
    {
        return size() == 0;
    }
    inline size_t Lexer::size() const
    {
        return m_tokens.size();
    }
    inline Token& Lexer::operator[](size_t i)
    {
        return m_tokens[i];
    }
    inline const Token& Lexer::operator[](size_t i) const
    {
        return m_tokens[i];
    }
    inline void Lexer::push_back(const Token& t)
    {
        m_tokens.push_back(t);
    }
    inline void Lexer::clear()
    {
        m_tokens.clear();
        m_index = 0;
    }
    inline char_type Lexer::getch()
    {
        return m_text.getch();
    }
    inline char_type Lexer::peekch() const
    {
        return m_text.peekch();
    }
    inline void Lexer::skip(size_t count)
    {
        m_text.skip(count);
    }
    inline void Lexer::ungetch()
    {
        m_text.ungetch();
    }
    inline void Lexer::nextch()
    {
        m_text.nextch();
    }
    inline bool Lexer::match_peek(const char_type *psz) const
    {
        return m_text.match_peek(psz);
    }
    inline bool Lexer::match_peek(const char_type *psz, string_type& str) const
    {
        return m_text.match_peek(psz, str);
    }
    inline bool Lexer::match_get(const char_type *psz)
    {
        return m_text.match_get(psz);
    }
    inline bool Lexer::match_get(const char_type *psz, string_type& str)
    {
        return m_text.match_get(psz, str);
    }
    inline void Lexer::push_pack(int pack)
    {
        m_pack_stack.push(pack);
        m_pack = pack;
    }
    inline void Lexer::pop_pack()
    {
        if (!m_pack_stack.empty())
        {
            m_pack = m_pack_stack.top();
            m_pack_stack.pop();
        }
    }
    inline bool Lexer::do_lex()
    {
        char_type ch;

        m_pragma_paren = 0;
        m_pragma_begin = size_t(-1);

        for (;;)
        {
            for (;;)
            {
                ch = peekch();
                if (!is_space(ch))
                    break;
                nextch();
            }

            if (match_get("/*"))    // */
            {
                do
                {
                    if (peekch() == 0)
                    {
                        m_aux.add_error(pos(), "comment not terminated");
                        return false;
                    }
                    nextch();
                } while (!match_get("*/"));
                continue;
            }

            if (match_get("//"))
            {
                nextch();
                for (;;)
                {
                    nextch();
                    ch = peekch(); 
                    if (ch == 0 || ch == '\n')
                        break;
                }
                continue;
            }

            if (ch == '#')
            {
                string_type str;
                for (;;)
                {
                    nextch();
                    ch = peekch(); 
                    if (ch == 0 || ch == '\n')
                        break;
                    str += ch;
                }
                str_trim(str);
                do_directive(str);
                continue;
            }

            if (is_digit(ch))
            {
                if (scan_number())
                    continue;

                m_aux.add_error(pos(), "invalid number");
                return false;
            }

            if (match_peek("L'") || match_peek("L\""))
            {
                Position p = pos();
                if (scan_string_or_character())
                    continue;

                m_aux.add_error(p, "invalid escape sequence");
                return false;
            }

            if (is_alpha(ch) || ch == '_')
            {
                if (scan_identifier())
                    continue;

                m_aux.add_error(pos(), "invalid identifier");
                return false;
            }

            if (ch == '"' || ch == '\'')
            {
                Position p = pos();
                if (scan_string_or_character())
                    continue;

                m_aux.add_error(p, "invalid escape sequence");
                return false;
            }

            if (ch == 0)
            {
                break;
            }

            if (scan_symbol())
                continue;

            m_aux.add_error(pos(), "invalid character");
            return false;
        }

        Token token(pos(), TK_EOF);
        push_back(token);
        return true;
    }
    inline bool Lexer::scan_number()
    {
        char_type ch;
        Token token(pos(), TK_INTEGER_LITERAL);

        if (match_get("0x", token.m_str) || match_get("0X", token.m_str))
        {
            for (;;)
            {
                ch = peekch();
                if (!is_xdigit(ch))
                    break;
                token.m_str += ch;
                nextch();
            }

            if (!do_integer_suffix(token))
            {
                return false;
            }

            push_back(token);
            return true;
        }

        for (;;)
        {
            ch = peekch();
            if (!is_digit(ch))
                break;
            token.m_str += ch;
            nextch();
        }

        ch = peekch();
        if (ch == '.')
        {
            token.m_str += ch;
            token.m_type = TK_FLOATING_LITERAL;
            nextch();

            for (;;)
            {
                ch = peekch();
                if (!is_digit(ch))
                    break;
                token.m_str += ch;
                nextch();
            }

            if (!do_floating_exponent(token) ||
                !do_floating_suffix(token))
            {
                return false;
            }

            push_back(token);
            return true;
        }

        if (ch == 'e' || ch == 'E')
        {
            if (!do_floating_exponent(token) ||
                !do_floating_suffix(token))
            {
                return false;
            }

            push_back(token);
            return true;
        }

        if (!do_integer_suffix(token))
            return false;

        if (token.m_str[0] == '0' &&
            (token.m_str.find('8') != string_type::npos ||
             token.m_str.find('9') != string_type::npos))
        {
            return false;
        }

        push_back(token);
        return true;
    }
    inline bool Lexer::do_floating_exponent(Token& token)
    {
        char_type ch = peekch();
        if (ch == 'e' || ch == 'E')
        {
            token.m_str += ch;
            nextch();

            ch = peekch();
            if (ch == '+' || ch == '-')
            {
                token.m_str += ch;
                nextch();
            }

            bool has_exp = false;
            for (;;)
            {
                ch = peekch();
                if (!is_digit(ch))
                    break;
                has_exp = true;
                token.m_str += ch;
                nextch();
            }

            return has_exp;
        }
        return true;
    }
    inline bool Lexer::do_floating_suffix(Token& token)
    {
        for (;;)
        {
            char_type ch = peekch();
            switch (ch)
            {
            case 'l': case 'f': case 'L': case 'F':
                token.m_fix += ch;
                break;
            default:
                return true;
            }
            nextch();
        }
    }
    inline bool Lexer::do_integer_suffix(Token& token)
    {
        for (;;)
        {
            char_type ch = peekch();
            switch (ch)
            {
            case 'u': case 'l': case 'U': case 'L':
                token.m_fix += ch;
                break;
            default:
                return true;
            }
            nextch();
        }
    }
    inline bool Lexer::do_pragma(const TokensType& tokens)
    {
        if (tokens[0].m_str == "pack")
        {
            if (tokens[1].m_str == "(")
            {
                if (tokens[2].m_str == ")")
                {
                    m_pack = 8;
                    return true;
                }
                else if (tokens[2].m_str == "show")
                {
                    m_aux.add_warning(pos(), "packing is %d", m_pack);
                    return true;
                }
                else if (tokens[2].m_str == "push")
                {
                    if (tokens[3].m_str == ",")
                    {
                        if (tokens[4].m_str == "_CRT_PACKING")
                        {
                            push_pack(8);
                            return true;
                        }
                        else
                        {
                            int i = atoi(tokens[2].m_str.c_str());
                            if (i == 0)
                                i = 1;
                            push_pack(i);
                            return true;
                        }
                    }
                    else
                    {
                        push_pack(8);
                        return true;
                    }
                }
                else if (tokens[2].m_str == "pop")
                {
                    pop_pack();
                    return true;
                }
                else if (tokens[2].m_str == "_CRT_PACKING")
                {
                    m_pack = 8;
                    return true;
                }
                else if (tokens[2].m_type == TK_INTEGER_LITERAL)
                {
                    int i = atoi(tokens[2].m_str.c_str());
                    if (i == 0)
                        i = 1;
                    m_pack = i;
                    return true;
                }
            }
        }

        return false;
    }
    inline bool Lexer::do_line(const TokensType& tokens)
    {
        size_t i = 0;
        if (tokens[0].m_str == "line")
            i = 1;

        size_t line = std::stoi(tokens[i++].m_str);
        std::string file;
        if (tokens[i].m_type == TK_STRING_LITERAL)
            file = str_unquote(tokens[i].m_str, '"');

        pos(Position(file.c_str(), line - 1));
        return true;
    }
    inline bool Lexer::do_directive(const string_type& str)
    {
        TextScanner text(str);
        Lexer lexer(text, m_aux);
        lexer.do_lex();

        auto& tokens = lexer.m_tokens;
        if (tokens[0].m_str == "pragma")
        {
            tokens.erase(tokens.begin());
            return do_pragma(tokens);
        }
        if (tokens[0].m_type == TK_INTEGER_LITERAL || 
            tokens[0].m_str == "line")
        {
            return do_line(tokens);
        }
        return false;
    }
    inline bool Lexer::scan_string_or_character()
    {
        Token token(pos(), TK_STRING_LITERAL);

        char_type ch = peekch();
        if (ch == 'L')
        {
            token.m_fix += ch;
            nextch();
        }

        char_type ch0 = ch = peekch();

        if (ch == '\'')
            token.m_type = TK_CHARACTER_LITERAL;
        else if (ch == '"')
            token.m_type = TK_STRING_LITERAL;
        else
            return false;

        token.m_str += ch;
        nextch();

        for (;;)
        {
            ch = peekch();
            switch (ch)
            {
            case 0:
                return false;
            case '\\':
                token.m_str += ch;
                nextch();

                ch = peekch();
                break;
            default:
                if (ch == ch0)
                {
                    token.m_str += ch;
                    nextch();

                    ch = peekch();
                    if (ch != ch0)
                    {
                        push_back(token);
                        return true;
                    }
                }
                break;
            }
            token.m_str += ch;
            nextch();
        }
    }
    inline bool Lexer::scan_identifier()
    {
        Token token(pos(), TK_IDENTIFIER);

        char_type ch = peekch();
        if (!is_alpha(ch) && ch != '_')
            return false;

        token.m_str += ch;
        nextch();

        for (;;)
        {
            ch = peekch();
            if (!is_alnum(ch) && ch != '_')
                break;
            token.m_str += ch;
            nextch();
        }

        if (m_keywords.count(token.m_str) > 0)
        {
            token.m_type = TK_KEYWORD;

            if (token.m_str == "__pragma")
            {
                m_pragma_begin = m_tokens.size();
                m_pragma_paren = 0;
            }
        }

        push_back(token);
        return true;
    }
    inline bool Lexer::scan_symbol()
    {
        Token token(pos(), TK_SYMBOL);

        char_type ch = peekch();
        string_type str;
        typedef symbol_map_type::iterator IT;
        std::pair<IT, IT> p = m_symbol_map.equal_range(ch);
        for (IT it = p.first; it != p.second; ++it)
        {
            if (match_peek(it->second.c_str(), str))
            {
                if (token.m_str.size() < str.size())
                {
                    token.m_str = str;
                }
            }
        }

        if (token.m_str.size())
        {
            skip(token.m_str.size());
            push_back(token);

            if (m_pragma_begin != size_t(-1))
            {
                if (token.m_str == "(")
                {
                    ++m_pragma_paren;
                }
                else if (token.m_str == ")")
                {
                    --m_pragma_paren;
                    if (m_pragma_paren == 0)
                    {
                        TokensType tokens(m_tokens.begin() + m_pragma_begin, m_tokens.end());
                        m_tokens.erase(m_tokens.begin() + m_pragma_begin, m_tokens.end());
                        tokens.erase(tokens.begin() + tokens.size() - 1);
                        tokens.erase(tokens.begin());
                        tokens.erase(tokens.begin());
                        do_pragma(tokens);
                        m_pragma_begin = -1;
                    }
                }
            }
            return true;
        }

        return false;
    }
    inline void Lexer::fixup()
    {
        fixup_gnu_extensions();
    }
    inline void Lexer::fixup_gnu_extensions()
    {
        for (size_t i = 0; i < size(); ++i)
        {
            if (m_tokens[i].m_str == "__extension__")
            {
                m_tokens.erase(m_tokens.begin() + i);
                --i;
            }
        }
    }

    inline size_t Lexer::paren_close(size_t i) const
    {
        int nest = 0;
        for (; i < m_tokens.size(); ++i)
        {
            const string_type& str = m_tokens[i].m_str;
            if (str == "(")
            {
				++nest;
            }
            else if (str == ")")
            {
                --nest;
                if (nest == 0)
                    return i;
            }
        }
        return m_tokens.size();
    }
    inline size_t Lexer::brace_close(size_t i) const
    {
        int nest = 0;
        for (; i < m_tokens.size(); ++i)
        {
            const string_type& str = m_tokens[i].m_str;
            if (str == "{")
            {
				++nest;
            }
            else if (str == "}")
            {
                --nest;
                if (nest == 0)
                    return i;
            }
        }
        return m_tokens.size();
    }

    /////////////////////////////////////////////////////////////////////////
    // string inlines

    template <typename T_STR>
    inline bool
    str_replace_all(T_STR& str, const T_STR& from, const T_STR& to)
    {
        bool ret = false;
        size_t i = 0;
        for (;;) {
            i = str.find(from, i);
            if (i == T_STR::npos)
                break;
            ret = true;
            str.replace(i, from.size(), to);
            i += to.size();
        }
        return ret;
    }
    template <typename T_STR>
    inline bool
    str_replace_all(T_STR& str,
                     const typename T_STR::value_type *from,
                     const typename T_STR::value_type *to)
    {
        return str_replace_all(str, T_STR(from), T_STR(to));
    }

    inline void str_trim(std::string& str, const char *spaces)
    {
        size_t i = str.find_first_not_of(spaces);
        size_t j = str.find_last_not_of(spaces);
        if ((i == std::string::npos) || (j == std::string::npos))
        {
            str.clear();
        }
        else
        {
            str = str.substr(i, j - i + 1);
        }
    }
    inline void str_trim(std::wstring& str, const wchar_t *spaces)
    {
        size_t i = str.find_first_not_of(spaces);
        size_t j = str.find_last_not_of(spaces);
        if ((i == std::wstring::npos) || (j == std::wstring::npos))
        {
            str.clear();
        }
        else
        {
            str = str.substr(i, j - i + 1);
        }
    }

    inline std::string str_quote(const std::string& str, char quote)
    {
        std::string ret;
        ret += quote;
        for (size_t i = 0; i < str.size(); ++i)
        {
            char ch = str[i];
            switch (ch)
            {
            case '\'':
                if (quote == ch)
                    ret += "\\'";
                else
                    ret += ch;
                break;
            case '\"':
                if (quote == ch)
                    ret += "\\\"";
                else
                    ret += ch;
                break;
            case '\\': ret += "\\\\"; break;
            case '\0': ret += "\\0"; break;
            case '\a': ret += "\\a"; break;
            case '\b': ret += "\\b"; break;
            case '\f': ret += "\\f"; break;
            case '\n': ret += "\\n"; break;
            case '\r': ret += "\\r"; break;
            case '\t': ret += "\\t"; break;
            case '\v': ret += "\\v"; break;
            default:
                if ((signed char)ch < 0x20)
                {
                    // hex
                    char buf[8];
                    std::sprintf(buf, "\\x%02X", (unsigned char)ch);
                    ret += buf;
                }
                else
                {
                    ret += ch;
                }
                break;
            }
        }
        ret += quote;
        return ret;
    }
    inline std::string str_unquote(const std::string& str, char quote)
    {
        static const char hex[] = "0123456789ABCDEF";
        std::string str0 = str;
        str_trim(str0);

        std::string ret;
        if (str0.empty())
            return ret;

        if (str0[0] == quote && str0[str0.size() - 1] == quote)
            str0 = str0.substr(1, str.size() - 2);

        int n;
        for (size_t i = 0; i < str0.size(); ++i)
        {
            if (str0[i] == '\\')
            {
                ++i;
                switch (str0[i])
                {
                case '\'': ret += "'"; break;
                case '\"': ret += "\""; break;
                case '\\': ret += "\\"; break;
                case 'a': ret += "\a"; break;
                case 'b': ret += "\b"; break;
                case 'f': ret += "\f"; break;
                case 'n': ret += "\n"; break;
                case 'r': ret += "\r"; break;
                case 't': ret += "\t"; break;
                case 'v': ret += "\v"; break;
                case 'x':
                    // hex
                    n = 0;
                    ++i;
                    if (is_xdigit(str0[i]))
                    {
                        n *= 16;
                        n += strchr(hex, str0[i]) - hex;
                        ++i;
                        if (is_xdigit(str0[i]))
                        {
                            n *= 16;
                            n += strchr(hex, str0[i]) - hex;
                            ++i;
                        }
                    }
                    --i;
                    ret += (char)n;
                    break;
                default:
                    n = 0;
                    if (is_digit(str0[i]))
                    {
                        // octal
                        n *= 8;
                        n += strchr(hex, str0[i]) - hex;
                        ++i;
                        if (is_digit(str0[i]))
                        {
                            n *= 8;
                            n += strchr(hex, str0[i]) - hex;
                            ++i;
                            if (is_digit(str0[i]))
                            {
                                n *= 8;
                                n += strchr(hex, str0[i]) - hex;
                                ++i;
                            }
                        }
                        --i;
                        ret += (char)n;
                    }
                    else
                    {
                        ret += str0[i];
                    }
                    break;
                }
            }
            else if (str0[i] == quote)
            {
                ++i;
                if (str0[i] == quote && quote == '"')
                {
                    ret += "\"";
                }
                else
                {
                    return ret;
                }
            }
            else
            {
                ret += str0[i];
            }
        }
        return ret;
    }
    inline std::string wstr_quote(const std::wstring& str, wchar_t quote)
    {
        std::string ret;
        ret += char(quote);
        for (size_t i = 0; i < str.size(); ++i)
        {
            wchar_t wch = str[i];
            switch (wch)
            {
            case L'\'':
                if (quote == wch)
                    ret += "\\'";
                else
                    ret += (char)wch;
                break;
            case '\"':
                if (quote == wch)
                    ret += "\\\"";
                else
                    ret += (char)wch;
                break;
            case L'\\': ret += "\\\\"; break;
            case L'\0': ret += "\\0"; break;
            case L'\a': ret += "\\a"; break;
            case L'\b': ret += "\\b"; break;
            case L'\f': ret += "\\f"; break;
            case L'\n': ret += "\\n"; break;
            case L'\r': ret += "\\r"; break;
            case L'\t': ret += "\\t"; break;
            case L'\v': ret += "\\v"; break;
            default:
                if (wch < 0x20 || 0x7F < wch)
                {
                    // hex
                    char buf[32];
                    if (wch <= 0xFF)
                    {
                        std::sprintf(buf, "\\x%02X", (char)wch);
                    }
                    else if (wch <= 0xFFFF)
                    {
                        std::sprintf(buf, "\\u%04X", wch);
                    }
                    else
                    {
                        std::sprintf(buf, "\\U%08X", wch);
                    }
                    ret += buf;
                }
                else
                {
                    ret += (char)wch;
                }
                break;
            }
        }
        ret += char(quote);
        return ret;
    }
    inline std::wstring wstr_unquote(const std::string& str, char quote)
    {
        static const char hex[] = "0123456789ABCDEF";

        std::wstring ret;
        std::string str0 = str;
        str_trim(str0);
        if (str.empty())
            return ret;

        if (str0[0] == quote && str0[str0.size() - 1] == quote)
            str0 = str0.substr(1, str0.size() - 2);

        int n;
        for (size_t i = 0; i < str0.size(); ++i)
        {
            if (str0[i] == '\\')
            {
                ++i;
                switch (str0[i])
                {
                case '\'': ret += L"'"; break;
                case '\"': ret += L"\""; break;
                case '\\': ret += L"\\"; break;
                case 'a': ret += L"\a"; break;
                case 'b': ret += L"\b"; break;
                case 'f': ret += L"\f"; break;
                case 'n': ret += L"\n"; break;
                case 'r': ret += L"\r"; break;
                case 't': ret += L"\t"; break;
                case 'v': ret += L"\v"; break;
                case 'x':
                    n = 0;
                    ++i;
                    if (is_xdigit(str0[i]))
                    {
                        n *= 16;
                        n += strchr(hex, str0[i]) - hex;
                        ++i;
                        if (is_xdigit(str0[i]))
                        {
                            n *= 16;
                            n += strchr(hex, str0[i]) - hex;
                            ++i;
                        }
                    }
                    --i;
                    ret += (wchar_t)n;
                    break;
                case 'u':
                    n = 0;
                    ++i;
                    if (is_xdigit(str0[i]))
                    {
                        n *= 16;
                        n += strchr(hex, str0[i]) - hex;
                        ++i;
                        if (is_xdigit(str0[i]))
                        {
                            n *= 16;
                            n += strchr(hex, str0[i]) - hex;
                            ++i;
                            if (is_xdigit(str0[i]))
                            {
                                n *= 16;
                                n += strchr(hex, str0[i]) - hex;
                                ++i;
                                if (is_xdigit(str0[i]))
                                {
                                    n *= 16;
                                    n += strchr(hex, str0[i]) - hex;
                                    ++i;
                                }
                            }
                        }
                    }
                    --i;
                    ret += (wchar_t)n;
                    break;
                case 'U':
                    n = 0;
                    ++i;
                    if (is_xdigit(str0[i]))
                    {
                        n *= 16;
                        n += strchr(hex, str0[i]) - hex;
                        ++i;
                        if (is_xdigit(str0[i]))
                        {
                            n *= 16;
                            n += strchr(hex, str0[i]) - hex;
                            ++i;
                            if (is_xdigit(str0[i]))
                            {
                                n *= 16;
                                n += strchr(hex, str0[i]) - hex;
                                ++i;
                                if (is_xdigit(str0[i]))
                                {
                                    n *= 16;
                                    n += strchr(hex, str0[i]) - hex;
                                    ++i;
                                    if (is_xdigit(str0[i]))
                                    {
                                        n *= 16;
                                        n += strchr(hex, str0[i]) - hex;
                                        ++i;
                                        if (is_xdigit(str0[i]))
                                        {
                                            n *= 16;
                                            n += strchr(hex, str0[i]) - hex;
                                            ++i;
                                            if (is_xdigit(str0[i]))
                                            {
                                                n *= 16;
                                                n += strchr(hex, str0[i]) - hex;
                                                ++i;
                                                if (is_xdigit(str0[i]))
                                                {
                                                    n *= 16;
                                                    n += strchr(hex, str0[i]) - hex;
                                                    ++i;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    --i;
                    ret += (wchar_t)n;
                    break;
                default:
                    n = 0;
                    if (is_digit(str0[i]))
                    {
                        // octal
                        n *= 8;
                        n += strchr(hex, str0[i]) - hex;
                        ++i;
                        if (is_digit(str0[i]))
                        {
                            n *= 8;
                            n += strchr(hex, str0[i]) - hex;
                            ++i;
                            if (is_digit(str0[i]))
                            {
                                n *= 8;
                                n += strchr(hex, str0[i]) - hex;
                                ++i;
                            }
                        }
                        --i;
                        ret += (wchar_t)n;
                    }
                    else
                    {
                        ret += (wchar_t)str0[i];
                    }
                    break;
                }
            }
            else if (str0[i] == quote)
            {
                ++i;
                if (str0[i] == quote && quote == '"')
                {
                    ret += L"\"";
                }
                else
                {
                    return ret;
                }
            }
            else
            {
                wchar_t wch;
                int code = mbtowc(&wch, &str0[i], str0.size() - i);
                if (code < 0)
                {
                    ret += L'?';
                }
                else if (code == 0)
                {
                    ret += L'\0';
                }
                else
                {
                    ret += wch;
                    i += code - 1;
                }
            }
        }
        return ret;
    }
} // namespace CodeReverse

/////////////////////////////////////////////////////////////////////////

#endif  // ndef CODEREVERSE_LEXER_HPP
