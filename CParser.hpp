#ifndef CODEREVERSE_C_PARSER_HPP
#define CODEREVERSE_C_PARSER_HPP

#include "Lexer.hpp"
#include "AST.hpp"
#include <set>

#ifndef NDEBUG
    #include <iostream>
#endif

#ifndef DEBUG_ONLY
    #ifdef NDEBUG
        #define DEBUG_ONLY(x)   /*empty*/
    #else
        #define DEBUG_ONLY(x)   x
    #endif
#endif

#if 1
    #define CR_SHOW_STATUS()        /*empty*/
    #define CR_RETURN_AST(value)    return (value)
#else
    #define CR_SHOW_STATUS() \
        DEBUG_ONLY(std::cout << __func__ << ": " << str() << std::endl)

    #define CR_RETURN_AST(value) do { \
        DEBUG_ONLY(std::cout << __func__ << ": (" << __LINE__ << "): " << \
                   str() << ": return " << s_p<AST_base>(value) << std::endl); \
        return value; \
    } while (0)
#endif

/////////////////////////////////////////////////////////////////////////

namespace CodeReverse
{
    /////////////////////////////////////////////////////////////////////////
    // CParser

    class CParser
    {
    public:
        CParser(Lexer& lexer);

        bool do_parse();

        s_p<AST_translation_unit> ast();
        void clear();

        TokenType type() const;
        string_type str() const;
        string_type fix() const;
        bool eof() const;
        void next();
        void prev();
        bool next_if(const char *psz);
        size_t paren_close();
        size_t brace_close();

        size_t index() const;
        void index(size_t pos);
        Position pos() const;
        Position parse_pos() const;

    protected:
        Lexer& m_lexer;
        AuxInfo& m_aux;
        s_p<AST_translation_unit> m_ast;
        typedef std::set<string_type> typedef_names_type;
        typedef_names_type m_typedef_names;
        std::set<string_type> m_enum_constant_names;
        typedef std::set<size_t> index_set_type;

        void add_typedef_name(const string_type& str);
        void care_of_typedefs(s_p<AST_declaration_specifiers> decl_specs, s_p<AST_declaration> decl);
        bool scan_function_attribute(attributes_type& attrs);
        bool scan_attribute(attributes_type& attrs);
        bool scan_declspec(attributes_type& attrs);
        bool scan_gnu_attribute(attributes_type& attrs);

        void trim_underline(string_type& str);

        s_p<AST_translation_unit> visit_translation_unit();
        s_p<AST_external_declaration> visit_external_declaration();
        s_p<AST_function_definition> visit_function_definition();
        s_p<AST_declaration> visit_declaration();
        s_p<AST_declaration_specifiers> visit_declaration_specifiers();
        s_p<AST_declaration_specifier> visit_declaration_specifier();
        s_p<AST_declarator> visit_declarator();
        s_p<AST_declaration_list> visit_declaration_list();
        s_p<AST_compound_statement> visit_compound_statement();
        s_p<AST_declaration_or_statement> visit_declaration_or_statement();
        s_p<AST_init_declarator_list> visit_init_declarator_list();
        s_p<AST_init_declarator> visit_init_declarator();
        s_p<AST_static_assert_declaration> visit_static_assert_declaration();
        s_p<AST_storage_class_specifier> visit_storage_class_specifier();
        s_p<AST_type_specifier> visit_type_specifier();
        s_p<AST_type_qualifier> visit_type_qualifier();
        s_p<AST_function_specifier> visit_function_specifier();
        s_p<AST_alignment_specifier> visit_alignment_specifier();
        s_p<AST_pointer> visit_pointer();
        s_p<AST_direct_declarator> visit_direct_declarator();
        s_p<AST_identifier_list> visit_identifier_list();
        s_p<AST_identifier> visit_identifier(bool tag = false);
        s_p<AST_initializer_list> visit_initializer_list();
        s_p<AST_designative_initializer> visit_designative_initializer();
        s_p<AST_initializer> visit_initializer();
        s_p<AST_constant_expression> visit_constant_expression();
        s_p<AST_atomic_type_specifier> visit_atomic_type_specifier();
        s_p<AST_struct_or_union_specifier> visit_struct_or_union_specifier();
        s_p<AST_struct_declaration_list> visit_struct_declaration_list();
        s_p<AST_struct_declaration> visit_struct_declaration();
        s_p<AST_enum_specifier> visit_enum_specifier();
        s_p<AST_enumerator_list> visit_enumerator_list();
        s_p<AST_enumerator> visit_enumerator();
        s_p<AST_type_name> visit_type_name();
        s_p<AST_specifier_qualifier_list> visit_specifier_qualifier_list();
        s_p<AST_specifier_qualifier> visit_specifier_qualifier();
        s_p<AST_abstract_declarator> visit_abstract_declarator();
        s_p<AST_direct_abstract_declarator> visit_direct_abstract_declarator();
        s_p<AST_struct_declarator_list> visit_struct_declarator_list();
        s_p<AST_type_qualifier_list> visit_type_qualifier_list();
        s_p<AST_parameter_type_list> visit_parameter_type_list();
        s_p<AST_struct_declarator> visit_struct_declarator();
        s_p<AST_parameter_list> visit_parameter_list();
        s_p<AST_parameter_declaration> visit_parameter_declaration();
        s_p<AST_expression> visit_expression();
        s_p<AST_assignment_expression> visit_assignment_expression();
        s_p<AST_conditional_expression> visit_conditional_expression();
        s_p<AST_logical_or_expression> visit_logical_or_expression();
        s_p<AST_logical_and_expression> visit_logical_and_expression();
        s_p<AST_inclusive_or_expression> visit_inclusive_or_expression();
        s_p<AST_exclusive_or_expression> visit_exclusive_or_expression();
        s_p<AST_and_expression> visit_and_expression();
        s_p<AST_equality_expression> visit_equality_expression();
        s_p<AST_relational_expression> visit_relational_expression();
        s_p<AST_shift_expression> visit_shift_expression();
        s_p<AST_additive_expression> visit_additive_expression();
        s_p<AST_multiplicative_expression> visit_multiplicative_expression();
        s_p<AST_cast_expression> visit_cast_expression();
        s_p<AST_unary_expression> visit_unary_expression();
        s_p<AST_postfix_expression> visit_postfix_expression();
        s_p<AST_primary_expression> visit_primary_expression();
        s_p<AST_argument_expression_list> visit_argument_expression_list();
        s_p<AST_constant> visit_constant();
        s_p<AST_generic_selection> visit_generic_selection();
        s_p<AST_generic_assoc_list> visit_generic_assoc_list();
        s_p<AST_generic_association> visit_generic_association();
        s_p<AST_designation> visit_designation();
        s_p<AST_designator_list> visit_designator_list();
        s_p<AST_designator> visit_designator();
        s_p<AST_statement> visit_statement();
        s_p<AST_labeled_statement> visit_labeled_statement();
        s_p<AST_expression_statement> visit_expression_statement();
        s_p<AST_selection_statement> visit_selection_statement();
        s_p<AST_iteration_statement> visit_iteration_statement();
        s_p<AST_jump_statement> visit_jump_statement();
        s_p<AST_asm_statement> visit_asm_statement();
    };

    /////////////////////////////////////////////////////////////////////////
    // CParser inlines

    inline CParser::CParser(Lexer& lexer)
        : m_lexer(lexer), m_aux(lexer.m_aux)
    {
        m_typedef_names.insert("__builtin_va_list");
        m_typedef_names.insert("va_list");
    }

    inline s_p<AST_translation_unit> CParser::ast()
    {
        return m_ast;
    }
    inline void CParser::clear()
    {
        m_ast.reset();
        m_lexer.clear();
    }

    inline TokenType CParser::type() const
    {
        return m_lexer.type();
    }
    inline string_type CParser::str() const
    {
        return m_lexer.str();
    }
    inline string_type CParser::fix() const
    {
        return m_lexer.fix();
    }
    inline bool CParser::eof() const
    {
        return m_lexer.eof();
    }
    inline void CParser::next()
    {
        m_lexer.next();
    }
    inline void CParser::prev()
    {
        m_lexer.prev();
    }
    inline bool CParser::next_if(const char *psz)
    {
        if (str() == psz)
        {
            next();
            return true;
        }
        return false;
    }
    inline size_t CParser::paren_close()
    {
        int nest = 0;
        for (size_t i = index(); i < m_lexer.size(); ++i)
        {
            string_type& str = m_lexer[i].m_str;
            if (str == "(")
            {
                ++nest;
            }
            else if (str == ")")
            {
                --nest;
                if (nest == 0)
                    return i + 1;
            }
        }
        return m_lexer.size();
    }
    inline size_t CParser::brace_close()
    {
        int nest = 0;
        for (size_t i = index(); i < m_lexer.size(); ++i)
        {
            string_type& str = m_lexer[i].m_str;
            if (str == "{")
            {
                ++nest;
            }
            else if (str == "}")
            {
                --nest;
                if (nest == 0)
                    return i + 1;
            }
        }
        return m_lexer.size();
    }
    inline size_t CParser::index() const
    {
        return m_lexer.index();
    }
    inline void CParser::index(size_t pos)
    {
        m_lexer.index(pos);
    }
    inline Position CParser::pos() const
    {
        return m_lexer.pos();
    }
    inline Position CParser::parse_pos() const
    {
        return m_lexer[index()].m_pos;
    }
    inline bool CParser::do_parse()
    {
        m_ast = visit_translation_unit();
        if (!m_ast)
        {
            m_aux.add_error(parse_pos(), "parse error (%d): %s",
                            int(type()), str().c_str());
        }
        return !!m_ast;
    }

    inline void CParser::add_typedef_name(const string_type& str)
    {
        m_typedef_names.insert(str);
    }

    inline void CParser::care_of_typedefs(
        s_p<AST_declaration_specifiers> decl_specs,
        s_p<AST_declaration> decl)
    {
        if ((*decl_specs)[0]->m_sto_class_spec &&
            (*decl_specs)[0]->m_sto_class_spec->m_str == "typedef" &&
            decl->m_init_declor_list)
        {
            auto init_declor_list = decl->m_init_declor_list;
            for (size_t i = 0; i < init_declor_list->size(); ++i)
            {
                auto init_declor = (*init_declor_list)[i];
                auto dir_declor = init_declor->m_declor->m_dir_declor;
                while (dir_declor)
                {
                    if (dir_declor->m_ident)
                    {
                        add_typedef_name(dir_declor->m_ident->m_str);
                        break;
                    }
                    if (dir_declor->m_child)
                        dir_declor = dir_declor->m_child;
                    else if (dir_declor->m_declor->m_dir_declor)
                        dir_declor = dir_declor->m_declor->m_dir_declor;
                }
            }
        }
    }

