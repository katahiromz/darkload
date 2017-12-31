// Main.hpp --- darkload main
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#include "CParser.hpp"
#include <iostream>
#include <fstream>

void show_help(void)
{
    std::cout <<
        "darkload --- C parser by katahiromz\n"
        "Usage: darkload input_file.i" << std::endl;
}

void show_version(void)
{
    std::cout << "darkload 0.0 by katahiromz\n";
}

int do_parse(const std::string& str)
{
    using namespace CodeReverse;
    TextScanner text(str);
    AuxInfo aux;

    CodeReverse::Lexer lexer(text, aux);
    std::cerr << "lexing...\n";
    if (lexer.do_lex())
    {
        lexer.fixup();
        //std::cout << lexer;

        CParser parser(lexer);
        std::cerr << "parsing...\n";
        if (parser.do_parse())
        {
            if (auto ast = parser.ast())
            {
                std::cerr << "done.\n";
                return 0;
            }
        }
        else
        {
            std::cerr << "Failed.\n";
        }
    }

    os_type os;
    aux.err_out(os);
    std::cout << os.str();
    return 1;
}

int just_do_it(int argc, char **argv)
{
    const char *fname = NULL;
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            std::cerr << "error: invalid argument '" << argv[i] << "'\n";
            return 2;
        }
        else
        {
            if (fname)
            {
                std::cerr << "error: multiple input file\n";
                return 1;
            }
            else
            {
                fname = argv[i];
            }
        }
    }

    if (fname == NULL)
    {
        std::cerr << "error: no input file\n";
        return 3;
    }

    std::ifstream fs(fname);
    if (!fs)
    {
        std::cerr << "error: cannot open input file '" << fname << "'\n";
        return 4;
    }
    fs.unsetf(std::ios::skipws);
    std::istreambuf_iterator<char> it = fs.rdbuf(), end;
    std::string text(it, end);

    return do_parse(text);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        show_help();
        return 0;
    }

    std::string arg1 = argv[1];
    if (arg1 == "--help")
    {
        show_help();
        return 0;
    }
    if (arg1 == "--version")
    {
        show_version();
        return 0;
    }

    return just_do_it(argc, argv);
}