    inline void CParser::trim_underline(string_type& str)
    {
        if (str.size() > 2)
        {
            if (str.substr(0, 2) == "__" &&
                str.substr(str.size() - 2, 2) == "__")
            {
                str = str.substr(2, str.size() - 4);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////
    // CParser::scan_* inlines

    // function-attribute = '__cdecl'
    //                    | '__fastcall'
    //                    | '__stdcall'
    //                    | attribute;
    // 
    // attribute = declspec
    //           | gnu-attribute;
    inline bool CParser::scan_function_attribute(attributes_type& attrs)
    {
        CR_SHOW_STATUS();
        if (str() == "__cdecl" || str() == "__fastcall" ||
            str() == "__stdcall")
        {
            attrs[str().substr(2)] = "";
            next();
            return true;
        }
        return scan_attribute(attrs);
    }

    // attribute = declspec
    //           | gnu-attribute;
    inline bool CParser::scan_attribute(attributes_type& attrs)
    {
        CR_SHOW_STATUS();
        auto i = index();
        if (scan_declspec(attrs))
        {
            return true;
        }
        index(i);
        if (scan_gnu_attribute(attrs))
        {
            return true;
        }
        index(i);
        return false;
    }

    // declspec = '__declspec', '(', {extended-decl-modifier}, ')';
    // extended-decl-modifier = ? See https://msdn.microsoft.com/en-us/library/dabb5z75.aspx ?;
    inline bool CParser::scan_declspec(attributes_type& attrs)
    {
        CR_SHOW_STATUS();
        if (next_if("__declspec") && next_if("("))
        {
            size_t level = 1;
            string_type name = str(), value;
            next();
            for (;;)
            {
                if (str() == "(")
                {
                    ++level;
                }
                else if (str() == ")")
                {
                    --level;
                    if (level == 0)
                        break;
                }
                value += str();
                next();
            }
            next();
            attrs[name] = value;
            return true;
        }
        return false;
    }

    // gnu-attribute = '__attribute__', '(', '(', gnu-attribute-content, {',', gnu-attribute-content}, ')', ')';
    // gnu-attribute-content = ? See https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html ?;
    inline bool CParser::scan_gnu_attribute(attributes_type& attrs)
    {
        CR_SHOW_STATUS();
        if (next_if("__attribute__"))
        {
            auto i = paren_close();
            next_if("("); next_if("("); // ))

            do
            {
                string_type name = str(), value;
                trim_underline(name);
                next();
                if (str() == "(")
                {
                    auto k1 = paren_close();
                    for (auto k0 = index(); k0 < k1; ++k0, next())
                    {
                        string_type item = str();
                        if (item == "__alignof")
                            item = "alignof";
                        trim_underline(item);

                        value += ' ';
                        value += item;
                    }
                    str_trim(value);
                    while (!value.empty() &&
                           value[0] == '(' && value[value.size() - 1] == ')')
                    {
                        value = value.substr(1, value.size() - 2);
                        str_trim(value);
                    }
                    str_replace_all(value, " (", "(");
                    str_replace_all(value, "( ", "(");
                    str_replace_all(value, ", ", ",");
                    str_replace_all(value, " ,", ",");
                    str_replace_all(value, " )", ")");
                    str_replace_all(value, ") ", ")");
                }
                //std::cout << name << "##" << value << std::endl;
                attrs[name] = value;
            } while (next_if(","));

            index(i);
            return true;
        }
        return false;
    }

    /////////////////////////////////////////////////////////////////////////
    // CParser::visit_* inlines

    // translation-unit = {external-declaration};
    inline s_p<AST_translation_unit> CParser::visit_translation_unit()
    {
        CR_SHOW_STATUS();
        auto trans_unit = m_s<AST_translation_unit>();
        for (;;)
        {
            auto i = index();
            //if (parse_pos().file().find("winnt.h") != string_type::npos &&
            //    parse_pos().line() >= 11878)
            //{
            //    puts("OK");
            //}
            if (auto ext_decl = visit_external_declaration())
            {
                trans_unit->push_back(ext_decl);
            }
            else
            {
                index(i);
                break;
            }
        }
        if (eof())
            CR_RETURN_AST(trans_unit);
        CR_RETURN_AST(nullptr);
    }

    // external-declaration = function-definition
    //                      | declaration;
    inline s_p<AST_external_declaration> CParser::visit_external_declaration()
    {
        CR_SHOW_STATUS();
        auto extern_decl = m_s<AST_external_declaration>();
        auto i = index();
        if (auto decl = visit_declaration())
        {
            extern_decl->m_decl = decl;
            CR_RETURN_AST(extern_decl);
        }
        index(i);
        if (auto func_def = visit_function_definition())
        {
            extern_decl->m_func_def = func_def;
            CR_RETURN_AST(extern_decl);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // function-definition = declaration-specifiers, {function-attribute}, declarator, [declaration-list], compound-statement;
    inline s_p<AST_function_definition> CParser::visit_function_definition()
    {
        CR_SHOW_STATUS();
        auto func_def = m_s<AST_function_definition>();
        if (auto decl_specs = visit_declaration_specifiers())
        {
            func_def->m_decl_specs = decl_specs;
            auto i = index();
            if (scan_function_attribute(func_def->m_attrs))
            {
                i = index();
            }
            index(i);
            if (auto declor = visit_declarator())
            {
                func_def->m_declor = declor;
                i = index();
                if (auto decl_list = visit_declaration_list())
                {
                    func_def->m_decl_list = decl_list;
                    i = index();
                }
                index(i);
                auto names_save = m_typedef_names;
                if (auto comp_stmt = visit_compound_statement())
                {
                    func_def->m_comp_stmt = comp_stmt;
                    m_typedef_names = names_save;
                    CR_RETURN_AST(func_def);
                }
                m_typedef_names = names_save;
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // declaration = declaration-specifiers, [init-declarator-list], {function-attribute}, ';'
    //             | static-assert-declaration
    //             | ';';
    inline s_p<AST_declaration> CParser::visit_declaration()
    {
        CR_SHOW_STATUS();
        auto decl = m_s<AST_declaration>();
        if (next_if(";"))
        {
            CR_RETURN_AST(decl);
        }

        bool is_typedef = (str() == "typedef");

        auto i = index();
        if (auto decl_specs = visit_declaration_specifiers())
        {
            decl->m_decl_specs = decl_specs;
            auto k = i = index();
            if (auto init_declor_list = visit_init_declarator_list())
            {
                decl->m_init_declor_list = init_declor_list;
                i = index();
            }
            index(i);
            while (scan_function_attribute(decl->m_attrs))
            {
                i = index();
            }
            if (next_if(";"))
            {
                care_of_typedefs(decl_specs, decl);
                CR_RETURN_AST(decl);
            }
            // failed. retry with removing typedef-name
            if (is_typedef && (*decl_specs.get())[decl_specs->size() - 1]->is_typedef_name())
            {
                decl_specs->resize(decl_specs->size() - 1);
                index(k - 1);
                i = index();
                if (auto init_declor_list = visit_init_declarator_list())
                {
                    decl->m_init_declor_list = init_declor_list;
                    i = index();
                }
                index(i);
                while (scan_function_attribute(decl->m_attrs))
                {
                    i = index();
                }
                if (next_if(";"))
                {
                    care_of_typedefs(decl_specs, decl);
                    CR_RETURN_AST(decl);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        index(i);
        if (auto static_assert_decl = visit_static_assert_declaration())
        {
            decl->m_static_assert_decl = static_assert_decl;
            CR_RETURN_AST(decl);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // declaration-specifiers = declaration-specifier, {declaration-specifier};
    inline s_p<AST_declaration_specifiers> CParser::visit_declaration_specifiers()
    {
        CR_SHOW_STATUS();
        auto decl_specs = m_s<AST_declaration_specifiers>();
        auto i = index();
        if (auto decl_spec = visit_declaration_specifier())
        {
            do
            {
                decl_specs->push_back(decl_spec);
                i = index();
                decl_spec = visit_declaration_specifier();
            } while (decl_spec);
            index(i);
            CR_RETURN_AST(decl_specs);
        }
        CR_RETURN_AST(nullptr);
    }

    // declaration-specifier = storage-class-specifier
    //                       | type-specifier
    //                       | type-qualifier
    //                       | function-specifier
    //                       | alignment-specifier;
    inline s_p<AST_declaration_specifier> CParser::visit_declaration_specifier()
    {
        CR_SHOW_STATUS();
        auto decl_spec = m_s<AST_declaration_specifier>();
        auto i = index();
        if (auto sto_class_spec = visit_storage_class_specifier())
        {
            decl_spec->m_sto_class_spec = sto_class_spec;
            decl_spec->m_type = AST_declaration_specifier::DS_STO_CLASS_SPEC;
            CR_RETURN_AST(decl_spec);
        }
        index(i);
        if (auto type_spec = visit_type_specifier())
        {
            decl_spec->m_type_spec = type_spec;
            decl_spec->m_type = AST_declaration_specifier::DS_TYPE_SPEC;
            CR_RETURN_AST(decl_spec);
        }
        index(i);
        if (auto type_qual = visit_type_qualifier())
        {
            decl_spec->m_type_qual = type_qual;
            decl_spec->m_type = AST_declaration_specifier::DS_TYPE_QUAL;
            CR_RETURN_AST(decl_spec);
        }
        index(i);
        if (auto func_spec = visit_function_specifier())
        {
            decl_spec->m_func_spec = func_spec;
            decl_spec->m_type = AST_declaration_specifier::DS_FUNC_SPEC;
            CR_RETURN_AST(decl_spec);
        }
        index(i);
        if (auto align_spec = visit_alignment_specifier())
        {
            decl_spec->m_align_spec = align_spec;
            decl_spec->m_type = AST_declaration_specifier::DS_ALIGN_SPEC;
            CR_RETURN_AST(decl_spec);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // declarator = {function-attribute}, [pointer], {function-attribute}, direct-declarator;
    inline s_p<AST_declarator> CParser::visit_declarator()
    {
        CR_SHOW_STATUS();
        auto declor = m_s<AST_declarator>();
        auto i = index();
        while (scan_function_attribute(declor->m_attrs))
        {
            i = index();
        }
        index(i);
        if (auto ptr = visit_pointer())
        {
            declor->m_ptr = ptr;
            i = index();
        }
        index(i);
        while (scan_function_attribute(declor->m_attrs))
        {
            i = index();
        }
        index(i);
        if (auto dir_declor = visit_direct_declarator())
        {
            declor->m_dir_declor = dir_declor;
            CR_RETURN_AST(declor);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // declaration-list = declaration, {declaration};
    inline s_p<AST_declaration_list> CParser::visit_declaration_list()
    {
        CR_SHOW_STATUS();
        auto decl_list = m_s<AST_declaration_list>();
        auto i = index();
        if (auto decl = visit_declaration())
        {
            do
            {
                decl_list->push_back(decl);
                i = index();
                decl = visit_declaration();
            } while (decl);
            index(i);
            CR_RETURN_AST(decl_list);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // compound-statement = '{', {declaration-or-statement}, '}';
    inline s_p<AST_compound_statement> CParser::visit_compound_statement()
    {
        CR_SHOW_STATUS();
        auto comp_stmt = m_s<AST_compound_statement>();
        if (next_if("{"))
        {
            auto i = index();
            while (auto decl_or_stmt = visit_declaration_or_statement())
            {
                comp_stmt->m_items.push_back(decl_or_stmt);
                i = index();
            }
            index(i);
            if (next_if("}"))
            {
                CR_RETURN_AST(comp_stmt);
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // declaration-or-statement = declaration | statement;
    inline s_p<AST_declaration_or_statement> CParser::visit_declaration_or_statement()
    {
        CR_SHOW_STATUS();
        auto decl_or_stmt = m_s<AST_declaration_or_statement>();
        auto i = index();
        if (auto decl = visit_declaration())
        {
            decl_or_stmt->m_decl = decl;
            CR_RETURN_AST(decl_or_stmt);
        }
        index(i);
        if (auto stmt = visit_statement())
        {
            decl_or_stmt->m_stmt = stmt;
            CR_RETURN_AST(decl_or_stmt);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // init-declarator-list = init-declarator, {',', init-declarator};
    inline s_p<AST_init_declarator_list> CParser::visit_init_declarator_list()
    {
        CR_SHOW_STATUS();
        auto init_declor_list = m_s<AST_init_declarator_list>();
        auto i = index();
        if (auto init_declor = visit_init_declarator())
        {
            init_declor_list->push_back(init_declor);
            while (next_if(","))
            {
                init_declor = visit_init_declarator();
                if (!init_declor)
                    CR_RETURN_AST(nullptr);

                init_declor_list->push_back(init_declor);
            }
            CR_RETURN_AST(init_declor_list);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // init-declarator = declarator, ['=', initializer];
    inline s_p<AST_init_declarator> CParser::visit_init_declarator()
    {
        CR_SHOW_STATUS();
        auto init_declor = m_s<AST_init_declarator>();
        auto i = index();
        if (auto declor = visit_declarator())
        {
            init_declor->m_declor = declor;
            if (next_if("="))
            {
                auto init = visit_initializer();
                if (!init)
                    CR_RETURN_AST(nullptr);

                init_declor->m_init = init;
            }
            CR_RETURN_AST(init_declor);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // static-assert-declaration = '_Static_assert', '(', constant-expression, ',', string, ')', ';';
    inline s_p<AST_static_assert_declaration> CParser::visit_static_assert_declaration()
    {
        CR_SHOW_STATUS();
        auto static_assert_decl = m_s<AST_static_assert_declaration>();
        if (next_if("_Static_assert") && next_if("("))
        {
            if (auto const_expr = visit_constant_expression())
            {
                if (next_if(","))
                {
                    if (type() == TK_STRING_LITERAL)
                    {
                        static_assert_decl->m_str = str();
                        next();
                        if (next_if(")") && next_if(";"))
                        {
                            CR_RETURN_AST(static_assert_decl);
                        }
                    }
                }
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // storage-class-specifier = 'typedef'
    //                         | 'extern'
    //                         | 'static'
    //                         | '_Thread_local'
    //                         | 'auto'
    //                         | 'register'
    //                         | attribute;
    inline s_p<AST_storage_class_specifier> CParser::visit_storage_class_specifier()
    {
        CR_SHOW_STATUS();
        auto sto_class_spec = m_s<AST_storage_class_specifier>();
        if (str() == "typedef" || str() == "extern" ||
            str() == "static" || str() == "_Thread_local" ||
            str() == "auto" || str() == "register")
        {
            sto_class_spec->m_str = str();
            next();
            CR_RETURN_AST(sto_class_spec);
        }
        if (scan_attribute(sto_class_spec->m_attrs))
        {
            CR_RETURN_AST(sto_class_spec);
        }
        CR_RETURN_AST(nullptr);
    }

    // type-specifier = 'void'
    //                | 'char'
    //                | 'short'
    //                | 'int'
    //                | 'long'
    //                | 'float'
    //                | 'double'
    //                | 'signed'
    //                | 'unsigned'
    //                | '_Bool'
    //                | '_Complex'
    //                | '_Imaginary'       (* non-mandated extension *)
    //                | '__int64'
    //                | '__float80',
    //                | '__float128',
    //                | atomic-type-specifier
    //                | struct-or-union-specifier
    //                | enum-specifier
    //                | typedef-name;
    inline s_p<AST_type_specifier> CParser::visit_type_specifier()
    {
        CR_SHOW_STATUS();
        auto type_spec = m_s<AST_type_specifier>();
        if (str() == "void" || str() == "char" || str() == "short" || 
            str() == "int" || str() == "long" || str() == "float" || 
            str() == "double" || str() == "signed" || str() == "unsigned" ||
            str() == "_Bool" || str() == "_Complex" || str() == "_Imaginary" ||
            str() == "__int64" || str() == "__float80" || str() == "__float128")
        {
            type_spec->m_str = str();
            next();
            type_spec->m_type = AST_type_specifier::TS_OTHER;
            CR_RETURN_AST(type_spec);
        }
        auto i = index();
        if (auto atom_type_spec = visit_atomic_type_specifier())
        {
            type_spec->m_atom_type_spec = atom_type_spec;
            type_spec->m_type = AST_type_specifier::TS_ATOMIC;
            CR_RETURN_AST(type_spec);
        }
        index(i);
        if (auto su_spec = visit_struct_or_union_specifier())
        {
            type_spec->m_su_spec = su_spec;
            type_spec->m_type = AST_type_specifier::TS_STRUCT_OR_UNION;
            CR_RETURN_AST(type_spec);
        }
        index(i);
        if (auto enum_spec = visit_enum_specifier())
        {
            type_spec->m_enum_spec = enum_spec;
            type_spec->m_type = AST_type_specifier::TS_ENUM;
            CR_RETURN_AST(type_spec);
        }
        index(i);
        if (type() == TK_IDENTIFIER && m_typedef_names.count(str()) > 0)
        {
            type_spec->m_str = str();
            next();
            type_spec->m_type = AST_type_specifier::TS_TYPEDEF_NAME;
            CR_RETURN_AST(type_spec);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // type-qualifier = 'const'
    //                | 'restrict'
    //                | 'volatile'
    //                | '_Atomic'
    //                | '__ptr64'
    //                | '__restrict__';
    inline s_p<AST_type_qualifier> CParser::visit_type_qualifier()
    {
        CR_SHOW_STATUS();
        auto type_qual = m_s<AST_type_qualifier>();
        if (str() == "const" || str() == "restrict" ||
            str() == "volatile" || str() == "_Atomic" ||
            str() == "__ptr64")
        {
            type_qual->m_str = str();
            next();
            CR_RETURN_AST(type_qual);
        }
        if (next_if("__restrict__"))
        {
            type_qual->m_str = "restrict";
            CR_RETURN_AST(type_qual);
        }
        CR_RETURN_AST(nullptr);
    }

    // function-specifier = 'inline'
    //                    | '_Noreturn'
    //                    | '__inline'
    //                    | '__inline__'
    //                    | '__forceinline'
    //                    | attribute;
    inline s_p<AST_function_specifier> CParser::visit_function_specifier()
    {
        CR_SHOW_STATUS();
        auto func_spec = m_s<AST_function_specifier>();
        if (str() == "inline" || str() == "_Noreturn")
        {
            func_spec->m_attrs[str()] = "";
            next();
            CR_RETURN_AST(func_spec);
        }
        if (next_if("__inline") || next_if("__inline__") ||
            next_if("__forceinline"))
        {
            func_spec->m_attrs["inline"] = "";
            CR_RETURN_AST(func_spec);
        }
        if (scan_attribute(func_spec->m_attrs))
        {
            CR_RETURN_AST(func_spec);
        }
        CR_RETURN_AST(nullptr);
    }

    // alignment-specifier = '_Alignas', '(', type-name, ')'
    //                     | '_Alignas', '(', constant-expression, ')';
    inline s_p<AST_alignment_specifier> CParser::visit_alignment_specifier()
    {
        CR_SHOW_STATUS();
        auto align_spec = m_s<AST_alignment_specifier>();
        if (next_if("_Alignas") && next_if("("))
        {
            auto i = index();
            if (auto type_name = visit_type_name())
            {
                align_spec->m_type_name = type_name;
                if (next_if(")"))
                {
                    CR_RETURN_AST(align_spec);
                }
            }
            index(i);
            if (auto const_expr = visit_constant_expression())
            {
                align_spec->m_const_expr = const_expr;
                if (next_if(")"))
                {
                    CR_RETURN_AST(align_spec);
                }
            }
            index(i);
        }
        CR_RETURN_AST(nullptr);
    }

    // pointer = '*', [type-qualifier-list], [pointer];
    inline s_p<AST_pointer> CParser::visit_pointer()
    {
        CR_SHOW_STATUS();
        auto ptr = m_s<AST_pointer>();
        if (next_if("*"))
        {
            auto i = index();
            if (auto type_qual_list = visit_type_qualifier_list())
            {
                ptr->m_type_qual_list = type_qual_list;
                i = index();
            }
            index(i);
            if (auto another = visit_pointer())
            {
                ptr->m_child = another;
                i = index();
            }
            index(i);
            CR_RETURN_AST(ptr);
        }
        CR_RETURN_AST(nullptr);
    }

    // direct-declarator = identifier
    //                   | '(', declarator, ')'
    //                   | direct-declarator, '[', ['*'], ']'
    //                   | direct-declarator, '[', 'static', [type-qualifier-list], assignment-expression, ']'
    //                   | direct-declarator, '[', type-qualifier-list, ['*'], ']'
    //                   | direct-declarator, '[', type-qualifier-list, ['static'], assignment-expression, ']'
    //                   | direct-declarator, '[', assignment-expression, ']'
    //                   | direct-declarator, '(', parameter-type-list, ')'
    //                   | direct-declarator, '(', identifier-list, ')'
    //                   | direct-declarator, '(', ')';
    inline s_p<AST_direct_declarator> CParser::visit_direct_declarator()
    {
        CR_SHOW_STATUS();
        auto dir_declor = m_s<AST_direct_declarator>();
        bool ok = false;
        if (next_if("("))
        {
            if (auto declor = visit_declarator())
            {
                dir_declor->m_declor = declor;
                dir_declor->m_type = AST_direct_declarator::DD_DECLOR;
                ok = next_if(")");
            }
        }
        else if (auto ident = visit_identifier(true))
        {
            dir_declor->m_ident = ident;
            dir_declor->m_type = AST_direct_declarator::DD_IDENT;
            ok = true;
        }
        if (ok)
        {
            while (str() == "[" || str() == "(")
            {
                auto another = m_s<AST_direct_declarator>();
                another->m_child = dir_declor;
                dir_declor = another;

                // '(', parameter-type-list, ')' |
                // '(', identifier-list, ')' |
                // '(', ')';
                if (next_if("("))
                {
                    another->m_type = AST_direct_declarator::DD_PAREN;
                    if (next_if(")"))
                    {
                        continue;
                    }
                    auto i = index();
                    if (auto param_type_list = visit_parameter_type_list())
                    {
                        another->m_param_type_list = param_type_list;
                        if (next_if(")"))
                            continue;
                    }
                    index(i);
                    if (auto ident_list = visit_identifier_list())
                    {
                        if (next_if(")"))
                            continue;
                    }
                    index(i);
                    CR_RETURN_AST(nullptr);
                }
                else if (next_if("["))
                {
                    another->m_type = AST_direct_declarator::DD_BRACKET;
                    // '[', ['*'], ']'
                    if (next_if("]"))
                    {
                        continue;
                    }
                    if (next_if("*"))
                    {
                        another->m_str = "*";
                        if (next_if("]"))
                        {
                            continue;
                        }
                        CR_RETURN_AST(nullptr);
                    }
                    // '[', 'static', [type-qualifier-list], assignment-expression, ']'
                    if (next_if("static"))
                    {
                        auto i = index();
                        if (auto type_qual_list = visit_type_qualifier_list())
                        {
                            another->m_type_qual_list = type_qual_list;
                            i = index();
                        }
                        index(i);
                        if (auto assign_expr = visit_assignment_expression())
                        {
                            another->m_assign_expr = assign_expr;
                            if (next_if("]"))
                            {
                                continue;
                            }
                        }
                        index(i);
                        CR_RETURN_AST(nullptr);
                    }
                    auto i = index();
                    // '[', type-qualifier-list, ['*'], ']'
                    // '[', type-qualifier-list, ['static'], assignment-expression, ']'
                    if (auto type_qual_list = visit_type_qualifier_list())
                    {
                        another->m_type_qual_list = type_qual_list;
                        if (next_if("*"))
                        {
                            another->m_str = "*";
                            if (next_if("]"))
                            {
                                continue;
                            }
                        }
                        else
                        {
                            if (next_if("static"))
                            {
                                another->m_str = "static";
                            }
                            if (auto assign_expr = visit_assignment_expression())
                            {
                                another->m_assign_expr = assign_expr;
                                if (next_if("]"))
                                {
                                    continue;
                                }
                            }
                        }
                        continue;
                    }
                    index(i);
                    // '[', assignment-expression, ']'
                    if (auto assign_expr = visit_assignment_expression())
                    {
                        another->m_assign_expr = assign_expr;
                        if (next_if("]"))
                        {
                            continue;
                        }
                    }
                    index(i);
                }
                CR_RETURN_AST(another->m_child);
            }
            CR_RETURN_AST(dir_declor);
        }
        CR_RETURN_AST(nullptr);
    }

    // identifier-list = identifier, {',', identifier};
    inline s_p<AST_identifier_list> CParser::visit_identifier_list()
    {
        CR_SHOW_STATUS();
        auto ident_list = m_s<AST_identifier_list>();
        if (auto ident = visit_identifier())
        {
            ident_list->push_back(ident);
            while (next_if(","))
            {
                ident = visit_identifier();
                if (!ident)
                    CR_RETURN_AST(nullptr);

                ident_list->push_back(ident);
            }
            CR_RETURN_AST(ident_list);
        }
        CR_RETURN_AST(nullptr);
    }

    inline s_p<AST_identifier> CParser::visit_identifier(bool tag)
    {
        CR_SHOW_STATUS();
        auto ident = m_s<AST_identifier>();
        if (type() == TK_IDENTIFIER)
        {
            if (tag || m_typedef_names.count(str()) == 0)
            {
                ident->m_str = str();
                next();
                CR_RETURN_AST(ident);
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // initializer-list = designative-initializer, {',', designative-initializer};
    inline s_p<AST_initializer_list> CParser::visit_initializer_list()
    {
        CR_SHOW_STATUS();
        auto init_list = m_s<AST_initializer_list>();
        auto i = index();
        if (auto design_init = visit_designative_initializer())
        {
            init_list->push_back(design_init);
            while (next_if(","))
            {
                design_init = visit_designative_initializer();
                if (!design_init)
                    CR_RETURN_AST(nullptr);

                init_list->push_back(design_init);
            }
            CR_RETURN_AST(init_list);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // designative-initializer = [designation], initializer;
    inline s_p<AST_designative_initializer> CParser::visit_designative_initializer()
    {
        CR_SHOW_STATUS();
        auto design_init = m_s<AST_designative_initializer>();
        auto i = index();
        if (auto design = visit_designation())
        {
            design_init->m_design = design;
            i = index();
        }
        index(i);
        if (auto init = visit_initializer())
        {
            design_init->m_init = init;
            CR_RETURN_AST(design_init);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // initializer = '{', initializer-list, [','], '}'
    //             | assignment-expression;
    inline s_p<AST_initializer> CParser::visit_initializer()
    {
        CR_SHOW_STATUS();
        auto init = m_s<AST_initializer>();
        if (next_if("{"))
        {
            if (auto init_list = visit_initializer_list())
            {
                init->m_init_list = init_list;
                next_if(",");
                if (next_if("}"))
                {
                    CR_RETURN_AST(init);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        auto i = index();
        if (auto assign_expr = visit_assignment_expression())
        {
            init->m_assign_expr = assign_expr;
            CR_RETURN_AST(init);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // constant-expression = conditional-expression;  (* with constraints *)
    inline s_p<AST_constant_expression> CParser::visit_constant_expression()
    {
        CR_SHOW_STATUS();
        auto const_expr = m_s<AST_constant_expression>();
        auto i = index();
        if (auto cond_expr = visit_conditional_expression())
        {
            const_expr->m_cond_expr = cond_expr;
            CR_RETURN_AST(const_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // atomic-type-specifier = '_Atomic', '(', type-name, ')';
    inline s_p<AST_atomic_type_specifier> CParser::visit_atomic_type_specifier()
    {
        CR_SHOW_STATUS();
        auto atomic = m_s<AST_atomic_type_specifier>();
        if (next_if("_Atomic") && next_if("("))
        {
            if (auto type_name = visit_type_name())
            {
                atomic->m_type_name = type_name;
                if (next_if(")"))
                {
                    CR_RETURN_AST(atomic);
                }
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // struct-or-union-specifier = struct-or-union, {attribute}, '{', struct-declaration-list, '}'
    //                           | struct-or-union, {attribute}, identifier, ['{', struct-declaration-list, '}'];
    // struct-or-union = 'struct'
    //                 | 'union';
    inline s_p<AST_struct_or_union_specifier> CParser::visit_struct_or_union_specifier()
    {
        CR_SHOW_STATUS();
        auto su_spec = m_s<AST_struct_or_union_specifier>();
        if (str() == "struct" || str() == "union")
        {
            su_spec->m_is_union = (str() == "union");
            next();
            auto i = index();
            while (scan_attribute(su_spec->m_attrs))
            {
                i = index();
            }
            index(i);
            if (auto ident = visit_identifier(true))
            {
                su_spec->m_ident = ident;
                i = index();
            }
            index(i);
            if (next_if("{"))
            {
                if (auto struct_decl_list = visit_struct_declaration_list())
                {
                    su_spec->m_struct_decl_list = struct_decl_list;
                    if (next_if("}"))
                    {
                        CR_RETURN_AST(su_spec);
                    }
                }
            }
            else
            {
                if (su_spec->m_ident)
                    CR_RETURN_AST(su_spec);
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // struct-declaration-list = struct-declaration, {struct-declaration};
    inline s_p<AST_struct_declaration_list> CParser::visit_struct_declaration_list()
    {
        CR_SHOW_STATUS();
        auto struct_decl_list = m_s<AST_struct_declaration_list>();
        auto i = index();
        if (auto struct_decl = visit_struct_declaration())
        {
            do
            {
                struct_decl_list->push_back(struct_decl);
                i = index();
                struct_decl = visit_struct_declaration();
            } while (struct_decl);
            index(i);
            CR_RETURN_AST(struct_decl_list);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // struct-declaration = specifier-qualifier-list, ';'     (* for anonymous struct/union *)
    //                    | specifier-qualifier-list, struct-declarator-list, {attribute}, ';'
    //                    | static-assert-declaration
    //                    | ';';
    inline s_p<AST_struct_declaration> CParser::visit_struct_declaration()
    {
        CR_SHOW_STATUS();
        auto struct_decl = m_s<AST_struct_declaration>();
        if (next_if(";"))
        {
            CR_RETURN_AST(struct_decl);
        }
        auto i = index();
        if (auto spec_qual_list = visit_specifier_qualifier_list())
        {
            struct_decl->m_spec_qual_list = spec_qual_list;
            if (next_if(";"))
            {
                CR_RETURN_AST(struct_decl);
            }
            if (auto struct_declor_list = visit_struct_declarator_list())
            {
                struct_decl->m_struct_declor_list = struct_declor_list;
                i = index();
                if (scan_attribute(struct_decl->m_attrs))
                {
                    i = index();
                }
                index(i);
                if (next_if(";"))
                {
                    CR_RETURN_AST(struct_decl);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        index(i);
        if (auto static_assert_decl = visit_static_assert_declaration())
        {
            struct_decl->m_static_assert_decl = static_assert_decl;
            CR_RETURN_AST(struct_decl);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // enum-specifier = 'enum', '{', enumerator-list, [','], '}'
    //                | 'enum', identifier, ['{', enumerator-list, [','], '}'];
    inline s_p<AST_enum_specifier> CParser::visit_enum_specifier()
    {
        CR_SHOW_STATUS();
        auto enum_spec = m_s<AST_enum_specifier>();
        if (next_if("enum"))
        {
            if (auto ident = visit_identifier(true))
            {
                enum_spec->m_ident = ident;
            }
            else
            {
                if (str() != "{")
                {
                    CR_RETURN_AST(nullptr);
                }
            }
            if (next_if("{"))
            {
                if (auto enum_list = visit_enumerator_list())
                {
                    enum_spec->m_enum_list = enum_list;
                    next_if(",");
                    if (next_if("}"))
                    {
                        CR_RETURN_AST(enum_spec);
                    }
                }
            }
            else
            {
                CR_RETURN_AST(enum_spec);
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // enumerator-list = enumerator, {',', enumerator};
    inline s_p<AST_enumerator_list> CParser::visit_enumerator_list()
    {
        CR_SHOW_STATUS();
        auto enum_list = m_s<AST_enumerator_list>();
        if (auto enumor = visit_enumerator())
        {
            enum_list->push_back(enumor);
            while (next_if(","))
            {
                if (str() == "}")
                {
                    prev();
                    break;
                }
                enumor = visit_enumerator();
                if (!enumor)
                    CR_RETURN_AST(nullptr);

                enum_list->push_back(enumor);
            }
            CR_RETURN_AST(enum_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // enumerator = enumeration-constant, ['=', constant-expression];
    // enumeration-constant = identifier;
    inline s_p<AST_enumerator> CParser::visit_enumerator()
    {
        CR_SHOW_STATUS();
        auto enumor = m_s<AST_enumerator>();
        if (auto ident = visit_identifier())
        {
            m_enum_constant_names.insert(ident->m_str);
            enumor->m_ident = ident;
            if (next_if("="))
            {
                if (auto const_expr = visit_constant_expression())
                {
                    enumor->m_const_expr = const_expr;
                    CR_RETURN_AST(enumor);
                }
                CR_RETURN_AST(nullptr);
            }
            CR_RETURN_AST(enumor);
        }
        CR_RETURN_AST(nullptr);
    }

    // type-name = specifier-qualifier-list, [abstract-declarator];
    inline s_p<AST_type_name> CParser::visit_type_name()
    {
        CR_SHOW_STATUS();
        auto type_name = m_s<AST_type_name>();
        auto i = index();
        if (auto spec_qual_list = visit_specifier_qualifier_list())
        {
            type_name->m_spec_qual_list = spec_qual_list;
            i = index();
            if (auto abst_declor = visit_abstract_declarator())
            {
                type_name->m_abst_declor = abst_declor;
                i = index();
            }
            index(i);
            CR_RETURN_AST(type_name);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // specifier-qualifier-list = specifier-qualifier, {specifier-qualifier};
    inline s_p<AST_specifier_qualifier_list> CParser::visit_specifier_qualifier_list()
    {
        CR_SHOW_STATUS();
        auto spec_qual_list = m_s<AST_specifier_qualifier_list>();
        if (auto spec_qual = visit_specifier_qualifier())
        {
            auto i = index();
            do
            {
                spec_qual_list->push_back(spec_qual);
                i = index();
                spec_qual = visit_specifier_qualifier();
            } while (spec_qual);
            index(i);
            CR_RETURN_AST(spec_qual_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // specifier-qualifier = type-specifier | type-qualifier;
    inline s_p<AST_specifier_qualifier> CParser::visit_specifier_qualifier()
    {
        CR_SHOW_STATUS();
        auto spec_qual = m_s<AST_specifier_qualifier>();
        auto i = index();
        if (auto type_spec = visit_type_specifier())
        {
            spec_qual->m_type_spec = type_spec;
            CR_RETURN_AST(spec_qual);
        }
        index(i);
        if (auto type_qual = visit_type_qualifier())
        {
            spec_qual->m_type_qual = type_qual;
            CR_RETURN_AST(spec_qual);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // abstract-declarator = {function-attribute}, pointer, {function-attribute}, [direct-abstract-declarator]
    //                     | {function-attribute}, direct-abstract-declarator;
    inline s_p<AST_abstract_declarator> CParser::visit_abstract_declarator()
    {
        CR_SHOW_STATUS();
        auto abst_declor = m_s<AST_abstract_declarator>();
        auto i = index();
        while (scan_function_attribute(abst_declor->m_attrs))
        {
            i = index();
        }
        index(i);
        if (auto ptr = visit_pointer())
        {
            abst_declor->m_ptr = ptr;
            i = index();
            while (scan_function_attribute(abst_declor->m_attrs))
            {
                i = index();
            }
            index(i);
            if (auto dir_abst_declor = visit_direct_abstract_declarator())
            {
                abst_declor->m_dir_abst_declor = dir_abst_declor;
                i = index();
            }
            index(i);
            CR_RETURN_AST(abst_declor);
        }
        index(i);
        if (auto dir_abst_declor = visit_direct_abstract_declarator())
        {
            abst_declor->m_dir_abst_declor = dir_abst_declor;
            CR_RETURN_AST(abst_declor);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // direct-abstract-declarator = '(', abstract-declarator, ')'
    //                            | '(', parameter-type-list, ')'
    //                            | '(', ')'
    //                            | '[', ['*'], ']'
    //                            | '[', 'static', [type-qualifier-list], assignment-expression, ']'
    //                            | '[', type-qualifier-list, [['static'], assignment-expression], ']'
    //                            | '[', assignment-expression, ']'
    //                            | direct-abstract-declarator, '[', ['*'], ']'
    //                            | direct-abstract-declarator, '[', 'static', [type-qualifier-list], assignment-expression, ']'
    //                            | direct-abstract-declarator, '[', type-qualifier-list, [['static'], assignment-expression], ']'
    //                            | direct-abstract-declarator, '[', assignment-expression, ']'
    //                            | direct-abstract-declarator, '(', parameter-type-list, ')'
    //                            | direct-abstract-declarator, '(', ')';
    inline s_p<AST_direct_abstract_declarator> CParser::visit_direct_abstract_declarator()
    {
        CR_SHOW_STATUS();
        auto dir_abst_declor = m_s<AST_direct_abstract_declarator>();
        bool ok = false;
        if (next_if("("))
        {
            auto i = index();
            if (next_if(")"))
            {
                // '(', ')', ...
                ok = true;
            }
            else if (auto param_type_list = visit_parameter_type_list())
            {
                // '(', parameter-type-list, ')', ...
                dir_abst_declor->m_param_type_list = param_type_list;
                ok = next_if(")");
            }
            else
            {
                // '(', abstract-declarator, ')', ...
                index(i);
                if (auto abst_declor = visit_abstract_declarator())
                {
                    dir_abst_declor->m_abst_declor = abst_declor;
                    ok = next_if(")");
                }
            }
        }
        else if (next_if("["))
        {
            // '[', ['*'], ']', ...
            if (next_if("]"))
            {
                ok = true;
            }
            else if (next_if("*"))
            {
                dir_abst_declor->m_str = "*";
                ok = next_if("]");
            }
            else if (next_if("static"))
            {
                // '[', 'static', [type-qualifier-list], assignment-expression, ']', ...
                auto i = index();
                if (auto type_qual_list = visit_type_qualifier_list())
                {
                    dir_abst_declor->m_type_qual_list = type_qual_list;
                    i = index();
                }
                index(i);
                if (auto assign_expr = visit_assignment_expression())
                {
                    dir_abst_declor->m_assign_expr = assign_expr;
                    ok = next_if("]");
                }
            }
            else
            {
                // '[', type-qualifier-list, [['static'], assignment-expression], ']', ...
                auto i = index();
                if (auto type_qual_list = visit_type_qualifier_list())
                {
                    dir_abst_declor->m_type_qual_list = type_qual_list;
                    if (next_if("]"))
                    {
                        ok = true;
                    }
                    else if (next_if("static"))
                    {
                        dir_abst_declor->m_str = "static";
                        if (auto assign_expr = visit_assignment_expression())
                        {
                            dir_abst_declor->m_assign_expr = assign_expr;
                            ok = next_if("]");
                        }
                    }
                    else if (auto assign_expr = visit_assignment_expression())
                    {
                        dir_abst_declor->m_assign_expr = assign_expr;
                        ok = next_if("]");
                    }
                }
                else
                {
                    // '[', assignment-expression, ']', ...
                    index(i);
                    if (auto assign_expr = visit_assignment_expression())
                    {
                        dir_abst_declor->m_assign_expr = assign_expr;
                        ok = next_if("]");
                    }
                }
            }
        }
        if (ok)
        {
            while (str() == "(" || str() == "[")
            {
                auto another = m_s<AST_direct_abstract_declarator>();
                another->m_child = dir_abst_declor;
                dir_abst_declor = another;

                if (next_if("("))
                {
                    if (next_if(")"))
                    {
                        // '(', ')';
                        continue;
                    }
                    else if (auto param_type_list = visit_parameter_type_list())
                    {
                        // '(', parameter-type-list, ')'
                        another->m_param_type_list = param_type_list;
                        if (next_if(")"))
                        {
                            continue;
                        }
                    }
                }
                else if (next_if("["))
                {
                    // '[', ['*'], ']'
                    if (next_if("]"))
                    {
                        continue;
                    }
                    else if (next_if("*"))
                    {
                        another->m_str = "*";
                        if (next_if("]"))
                        {
                            continue;
                        }
                    }
                    else if (next_if("static"))
                    {
                        // '[', 'static', [type-qualifier-list], assignment-expression, ']'
                        another->m_str = "static";
                        auto i = index();
                        if (auto type_qual_list = visit_type_qualifier_list())
                        {
                            another->m_type_qual_list = type_qual_list;
                            i = index();
                        }
                        index(i);
                        if (auto assign_expr = visit_assignment_expression())
                        {
                            another->m_assign_expr = assign_expr;
                            if (next_if("]"))
                            {
                                continue;
                            }
                        }
                    }
                    else
                    {
                        auto i = index();
                        // '[', type-qualifier-list, [['static'], assignment-expression], ']'
                        if (auto type_qual_list = visit_type_qualifier_list())
                        {
                            another->m_type_qual_list = type_qual_list;
                            if (next_if("]"))
                            {
                                continue;
                            }
                            else
                            {
                                if (next_if("static"))
                                {
                                    another->m_str = "static";
                                }
                                if (auto assign_expr = visit_assignment_expression())
                                {
                                    another->m_assign_expr = assign_expr;
                                    if (next_if("]"))
                                    {
                                        continue;
                                    }
                                }
                            }
                        }
                        // '[', assignment-expression, ']'
                        index(i);
                        if (auto assign_expr = visit_assignment_expression())
                        {
                            another->m_assign_expr = assign_expr;
                            if (next_if("]"))
                            {
                                continue;
                            }
                        }
                    }
                }
                CR_RETURN_AST(nullptr);
            }
            CR_RETURN_AST(dir_abst_declor);
        }
        CR_RETURN_AST(nullptr);
    }

    // struct-declarator-list = struct-declarator, {',', struct-declarator};
    inline s_p<AST_struct_declarator_list> CParser::visit_struct_declarator_list()
    {
        CR_SHOW_STATUS();
        auto struct_declor_list = m_s<AST_struct_declarator_list>();
        if (auto struct_declor = visit_struct_declarator())
        {
            struct_declor_list->push_back(struct_declor);
            while (next_if(","))
            {
                struct_declor = visit_struct_declarator();
                if (!struct_declor)
                    CR_RETURN_AST(nullptr);

                struct_declor_list->push_back(struct_declor);
            }
            CR_RETURN_AST(struct_declor_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // type-qualifier-list = type-qualifier, {type-qualifier};
    inline s_p<AST_type_qualifier_list> CParser::visit_type_qualifier_list()
    {
        CR_SHOW_STATUS();
        auto type_qual_list = m_s<AST_type_qualifier_list>();
        if (auto type_qual = visit_type_qualifier())
        {
            auto i = index();
            do
            {
                type_qual_list->push_back(type_qual);
                i = index();
                type_qual = visit_type_qualifier();
            } while (type_qual);
            index(i);
            CR_RETURN_AST(type_qual_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // parameter-type-list = parameter-list, [',', '...'];
    inline s_p<AST_parameter_type_list> CParser::visit_parameter_type_list()
    {
        CR_SHOW_STATUS();
        auto param_type_list = m_s<AST_parameter_type_list>();
        param_type_list->m_has_dots = false;
        if (auto param_list = visit_parameter_list())
        {
            param_type_list->m_param_list = param_list;
            if (next_if(","))
            {
                if (next_if("..."))
                {
                    param_type_list->m_has_dots = true;
                    CR_RETURN_AST(param_type_list);
                }
                CR_RETURN_AST(nullptr);
            }
            CR_RETURN_AST(param_type_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // struct-declarator = ':', constant-expression
    //                   | declarator, [':', constant-expression];
    inline s_p<AST_struct_declarator> CParser::visit_struct_declarator()
    {
        CR_SHOW_STATUS();
        auto struct_declor = m_s<AST_struct_declarator>();
        if (next_if(":"))
        {
            if (auto const_expr = visit_constant_expression())
            {
                struct_declor->m_const_expr = const_expr;
                CR_RETURN_AST(struct_declor);
            }
            CR_RETURN_AST(nullptr);
        }
        auto i = index();
        if (auto declor = visit_declarator())
        {
            struct_declor->m_declor = declor;
            if (next_if(":"))
            {
                if (auto const_expr = visit_constant_expression())
                {
                    struct_declor->m_const_expr = const_expr;
                    CR_RETURN_AST(struct_declor);
                }
                CR_RETURN_AST(nullptr);
            }
            CR_RETURN_AST(struct_declor);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // parameter-list = parameter-declaration, {',', parameter-declaration};
    inline s_p<AST_parameter_list> CParser::visit_parameter_list()
    {
        CR_SHOW_STATUS();
        auto param_list = m_s<AST_parameter_list>();
        if (auto param_decl = visit_parameter_declaration())
        {
            param_list->push_back(param_decl);
            while (next_if(","))
            {
                if (str() == "...")
                {
                    prev();
                    break;
                }
                param_decl = visit_parameter_declaration();
                if (!param_decl)
                    CR_RETURN_AST(nullptr);

                param_list->push_back(param_decl);
            }
            CR_RETURN_AST(param_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // parameter-declaration = declaration-specifiers, [declarator | abstract-declarator];
    inline s_p<AST_parameter_declaration> CParser::visit_parameter_declaration()
    {
        CR_SHOW_STATUS();
        auto param_decl = m_s<AST_parameter_declaration>();
        if (auto decl_specs = visit_declaration_specifiers())
        {
            param_decl->m_decl_specs = decl_specs;
            auto i = index();
            if (auto declor = visit_declarator())
            {
                param_decl->m_declor = declor;
                CR_RETURN_AST(param_decl);
            }
            index(i);
            if (auto abst_declor = visit_abstract_declarator())
            {
                param_decl->m_abst_declor = abst_declor;
                CR_RETURN_AST(param_decl);
            }
            index(i);
            CR_RETURN_AST(param_decl);
        }
        CR_RETURN_AST(nullptr);
    }

    // expression = assignment-expression, {',', assignment-expression};
    inline s_p<AST_expression> CParser::visit_expression()
    {
        CR_SHOW_STATUS();
        auto expr = m_s<AST_expression>();
        auto i = index();
        if (auto assign_expr = visit_assignment_expression())
        {
            expr->push_back(assign_expr);
            while (next_if(","))
            {
                assign_expr = visit_assignment_expression();
                if (!assign_expr)
                    CR_RETURN_AST(nullptr);

                expr->push_back(assign_expr);
            }
            CR_RETURN_AST(expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // assignment-expression = conditional-expression
    //                       | unary-expression, assignment-operator, assignment-expression;
    // assignment-operator = '=' 
    //                     | '*=' 
    //                     | '/=' 
    //                     | '%=' 
    //                     | '+=' 
    //                     | '-=' 
    //                     | '<<=' 
    //                     | '>>=' 
    //                     | '&=' 
    //                     | '^=' 
    //                     | '|=';
    inline s_p<AST_assignment_expression> CParser::visit_assignment_expression()
    {
        CR_SHOW_STATUS();
        auto assign_expr = m_s<AST_assignment_expression>();
        auto i = index();
        if (auto unary_expr = visit_unary_expression())
        {
            if (str() == "=" || str() == "*=" || str() == "/=" ||
                str() == "%=" || str() == "+=" || str() == "-=" ||
                str() == "<<=" || str() == ">>=" || str() == "&=" ||
                str() == "^=" || str() == "|=")
            {
                assign_expr->m_assign_op = str();
                next();
                if (auto another = visit_assignment_expression())
                {
                    assign_expr->m_unary_expr = unary_expr;
                    assign_expr->m_child = another;
                    CR_RETURN_AST(assign_expr);
                }
            }
        }
        index(i);
        if (auto cond_expr = visit_conditional_expression())
        {
            assign_expr->m_cond_expr = cond_expr;
            CR_RETURN_AST(assign_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // conditional-expression = logical-or-expression, ['?', expression, ':', conditional-expression];
    inline s_p<AST_conditional_expression> CParser::visit_conditional_expression()
    {
        CR_SHOW_STATUS();
        auto cond_expr = m_s<AST_conditional_expression>();
        auto i = index();
        if (auto log_or_expr = visit_logical_or_expression())
        {
            cond_expr->m_log_or_expr = log_or_expr;
            if (next_if("?"))
            {
                if (auto expr = visit_expression())
                {
                    cond_expr->m_expr = expr;
                    if (next_if(":"))
                    {
                        if (auto another = visit_conditional_expression())
                        {
                            cond_expr->m_child = another;
                            CR_RETURN_AST(cond_expr);
                        }
                    }
                }
                CR_RETURN_AST(nullptr);
            }
            CR_RETURN_AST(cond_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // logical-or-expression = logical-and-expression, {'||', logical-and-expression};
    inline s_p<AST_logical_or_expression> CParser::visit_logical_or_expression()
    {
        CR_SHOW_STATUS();
        auto log_or_expr = m_s<AST_logical_or_expression>();
        auto i = index();
        if (auto log_and_expr = visit_logical_and_expression())
        {
            log_or_expr->push_back(log_and_expr);
            while (next_if("||"))
            {
                log_and_expr = visit_logical_and_expression();
                if (!log_and_expr)
                    CR_RETURN_AST(nullptr);

                log_or_expr->push_back(log_and_expr);
            }
            CR_RETURN_AST(log_or_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // logical-and-expression = inclusive-or-expression, {'&&', inclusive-or-expression};
    inline s_p<AST_logical_and_expression> CParser::visit_logical_and_expression()
    {
        CR_SHOW_STATUS();
        auto log_and_expr = m_s<AST_logical_and_expression>();
        auto i = index();
        if (auto incl_or_expr = visit_inclusive_or_expression())
        {
            log_and_expr->push_back(incl_or_expr);
            while (next_if("&&"))
            {
                incl_or_expr = visit_inclusive_or_expression();
                if (!incl_or_expr)
                    CR_RETURN_AST(nullptr);

                log_and_expr->push_back(incl_or_expr);
            }
            CR_RETURN_AST(log_and_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // inclusive-or-expression = exclusive-or-expression, {'|', exclusive-or-expression};
    inline s_p<AST_inclusive_or_expression> CParser::visit_inclusive_or_expression()
    {
        CR_SHOW_STATUS();
        auto incl_or_expr = m_s<AST_inclusive_or_expression>();
        auto i = index();
        if (auto excl_or_expr = visit_exclusive_or_expression())
        {
            incl_or_expr->push_back(excl_or_expr);
            while (next_if("|"))
            {
                excl_or_expr = visit_exclusive_or_expression();
                if (!excl_or_expr)
                    CR_RETURN_AST(nullptr);

                incl_or_expr->push_back(excl_or_expr);
            }
            CR_RETURN_AST(incl_or_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // exclusive-or-expression = and-expression, {'^', and-expression};
    inline s_p<AST_exclusive_or_expression> CParser::visit_exclusive_or_expression()
    {
        CR_SHOW_STATUS();
        auto excl_or_expr = m_s<AST_exclusive_or_expression>();
        auto i = index();
        if (auto and_expr = visit_and_expression())
        {
            excl_or_expr->push_back(and_expr);
            while (next_if("^"))
            {
                and_expr = visit_and_expression();
                if (!and_expr)
                    CR_RETURN_AST(nullptr);

                excl_or_expr->push_back(and_expr);
            }
            CR_RETURN_AST(excl_or_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // and-expression = equality-expression, {'&', equality-expression};
    inline s_p<AST_and_expression> CParser::visit_and_expression()
    {
        CR_SHOW_STATUS();
        auto and_expr = m_s<AST_and_expression>();
        auto i = index();
        if (auto equal_expr = visit_equality_expression())
        {
            and_expr->push_back(equal_expr);
            while (next_if("&"))
            {
                equal_expr = visit_equality_expression();
                if (!equal_expr)
                    CR_RETURN_AST(nullptr);

                and_expr->push_back(equal_expr);
            }
            CR_RETURN_AST(and_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // equality-expression = relational-expression, {('==' | '!='), relational-expression};
    inline s_p<AST_equality_expression> CParser::visit_equality_expression()
    {
        CR_SHOW_STATUS();
        auto equal_expr = m_s<AST_equality_expression>();
        auto i = index();
        if (auto rel_expr = visit_relational_expression())
        {
            equal_expr->m_rel_expr = rel_expr;
            while (str() == "==" || str() == "!=")
            {
                equal_expr->m_op = str();
                next();

                rel_expr = visit_relational_expression();
                if (!rel_expr)
                    CR_RETURN_AST(nullptr);

                auto another = m_s<AST_equality_expression>();
                another->m_child = equal_expr;
                another->m_rel_expr = rel_expr;

                equal_expr = another;
            }
            CR_RETURN_AST(equal_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // relational-expression = shift-expression, {('<' | '>' | '<=' | '>='), shift-expression};
    inline s_p<AST_relational_expression> CParser::visit_relational_expression()
    {
        CR_SHOW_STATUS();
        auto rel_expr = m_s<AST_relational_expression>();
        auto i = index();
        if (auto shift_expr = visit_shift_expression())
        {
            rel_expr->m_shift_expr = shift_expr;
            while (str() == "<" || str() == ">" ||
                   str() == "<=" || str() == ">=")
            {
                rel_expr->m_op = str();
                next();

                shift_expr = visit_shift_expression();
                if (!shift_expr)
                    CR_RETURN_AST(nullptr);

                auto another = m_s<AST_relational_expression>();
                another->m_child = rel_expr;
                another->m_shift_expr = shift_expr;

                rel_expr = another;
            }
            CR_RETURN_AST(rel_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // shift-expression = additive-expression, {('<<' | '>>'), additive-expression};
    inline s_p<AST_shift_expression> CParser::visit_shift_expression()
    {
        CR_SHOW_STATUS();
        auto shift_expr = m_s<AST_shift_expression>();
        auto i = index();
        if (auto add_expr = visit_additive_expression())
        {
            shift_expr->m_add_expr = add_expr;
            while (str() == "<<" || str() == ">>")
            {
                shift_expr->m_op = str();
                next();

                add_expr = visit_additive_expression();
                if (!add_expr)
                    CR_RETURN_AST(nullptr);

                auto another = m_s<AST_shift_expression>();
                another->m_child = shift_expr;
                another->m_add_expr = add_expr;

                shift_expr = another;
            }
            CR_RETURN_AST(shift_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // additive-expression = multiplicative-expression, {('+' | '-'), multiplicative-expression};
    inline s_p<AST_additive_expression> CParser::visit_additive_expression()
    {
        CR_SHOW_STATUS();
        auto add_expr = m_s<AST_additive_expression>();
        auto i = index();
        if (auto mul_expr = visit_multiplicative_expression())
        {
            add_expr->m_mul_expr = mul_expr;
            while (str() == "+" || str() == "-")
            {
                add_expr->m_op = str();
                next();

                mul_expr = visit_multiplicative_expression();
                if (!mul_expr)
                    CR_RETURN_AST(nullptr);

                auto another = m_s<AST_additive_expression>();
                another->m_child = add_expr;
                another->m_mul_expr = mul_expr;
                add_expr = another;
            }
            CR_RETURN_AST(add_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // multiplicative-expression = cast-expression, {('*' | '/' | '%'), cast-expression};
    inline s_p<AST_multiplicative_expression> CParser::visit_multiplicative_expression()
    {
        CR_SHOW_STATUS();
        auto mul_expr = m_s<AST_multiplicative_expression>();
        auto i = index();
        if (auto cast_expr = visit_cast_expression())
        {
            mul_expr->m_cast_expr = cast_expr;
            while (str() == "*" || str() == "/" || str() == "%")
            {
                mul_expr->m_op = str();
                next();

                cast_expr = visit_cast_expression();
                if (!cast_expr)
                    CR_RETURN_AST(nullptr);

                auto another = m_s<AST_multiplicative_expression>();
                another->m_child = mul_expr;
                another->m_cast_expr = cast_expr;
                mul_expr = another;
            }
            CR_RETURN_AST(mul_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // cast-expression = unary-expression
    //                 | '(', type-name, ')', cast-expression;
    inline s_p<AST_cast_expression> CParser::visit_cast_expression()
    {
        CR_SHOW_STATUS();
        auto cast_expr = m_s<AST_cast_expression>();
        auto i = index();
        if (auto unary_expr = visit_unary_expression())
        {
            cast_expr->m_unary_expr = unary_expr;
            CR_RETURN_AST(cast_expr);
        }
        index(i);
        if (next_if("("))
        {
            if (auto type_name = visit_type_name())
            {
                cast_expr->m_type_name = type_name;
                if (next_if(")"))
                {
                    if (auto another = visit_cast_expression())
                    {
                        cast_expr->m_child = another;
                        CR_RETURN_AST(cast_expr);
                    }
                }
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // unary-expression = postfix-expression
    //                  | ('++' | '--'), unary-expression
    //                  | unary-operator, cast-expression
    //                  | 'sizeof', unary-expression
    //                  | 'sizeof', '(', type-name, ')'
    //                  | '_Alignof', '(', type-name, ')';
    // unary-operator = '&'
    //                | '*'
    //                | '+'
    //                | '-'
    //                | '~'
    //                | '!';
    inline s_p<AST_unary_expression> CParser::visit_unary_expression()
    {
        CR_SHOW_STATUS();
        auto unary_expr = m_s<AST_unary_expression>();
        if (str() == "++" || str() == "--")
        {
            unary_expr->m_op = str();
            next();
            if (auto another = visit_unary_expression())
            {
                unary_expr->m_child = another;
                CR_RETURN_AST(unary_expr);
            }
            CR_RETURN_AST(nullptr);
        }
        if (str() == "&" || str() == "*" || str() == "+" ||
            str() == "-" || str() == "~" || str() == "!")
        {
            unary_expr->m_op = str();
            next();
            if (auto cast_expr = visit_cast_expression())
            {
                unary_expr->m_cast_expr = cast_expr;
                CR_RETURN_AST(unary_expr);
            }
            CR_RETURN_AST(nullptr);
        }
        if (str() == "sizeof")
        {
            unary_expr->m_op = str();
            next();
            auto i = index();
            if (auto another = visit_unary_expression())
            {
                unary_expr->m_child = another;
                CR_RETURN_AST(unary_expr);
            }
            index(i);
            if (next_if("("))
            {
                if (auto type_name = visit_type_name())
                {
                    unary_expr->m_type_name = type_name;
                    if (next_if(")"))
                    {
                        CR_RETURN_AST(unary_expr);
                    }
                }
            }
            index(i);
            CR_RETURN_AST(nullptr);
        }
        if (str() == "_Alignof")
        {
            unary_expr->m_op = str();
            next();
            if (next_if("("))
            {
                if (auto type_name = visit_type_name())
                {
                    unary_expr->m_type_name = type_name;
                    if (next_if(")"))
                    {
                        CR_RETURN_AST(unary_expr);
                    }
                }
            }
            CR_RETURN_AST(nullptr);
        }
        auto i = index();
        if (auto postfix_expr = visit_postfix_expression())
        {
            unary_expr->m_postfix_expr = postfix_expr;
            CR_RETURN_AST(unary_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // postfix-expression = primary-expression
    //                    | postfix-expression, '[', expression, ']'
    //                    | postfix-expression, '(', [argument-expression-list], ')'
    //                    | postfix-expression, ('.' | '->'), identifier
    //                    | postfix-expression, ('++' | '--')
    //                    | '(', type-name, ')', '{', initializer-list, [','], '}';
    inline s_p<AST_postfix_expression> CParser::visit_postfix_expression()
    {
        CR_SHOW_STATUS();
        auto postfix_expr = m_s<AST_postfix_expression>();
        bool ok = false;
        auto i = index();
        if (auto prim_expr = visit_primary_expression())
        {
            postfix_expr->m_prim_expr = prim_expr;
            ok = true;
        }
        else
        {
            index(i);
            if (next_if("("))
            {
                if (auto type_name = visit_type_name())
                {
                    if (next_if(")") && next_if("{"))
                    {
                        if (auto init_list = visit_initializer_list())
                        {
                            next_if(",");
                            if (next_if("}"))
                            {
                                ok = true;
                            }
                        }
                    }
                }
            }
        }
        if (ok)
        {
            while (str() == "[" || str() == "(" || str() == "." ||
                   str() == "->" || str() == "++" || str() == "--")
            {
                auto another = m_s<AST_postfix_expression>();
                another->m_child = postfix_expr;
                postfix_expr = another;

                if (next_if("["))
                {
                    another->m_str = "[]";
                    // '[', expression, ']'
                    if (auto expr = visit_expression())
                    {
                        if (next_if("]"))
                            continue;
                    }
                }
                else if (next_if("("))
                {
                    // '(', [argument-expression-list], ')'
                    auto i = index();
                    if (auto arg_expr_list = visit_argument_expression_list())
                    {
                        another->m_arg_expr_list = arg_expr_list;
                        i = index();
                    }
                    index(i);
                    if (next_if(")"))
                        continue;
                }
                else if (str() == "." || str() == "->")
                {
                    // ('.' | '->'), identifier
                    another->m_str = str();
                    next();
                    if (auto ident = visit_identifier())
                    {
                        another->m_ident = ident;
                        continue;
                    }
                }
                else if (str() == "++" || str() == "--")
                {
                    // ('++' | '--')
                    another->m_str = str();
                    next();
                    continue;
                }
                CR_RETURN_AST(another->m_child);
            }
            CR_RETURN_AST(postfix_expr);
        }
        CR_RETURN_AST(nullptr);
    }

    // primary-expression = identifier
    //                    | constant
    //                    | string
    //                    | '(', expression, ')'
    //                    | generic-selection;
    // string = string-literal, {string-literal};
    inline s_p<AST_primary_expression> CParser::visit_primary_expression()
    {
        CR_SHOW_STATUS();
        auto prim_expr = m_s<AST_primary_expression>();
        if (next_if("("))
        {
            if (auto expr = visit_expression())
            {
                prim_expr->m_expr = expr;
                if (next_if(")"))
                {
                    prim_expr->m_type = AST_primary_expression::PE_PAREN;
                    CR_RETURN_AST(prim_expr);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        auto i = index();
        if (auto ident = visit_identifier())
        {
            prim_expr->m_type = AST_primary_expression::PE_IDENT;
            prim_expr->m_ident = ident;
            CR_RETURN_AST(prim_expr);
        }
        index(i);
        if (auto constant = visit_constant())
        {
            prim_expr->m_type = AST_primary_expression::PE_CONST;
            prim_expr->m_const = constant;
            CR_RETURN_AST(prim_expr);
        }
        index(i);
        if (type() == TK_STRING_LITERAL)
        {
            std::wstring unquoted_w;
            std::string unquoted;
            prim_expr->m_type = AST_primary_expression::PE_STR;
            prim_expr->m_fix = fix();
            if (prim_expr->m_fix == "L")
            {
                while (type() == TK_STRING_LITERAL)
                {
                    unquoted_w += wstr_unquote(str(), '"');
                    next();
                }
                prim_expr->m_str = wstr_quote(unquoted_w, '"');
            }
            else
            {
                while (type() == TK_STRING_LITERAL)
                {
                    unquoted += str_unquote(str(), '"');
                    next();
                }
                prim_expr->m_str = str_quote(unquoted, '"');
            }
            CR_RETURN_AST(prim_expr);
        }
        index(i);
        if (auto gen_sel = visit_generic_selection())
        {
            prim_expr->m_type = AST_primary_expression::PE_GEN;
            prim_expr->m_gen_sel = gen_sel;
            CR_RETURN_AST(prim_expr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // argument-expression-list = assignment-expression, {',', assignment-expression};
    inline s_p<AST_argument_expression_list> CParser::visit_argument_expression_list()
    {
        CR_SHOW_STATUS();
        auto arg_expr_list = m_s<AST_argument_expression_list>();
        auto i = index();
        if (auto assign_expr = visit_assignment_expression())
        {
            arg_expr_list->push_back(assign_expr);
            while (next_if(","))
            {
                assign_expr = visit_assignment_expression();
                if (!assign_expr)
                    CR_RETURN_AST(nullptr);

                arg_expr_list->push_back(assign_expr);
            }
            CR_RETURN_AST(arg_expr_list);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // constant = integer-constant
    //          | character-constant
    //          | floating-constant
    //          | enumeration-constant;
    inline s_p<AST_constant> CParser::visit_constant()
    {
        CR_SHOW_STATUS();
        auto constant = m_s<AST_constant>();
        switch (type())
        {
        case TK_INTEGER_LITERAL:
            constant->m_type = AST_constant::C_INTEGER;
            break;
        case TK_CHARACTER_LITERAL:
            constant->m_type = AST_constant::C_CHAR;
            break;
        case TK_FLOATING_LITERAL:
            constant->m_type = AST_constant::C_FLOATING;
            break;
        case TK_IDENTIFIER:
            if (m_enum_constant_names.count(str()) > 0)
                constant->m_type = AST_constant::C_ENUM;
            else
                CR_RETURN_AST(nullptr);
            break;
        default:
            CR_RETURN_AST(nullptr);
        }
        constant->m_str = str();
        constant->m_fix = fix();
        next();
        CR_RETURN_AST(constant);
    }

    // generic-selection = '_Generic', '(', assignment-expression, ',', generic-assoc-list, ')';
    inline s_p<AST_generic_selection> CParser::visit_generic_selection()
    {
        CR_SHOW_STATUS();
        auto gen_sel = m_s<AST_generic_selection>();
        if (next_if("_Generic") && next_if("("))
        {
            if (auto assign_expr = visit_assignment_expression())
            {
                gen_sel->m_assign_expr = assign_expr;
                if (next_if(","))
                {
                    if (auto gen_assoc_list = visit_generic_assoc_list())
                    {
                        gen_sel->m_gen_assoc_list = gen_assoc_list;
                        if (next_if(")"))
                        {
                            CR_RETURN_AST(gen_sel);
                        }
                    }
                }
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // generic-assoc-list = generic-association, {',', generic-association};
    inline s_p<AST_generic_assoc_list> CParser::visit_generic_assoc_list()
    {
        CR_SHOW_STATUS();
        auto gen_assoc_list = m_s<AST_generic_assoc_list>();
        if (auto gen_assoc = visit_generic_association())
        {
            gen_assoc_list->push_back(gen_assoc);
            while (next_if(","))
            {
                gen_assoc = visit_generic_association();
                if (!gen_assoc)
                    CR_RETURN_AST(nullptr);

                gen_assoc_list->push_back(gen_assoc);
            }
            CR_RETURN_AST(gen_assoc_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // generic-association = type-name, ':', assignment-expression
    //                     | 'default', ':', assignment-expression;
    inline s_p<AST_generic_association> CParser::visit_generic_association()
    {
        CR_SHOW_STATUS();
        auto gen_assoc = m_s<AST_generic_association>();
        if (next_if("default"))
        {
            if (next_if(":"))
            {
                if (auto assign_expr = visit_assignment_expression())
                {
                    gen_assoc->m_assign_expr = assign_expr;
                    CR_RETURN_AST(gen_assoc);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        auto i = index();
        if (auto type_name = visit_type_name())
        {
            if (next_if(":"))
            {
                if (auto assign_expr = visit_assignment_expression())
                {
                    gen_assoc->m_assign_expr = assign_expr;
                    CR_RETURN_AST(gen_assoc);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // designation = designator-list, '=';
    inline s_p<AST_designation> CParser::visit_designation()
    {
        CR_SHOW_STATUS();
        auto design = m_s<AST_designation>();
        auto i = index();
        if (auto design_list = visit_designator_list())
        {
            if (next_if("="))
            {
                design->m_design_list = design_list;
                CR_RETURN_AST(design);
            }
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // designator-list = designator, {designator};
    inline s_p<AST_designator_list> CParser::visit_designator_list()
    {
        CR_SHOW_STATUS();
        auto design_list = m_s<AST_designator_list>();
        if (auto design = visit_designator())
        {
            design_list->push_back(design);
            auto i = index();
            for (;;)
            {
                i = index();
                design = visit_designator();
                if (!design)
                    break;

                design_list->push_back(design);
            }
            index(i);
            CR_RETURN_AST(design_list);
        }
        CR_RETURN_AST(nullptr);
    }

    // designator = '[', constant-expression, ']'
    //            | '.', identifier;
    inline s_p<AST_designator> CParser::visit_designator()
    {
        CR_SHOW_STATUS();
        auto design = m_s<AST_designator>();
        if (next_if("["))
        {
            if (auto const_expr = visit_constant_expression())
            {
                design->m_const_expr = const_expr;
                if (next_if("]"))
                {
                    CR_RETURN_AST(design);
                }
            }
        }
        else if (next_if("."))
        {
            if (auto ident = visit_identifier())
            {
                design->m_ident = ident;
                CR_RETURN_AST(design);
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // statement = labeled-statement
    //           | compound-statement
    //           | expression-statement
    //           | selection-statement
    //           | iteration-statement
    //           | jump-statement
    //           | asm-statement;
    inline s_p<AST_statement> CParser::visit_statement()
    {
        CR_SHOW_STATUS();
        auto stmt = m_s<AST_statement>();
        auto i = index();
        if (auto label_stmt = visit_labeled_statement())
        {
            stmt->m_label_stmt = label_stmt;
            stmt->m_type = AST_statement::S_LABEL;
            CR_RETURN_AST(stmt);
        }
        index(i);
        if (auto comp_stmt = visit_compound_statement())
        {
            stmt->m_comp_stmt = comp_stmt;
            stmt->m_type = AST_statement::S_COMP;
            CR_RETURN_AST(stmt);
        }
        index(i);
        if (auto expr_stmt = visit_expression_statement())
        {
            stmt->m_expr_stmt = expr_stmt;
            stmt->m_type = AST_statement::S_EXPR;
            CR_RETURN_AST(stmt);
        }
        index(i);
        if (auto sel_stmt = visit_selection_statement())
        {
            stmt->m_sel_stmt = sel_stmt;
            stmt->m_type = AST_statement::S_SEL;
            CR_RETURN_AST(stmt);
        }
        index(i);
        if (auto iter_stmt = visit_iteration_statement())
        {
            stmt->m_iter_stmt = iter_stmt;
            stmt->m_type = AST_statement::S_ITER;
            CR_RETURN_AST(stmt);
        }
        index(i);
        if (auto jump_stmt = visit_jump_statement())
        {
            stmt->m_jump_stmt = jump_stmt;
            stmt->m_type = AST_statement::S_JUMP;
            CR_RETURN_AST(stmt);
        }
        index(i);
        if (auto asm_stmt = visit_asm_statement())
        {
            stmt->m_asm_stmt = asm_stmt;
            stmt->m_type = AST_statement::S_ASM;
            CR_RETURN_AST(stmt);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // labeled-statement = identifier, ':', statement
    //                   | 'case', constant-expression, ':', statement
    //                   | 'default', ':', statement;
    inline s_p<AST_labeled_statement> CParser::visit_labeled_statement()
    {
        CR_SHOW_STATUS();
        auto label_stmt = m_s<AST_labeled_statement>();
        if (next_if("case"))
        {
            if (auto const_expr = visit_constant_expression())
            {
                label_stmt->m_const_expr = const_expr;
                if (next_if(":"))
                {
                    if (auto stmt = visit_statement())
                    {
                        label_stmt->m_stmt = stmt;
                        CR_RETURN_AST(label_stmt);
                    }
                }
            }
            CR_RETURN_AST(nullptr);
        }
        if (next_if("default"))
        {
            if (next_if(":"))
            {
                if (auto stmt = visit_statement())
                {
                    label_stmt->m_stmt = stmt;
                    CR_RETURN_AST(label_stmt);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        auto i = index();
        if (auto ident = visit_identifier())
        {
            label_stmt->m_ident = ident;
            if (next_if(":"))
            {
                if (auto stmt = visit_statement())
                {
                    label_stmt->m_stmt = stmt;
                    CR_RETURN_AST(label_stmt);
                }
            }
            CR_RETURN_AST(nullptr);
        }
        index(i);
        CR_RETURN_AST(nullptr);
    }

    // expression-statement = [expression], ';';
    inline s_p<AST_expression_statement> CParser::visit_expression_statement()
    {
        CR_SHOW_STATUS();
        auto expr_stmt = m_s<AST_expression_statement>();
        auto i = index();
        if (auto expr = visit_expression())
        {
            expr_stmt->m_expr = expr;
            i = index();
        }
        index(i);
        if (next_if(";"))
        {
            CR_RETURN_AST(expr_stmt);
        }
        CR_RETURN_AST(nullptr);
    }

    // selection-statement = 'if', '(', expression, ')', statement, 'else', statement
    //                     | 'if', '(', expression, ')', statement
    //                     | 'switch', '(', expression, ')', statement;
    inline s_p<AST_selection_statement> CParser::visit_selection_statement()
    {
        CR_SHOW_STATUS();
        auto sel_stmt = m_s<AST_selection_statement>();
        if (next_if("if"))
        {
            if (next_if("("))
            {
                if (auto expr = visit_expression())
                {
                    sel_stmt->m_expr = expr;
                    if (next_if(")"))
                    {
                        if (auto stmt0 = visit_statement())
                        {
                            sel_stmt->m_stmt0 = stmt0;
                            if (next_if("else"))
                            {
                                if (auto stmt1 = visit_statement())
                                {
                                    sel_stmt->m_type = AST_selection_statement::SS_IF_ELSE;
                                    sel_stmt->m_stmt1 = stmt1;
                                    CR_RETURN_AST(sel_stmt);
                                }
                                CR_RETURN_AST(nullptr);
                            }
                            sel_stmt->m_type = AST_selection_statement::SS_IF;
                            CR_RETURN_AST(sel_stmt);
                        }
                    }
                }
            }
        }
        else if (next_if("switch"))
        {
            if (next_if("("))
            {
                if (auto expr = visit_expression())
                {
                    sel_stmt->m_expr = expr;
                    if (next_if(")"))
                    {
                        if (auto stmt0 = visit_statement())
                        {
                            sel_stmt->m_type = AST_selection_statement::SS_SWITCH;
                            sel_stmt->m_stmt0 = stmt0;
                            CR_RETURN_AST(sel_stmt);
                        }
                    }
                }
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // iteration-statement = 'while', '(', expression, ')', statement
    //                     | 'do', statement, 'while', '(', expression, ')', ';'
    //                     | 'for', '(', [expression], ';', [expression], ';', [expression], ')', statement
    //                     | 'for', '(', declaration, [expression], ';', [expression], ')', statement;
    inline s_p<AST_iteration_statement> CParser::visit_iteration_statement()
    {
        CR_SHOW_STATUS();
        auto iter_stmt = m_s<AST_iteration_statement>();
        if (next_if("while"))
        {
            iter_stmt->m_type = AST_iteration_statement::IS_WHILE;
            if (next_if("("))
            {
                if (auto expr0 = visit_expression())
                {
                    iter_stmt->m_expr0 = expr0;
                    if (next_if(")"))
                    {
                        if (auto stmt = visit_statement())
                        {
                            iter_stmt->m_stmt = stmt;
                            CR_RETURN_AST(iter_stmt);
                        }
                    }
                }
            }
        }
        else if (next_if("do"))
        {
            iter_stmt->m_type = AST_iteration_statement::IS_DO;
            if (auto stmt = visit_statement())
            {
                if (next_if("while") && next_if("("))
                {
                    if (auto expr0 = visit_expression())
                    {
                        iter_stmt->m_expr0 = expr0;
                        if (next_if(")"))
                        {
                            CR_RETURN_AST(iter_stmt);
                        }
                    }
                }
            }
        }
        else if (next_if("for"))
        {
            iter_stmt->m_type = AST_iteration_statement::IS_FOR;
            if (next_if("("))
            {
                auto i = index();
                if (auto expr0 = visit_expression())
                {
                    iter_stmt->m_expr0 = expr0;
                    if (!next_if(";"))
                        CR_RETURN_AST(nullptr);

                    i = index();
                }
                else
                {
                    index(i);
                    if (auto decl = visit_declaration())
                    {
                        iter_stmt->m_decl = decl;
                        i = index();
                    }
                }
                index(i);
                if (auto expr1 = visit_expression())
                {
                    iter_stmt->m_expr1 = expr1;
                    i = index();
                }
                index(i);
                if (next_if(";"))
                {
                    i = index();
                    if (auto expr2 = visit_expression())
                    {
                        iter_stmt->m_expr2 = expr2;
                        i = index();
                    }
                    index(i);
                    if (next_if(")"))
                    {
                        if (auto stmt = visit_statement())
                        {
                            iter_stmt->m_stmt = stmt;
                            CR_RETURN_AST(iter_stmt);
                        }
                    }
                }
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // jump-statement = 'goto', identifier, ';'
    //                | 'continue', ';'
    //                | 'break', ';'
    //                | 'return', [expression], ';';
    inline s_p<AST_jump_statement> CParser::visit_jump_statement()
    {
        CR_SHOW_STATUS();
        auto jump_stmt = m_s<AST_jump_statement>();
        if (next_if("goto"))
        {
            jump_stmt->m_type = AST_jump_statement::JS_GOTO;
            if (auto ident = visit_identifier())
            {
                jump_stmt->m_ident = ident;
                if (next_if(";"))
                {
                    CR_RETURN_AST(jump_stmt);
                }
            }
        }
        else if (next_if("continue"))
        {
            jump_stmt->m_type = AST_jump_statement::JS_CONT;
            if (next_if(";"))
                CR_RETURN_AST(jump_stmt);
        }
        else if (next_if("break"))
        {
            jump_stmt->m_type = AST_jump_statement::JS_BREAK;
            if (next_if(";"))
                CR_RETURN_AST(jump_stmt);
        }
        else if (next_if("return"))
        {
            jump_stmt->m_type = AST_jump_statement::JS_RETURN;
            if (next_if(";"))
                CR_RETURN_AST(jump_stmt);
            if (auto expr = visit_expression())
            {
                jump_stmt->m_expr = expr;
                if (next_if(";"))
                    CR_RETURN_AST(jump_stmt);
            }
        }
        CR_RETURN_AST(nullptr);
    }

    // asm-statement = '__asm__', ['__volatile__'], '(', ? ... ?, ')', ';'
    //               | '__asm', '{', ? ... ?, '}';
    inline s_p<AST_asm_statement> CParser::visit_asm_statement()
    {
        auto asm_stmt = m_s<AST_asm_statement>();
        if (next_if("__asm__"))
        {
            auto i = paren_close();
            index(i);
            if (next_if(";"))
            {
                CR_RETURN_AST(asm_stmt);
            }
            CR_RETURN_AST(nullptr);
        }
        if (next_if("__asm"))
        {
            auto i = brace_close();
            index(i);
            CR_RETURN_AST(asm_stmt);
        }
        CR_RETURN_AST(nullptr);
    }
} // namespace CodeReverse

/////////////////////////////////////////////////////////////////////////

#endif  // ndef CODEREVERSE_C_PARSER_HPP
