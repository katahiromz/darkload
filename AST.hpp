// AST.hpp --- CodeReverse A.S.T.
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#ifndef CODEREVERSE_AST_HPP
#define CODEREVERSE_AST_HPP

#include "Common.hpp"
#include <memory>   // for std::shared_ptr, std::make_shared

/////////////////////////////////////////////////////////////////////////

namespace CodeReverse
{
    #define s_p  std::shared_ptr
    #define m_s  std::make_shared
    typedef std::map<string_type, string_type> attributes_type;

    /////////////////////////////////////////////////////////////////////////

    struct AST_base;
        template <typename T_ITEM>
        struct AST_vector;

        struct AST_translation_unit;
        struct AST_external_declaration;
        struct AST_function_definition;
        struct AST_declaration;
        struct AST_declaration_specifiers;
        struct AST_declaration_specifier;
        struct AST_declarator;
        struct AST_function_attribute;
        struct AST_declaration_list;
        struct AST_compound_statement;
        struct AST_declaration_or_statement;
        struct AST_init_declarator_list;
        struct AST_init_declarator;
        struct AST_static_assert_declaration;
        struct AST_storage_class_specifier;
        struct AST_type_specifier;
        struct AST_type_qualifier;
        struct AST_function_specifier;
        struct AST_alignment_specifier;
        struct AST_pointer;
        struct AST_direct_declarator;
        struct AST_identifier_list;
        struct AST_identifier;
        struct AST_initializer_list;
        struct AST_designative_initializer;
        struct AST_initializer;
        struct AST_constant_expression;
        struct AST_atomic_type_specifier;
        struct AST_struct_or_union_specifier;
        struct AST_struct_declaration_list;
        struct AST_struct_declaration;
        struct AST_enum_specifier;
        struct AST_enumerator_list;
        struct AST_enumerator;
        struct AST_type_name;
        struct AST_specifier_qualifier_list;
        struct AST_specifier_qualifier;
        struct AST_abstract_declarator;
        struct AST_direct_abstract_declarator;
        struct AST_struct_declarator_list;
        struct AST_type_qualifier_list;
        struct AST_parameter_type_list;
        struct AST_struct_declarator;
        struct AST_parameter_list;
        struct AST_parameter_declaration;
        struct AST_expression;
        struct AST_assignment_expression;
        struct AST_conditional_expression;
        struct AST_logical_or_expression;
        struct AST_logical_and_expression;
        struct AST_inclusive_or_expression;
        struct AST_exclusive_or_expression;
        struct AST_and_expression;
        struct AST_equality_expression;
        struct AST_relational_expression;
        struct AST_shift_expression;
        struct AST_additive_expression;
        struct AST_multiplicative_expression;
        struct AST_cast_expression;
        struct AST_unary_expression;
        struct AST_postfix_expression;
        struct AST_primary_expression;
        struct AST_argument_expression_list;
        struct AST_constant;
        struct AST_generic_selection;
        struct AST_generic_assoc_list;
        struct AST_generic_association;
        struct AST_designation;
        struct AST_designator_list;
        struct AST_designator;
        struct AST_statement;
        struct AST_labeled_statement;
        struct AST_expression_statement;
        struct AST_selection_statement;
        struct AST_iteration_statement;
        struct AST_jump_statement;
        struct AST_asm_statement;

    /////////////////////////////////////////////////////////////////////////
    // AST_base --- base class of all A.S.T.

    struct AST_base
    {
        AST_base()
        {
            increment();
        }
        virtual ~AST_base()
        {
            decrement();
        }
        static int& alive_count()
        {
            static int s_count = 0;
            return s_count;
        }
        void increment()
        {
            ++alive_count();
        }
        void decrement()
        {
            --alive_count();
        }
    private:
        AST_base(const AST_base&);
        AST_base& operator=(const AST_base&);
    };

    /////////////////////////////////////////////////////////////////////////
    // AST_vector --- vector of A.S.T.

    template <typename T_ITEM>
    struct AST_vector : AST_base
    {
        std::vector<s_p<T_ITEM> > m_vec;

        bool empty() const
        {
            return size() == 0;
        }
        size_t size() const
        {
            return m_vec.size();
        }
        void resize(size_t siz)
        {
            m_vec.resize(siz);
        }
        s_p<T_ITEM>& operator[](size_t i)
        {
            return m_vec[i];
        }
        const s_p<T_ITEM>& operator[](size_t i) const
        {
            return m_vec[i];
        }
        void push_back(s_p<T_ITEM> ptr)
        {
            m_vec.push_back(ptr);
        }
    };

    /////////////////////////////////////////////////////////////////////////

    // translation-unit = {external-declaration};
    struct AST_translation_unit : AST_vector<AST_external_declaration>
    {
    };

    // external-declaration = function-definition
    //                      | declaration;
    struct AST_external_declaration : AST_base
    {
        s_p<AST_declaration> m_decl;
        s_p<AST_function_definition> m_func_def;
    };

    // function-definition = declaration-specifiers, {function-attribute}, declarator, [declaration-list], compound-statement;
    struct AST_function_definition : AST_base
    {
        s_p<AST_declaration_specifiers> m_decl_specs;
        s_p<AST_declarator> m_declor;
        s_p<AST_declaration_list> m_decl_list;
        s_p<AST_compound_statement> m_comp_stmt;
        attributes_type m_attrs;
    };

    // declaration = declaration-specifiers, [init-declarator-list], {function-attribute}, ';'
    //             | static-assert-declaration
    //             | ';';
    struct AST_declaration : AST_base
    {
        s_p<AST_declaration_specifiers> m_decl_specs;
        s_p<AST_init_declarator_list> m_init_declor_list;
        s_p<AST_static_assert_declaration> m_static_assert_decl;
        attributes_type m_attrs;
    };

    // declaration-specifiers = declaration-specifier, {declaration-specifier};
    struct AST_declaration_specifiers : AST_vector<AST_declaration_specifier>
    {
    };

    // declaration-specifier = storage-class-specifier
    //                       | type-specifier
    //                       | type-qualifier
    //                       | function-specifier
    //                       | alignment-specifier;
    struct AST_declaration_specifier : AST_base
    {
        enum {
            DS_STO_CLASS_SPEC,
            DS_TYPE_SPEC,
            DS_TYPE_QUAL,
            DS_FUNC_SPEC,
            DS_ALIGN_SPEC
        } m_type;
        s_p<AST_storage_class_specifier>  m_sto_class_spec;
        s_p<AST_type_specifier> m_type_spec;
        s_p<AST_type_qualifier> m_type_qual;
        s_p<AST_function_specifier> m_func_spec;
        s_p<AST_alignment_specifier> m_align_spec;
        bool is_typedef_name() const;
    };

    // declarator = {function-attribute}, [pointer], {function-attribute}, direct-declarator;
    struct AST_declarator : AST_base
    {
        s_p<AST_pointer> m_ptr;
        s_p<AST_direct_declarator> m_dir_declor;
        attributes_type m_attrs;
    };

    // function-attribute = '__cdecl'
    //                    | '__fastcall'
    //                    | '__stdcall'
    //                    | attribute;
    struct AST_function_attribute : AST_base
    {
        string_type m_str;
    };

    // declaration-list = declaration, {declaration};
    struct AST_declaration_list : AST_vector<AST_declaration>
    {
    };

    // compound-statement = '{', {declaration-or-statement}, '}';
    struct AST_compound_statement : AST_base
    {
        AST_vector<AST_declaration_or_statement> m_items;
    };

    // declaration-or-statement = declaration | statement;
    struct AST_declaration_or_statement : AST_base
    {
        s_p<AST_declaration> m_decl;
        s_p<AST_statement> m_stmt;
    };

    // init-declarator-list = init-declarator, {',', init-declarator};
    struct AST_init_declarator_list : AST_vector<AST_init_declarator>
    {
    };

    // init-declarator = declarator, ['=', initializer];
    struct AST_init_declarator : AST_base
    {
        s_p<AST_declarator> m_declor;
        s_p<AST_initializer> m_init;
    };

    // static-assert-declaration = '_Static_assert', '(', constant-expression, ',', string, ')', ';';
    struct AST_static_assert_declaration : AST_base
    {
        s_p<AST_constant_expression> m_const_expr;
        string_type m_str;
    };

    // storage-class-specifier = 'typedef'
    //                         | 'extern'
    //                         | 'static'
    //                         | '_Thread_local'
    //                         | 'auto'
    //                         | 'register'
    //                         | attribute;
    struct AST_storage_class_specifier : AST_base
    {
        string_type m_str;
        attributes_type m_attrs;
    };

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
    //                | atomic-type-specifier
    //                | struct-or-union-specifier
    //                | enum-specifier
    //                | typedef-name;
    struct AST_type_specifier : AST_base
    {
        enum {
            TS_ATOMIC,
            TS_STRUCT_OR_UNION,
            TS_ENUM,
            TS_TYPEDEF_NAME,
            TS_OTHER
        } m_type;
        s_p<AST_atomic_type_specifier> m_atom_type_spec;
        s_p<AST_struct_or_union_specifier> m_su_spec;
        s_p<AST_enum_specifier> m_enum_spec;
        string_type m_str;
    };

    // type-qualifier = 'const'
    //                | 'restrict'
    //                | 'volatile'
    //                | '_Atomic'
    //                | '__ptr64'
    //                | '__restrict__';
    struct AST_type_qualifier : AST_base
    {
        string_type m_str;
    };

    // function-specifier = 'inline'
    //                    | '_Noreturn'
    //                    | '__inline'
    //                    | '__inline__'
    //                    | '__forceinline'
    //                    | attribute;
    struct AST_function_specifier : AST_base
    {
        attributes_type m_attrs;
    };

    // alignment-specifier = '_Alignas', '(', type-name, ')'
    //                     | '_Alignas', '(', constant-expression, ')';
    struct AST_alignment_specifier : AST_base
    {
        s_p<AST_type_name> m_type_name;
        s_p<AST_constant_expression> m_const_expr;
    };

    // pointer = '*', [type-qualifier-list], [pointer];
    struct AST_pointer : AST_base
    {
        s_p<AST_type_qualifier_list> m_type_qual_list;
        s_p<AST_pointer> m_child;
    };

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
    struct AST_direct_declarator : AST_base
    {
        enum {
            DD_IDENT, DD_DECLOR, DD_BRACKET, DD_PAREN
        } m_type;
        string_type m_str;  // '', '*' or 'static'
        s_p<AST_identifier> m_ident;
        s_p<AST_declarator> m_declor;
        s_p<AST_direct_declarator> m_child;     // left associative
        s_p<AST_type_qualifier_list> m_type_qual_list;
        s_p<AST_assignment_expression> m_assign_expr;
        s_p<AST_parameter_type_list> m_param_type_list;
        s_p<AST_identifier_list> m_ident_list;
    };

    // identifier-list = identifier, {',', identifier};
    struct AST_identifier_list : AST_vector<AST_identifier>
    {
    };

    struct AST_identifier : AST_base
    {
        string_type m_str;
    };

    // initializer-list = designative-initializer, {',', designative-initializer};
    struct AST_initializer_list : AST_vector<AST_designative_initializer>
    {
    };

    // designative-initializer = [designation], initializer;
    struct AST_designative_initializer : AST_base
    {
        s_p<AST_designation> m_design;
        s_p<AST_initializer> m_init;
    };

    // initializer = '{', initializer-list, [','], '}'
    //             | assignment-expression;
    struct AST_initializer : AST_base
    {
        s_p<AST_initializer_list> m_init_list;
        s_p<AST_assignment_expression> m_assign_expr;
    };

    // constant-expression = conditional-expression;  (* with constraints *)
    struct AST_constant_expression : AST_base
    {
        s_p<AST_conditional_expression> m_cond_expr;
    };

    // atomic-type-specifier = '_Atomic', '(', type-name, ')';
    struct AST_atomic_type_specifier : AST_base
    {
        s_p<AST_type_name> m_type_name;
    };

    // struct-or-union-specifier = struct-or-union, {attribute}, '{', struct-declaration-list, '}'
    //                           | struct-or-union, {attribute}, identifier, ['{', struct-declaration-list, '}'];
    // struct-or-union = 'struct'
    //                 | 'union';
    struct AST_struct_or_union_specifier : AST_base
    {
        bool m_is_union;
        s_p<AST_identifier> m_ident;
        s_p<AST_struct_declaration_list> m_struct_decl_list;
        attributes_type m_attrs;
    };

    // struct-declaration-list = struct-declaration, {struct-declaration};
    struct AST_struct_declaration_list : AST_vector<AST_struct_declaration>
    {
    };

    // struct-declaration = specifier-qualifier-list, ';'     (* for anonymous struct/union *)
    //                    | specifier-qualifier-list, struct-declarator-list, {attribute}, ';'
    //                    | static-assert-declaration
    //                    | ';';
    struct AST_struct_declaration : AST_base
    {
        s_p<AST_specifier_qualifier_list> m_spec_qual_list;
        s_p<AST_struct_declarator_list> m_struct_declor_list;
        s_p<AST_static_assert_declaration> m_static_assert_decl;
        attributes_type m_attrs;
    };

    // enum-specifier = 'enum', '{', enumerator-list, [','], '}'
    //                | 'enum', identifier, ['{', enumerator-list, [','], '}'];
    struct AST_enum_specifier : AST_base
    {
        s_p<AST_enumerator_list> m_enum_list;
        s_p<AST_identifier> m_ident;
    };

    // enumerator-list = enumerator, {',', enumerator};
    struct AST_enumerator_list : AST_vector<AST_enumerator>
    {
    };

    // enumerator = enumeration-constant, ['=', constant-expression];
    // enumeration-constant = identifier;
    struct AST_enumerator : AST_base
    {
        s_p<AST_identifier> m_ident;
        s_p<AST_constant_expression> m_const_expr;
    };

    // type-name = specifier-qualifier-list, [abstract-declarator];
    struct AST_type_name : AST_base
    {
        s_p<AST_specifier_qualifier_list> m_spec_qual_list;
        s_p<AST_abstract_declarator> m_abst_declor;
    };

    // specifier-qualifier-list = specifier-qualifier, {specifier-qualifier};
    struct AST_specifier_qualifier_list : AST_vector<AST_specifier_qualifier>
    {
    };

    // specifier-qualifier = type-specifier | type-qualifier;
    struct AST_specifier_qualifier : AST_base
    {
        s_p<AST_type_specifier> m_type_spec;
        s_p<AST_type_qualifier> m_type_qual;
    };

    // abstract-declarator = {function-attribute}, pointer, {function-attribute}, [direct-abstract-declarator]
    //                     | {function-attribute}, direct-abstract-declarator;
    struct AST_abstract_declarator : AST_base
    {
        s_p<AST_pointer> m_ptr;
        s_p<AST_direct_abstract_declarator> m_dir_abst_declor;
        attributes_type m_attrs;
    };

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
    struct AST_direct_abstract_declarator : AST_base
    {
        string_type m_str; // '', '*' or 'static'
        s_p<AST_abstract_declarator> m_abst_declor;
        s_p<AST_parameter_type_list> m_param_type_list;
        s_p<AST_direct_abstract_declarator> m_child;    // left associative
        s_p<AST_type_qualifier_list> m_type_qual_list;
        s_p<AST_assignment_expression> m_assign_expr;
    };

    // struct-declarator-list = struct-declarator, {',', struct-declarator};
    struct AST_struct_declarator_list : AST_vector<AST_struct_declarator>
    {
    };

    // type-qualifier-list = type-qualifier, {type-qualifier};
    struct AST_type_qualifier_list : AST_vector<AST_type_qualifier>
    {
    };

    // parameter-type-list = parameter-list, [',', '...'];
    struct AST_parameter_type_list : AST_base
    {
        bool m_has_dots;
        s_p<AST_parameter_list> m_param_list;
    };

    // struct-declarator = ':', constant-expression
    //                   | declarator, [':', constant-expression];
    struct AST_struct_declarator : AST_base
    {
        s_p<AST_constant_expression> m_const_expr;
        s_p<AST_declarator> m_declor;
    };

    // parameter-list = parameter-declaration, {',', parameter-declaration};
    struct AST_parameter_list : AST_vector<AST_parameter_declaration>
    {
    };

    // parameter-declaration = declaration-specifiers, [declarator | abstract-declarator];
    struct AST_parameter_declaration : AST_base
    {
        s_p<AST_declaration_specifiers> m_decl_specs;
        s_p<AST_declarator> m_declor;
        s_p<AST_abstract_declarator> m_abst_declor;
    };

    // expression = assignment-expression, {',', assignment-expression};
    struct AST_expression : AST_vector<AST_assignment_expression>
    {
    };

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
    struct AST_assignment_expression : AST_base
    {
        s_p<AST_conditional_expression> m_cond_expr;
        s_p<AST_unary_expression> m_unary_expr;
        string_type m_assign_op;
        s_p<AST_assignment_expression> m_child;     // right associative
    };

    // conditional-expression = logical-or-expression, ['?', expression, ':', conditional-expression];
    struct AST_conditional_expression : AST_base
    {
        s_p<AST_logical_or_expression> m_log_or_expr;
        s_p<AST_expression> m_expr;
        s_p<AST_conditional_expression> m_child;    // right associative
    };

    // logical-or-expression = logical-and-expression, {'||', logical-and-expression};
    struct AST_logical_or_expression : AST_vector<AST_logical_and_expression>
    {
        // left associative
    };

    // logical-and-expression = inclusive-or-expression, {'&&', inclusive-or-expression};
    struct AST_logical_and_expression : AST_vector<AST_inclusive_or_expression>
    {
        // left associative
    };

    // inclusive-or-expression = exclusive-or-expression, {'|', exclusive-or-expression};
    struct AST_inclusive_or_expression : AST_vector<AST_exclusive_or_expression>
    {
        // left associative
    };

    // exclusive-or-expression = and-expression, {'^', and-expression};
    struct AST_exclusive_or_expression : AST_vector<AST_and_expression>
    {
        // left associative
    };

    // and-expression = equality-expression, {'&', equality-expression};
    struct AST_and_expression : AST_vector<AST_equality_expression>
    {
        // left associative
    };

    // equality-expression = relational-expression, {('==' | '!='), relational-expression};
    struct AST_equality_expression : AST_base
    {
        s_p<AST_equality_expression> m_child;   // left associative
        string_type m_op;   // '', '==' or '!='
        s_p<AST_relational_expression> m_rel_expr;
    };

    // relational-expression = shift-expression, {('<' | '>' | '<=' | '>='), shift-expression};
    struct AST_relational_expression : AST_base
    {
        s_p<AST_relational_expression> m_child; // left associative
        string_type m_op;   // '', '<', '>', '<=' or '>='
        s_p<AST_shift_expression> m_shift_expr;
    };

    // shift-expression = additive-expression, {('<<' | '>>'), additive-expression};
    struct AST_shift_expression : AST_base
    {
        s_p<AST_shift_expression> m_child;      // left associative
        string_type m_op;   // '', '<<' or '>>'
        s_p<AST_additive_expression> m_add_expr;
    };

    // additive-expression = multiplicative-expression, {('+' | '-'), multiplicative-expression};
    struct AST_additive_expression : AST_base
    {
        s_p<AST_additive_expression> m_child;   // left associative
        string_type m_op;   // '', '+' or '-'
        s_p<AST_multiplicative_expression> m_mul_expr;
    };

    // multiplicative-expression = cast-expression, {('*' | '/' | '%'), cast-expression};
    struct AST_multiplicative_expression : AST_base
    {
        s_p<AST_multiplicative_expression> m_child;     // left associative
        string_type m_op;   // '', '*', '/' or '%'
        s_p<AST_cast_expression> m_cast_expr;
    };

    // cast-expression = unary-expression
    //                 | '(', type-name, ')', cast-expression;
    struct AST_cast_expression : AST_base
    {
        s_p<AST_unary_expression> m_unary_expr;
        s_p<AST_type_name> m_type_name;
        s_p<AST_cast_expression> m_child;
    };

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
    struct AST_unary_expression : AST_base
    {
        string_type m_op;   // '', '++', '--', '&', '*', '+', '-', '~', '!', 'sizeof' or '_Alignof'
        s_p<AST_postfix_expression> m_postfix_expr;
        s_p<AST_unary_expression> m_child;
        s_p<AST_cast_expression> m_cast_expr;
        s_p<AST_type_name> m_type_name;
    };

    // postfix-expression = primary-expression
    //                    | postfix-expression, '[', expression, ']'
    //                    | postfix-expression, '(', [argument-expression-list], ')'
    //                    | postfix-expression, ('.' | '->'), identifier
    //                    | postfix-expression, ('++' | '--')
    //                    | '(', type-name, ')', '{', initializer-list, [','], '}';
    struct AST_postfix_expression : AST_base
    {
        s_p<AST_primary_expression> m_prim_expr;
        string_type m_str;   // '', '[]', '()', 'compound', '.', '->', '++' or '--'
        s_p<AST_postfix_expression> m_child;    // left associative
        s_p<AST_expression> m_expr;
        s_p<AST_argument_expression_list> m_arg_expr_list;
        s_p<AST_identifier> m_ident;
        s_p<AST_type_name> m_type_name;
        s_p<AST_initializer_list> m_init_list;  // for 'compound'
    };

    // primary-expression = identifier
    //                    | constant
    //                    | string
    //                    | '(', expression, ')'
    //                    | generic-selection;
    // string = string-literal
    //        | '__func__';
    struct AST_primary_expression : AST_base
    {
        enum {
            PE_IDENT, PE_CONST, PE_STR, PE_PAREN, PE_GEN
        } m_type;
        s_p<AST_identifier> m_ident;
        s_p<AST_constant> m_const;
        string_type m_str;  // "string-literal" or "__func__"
		string_type m_fix;
        s_p<AST_expression> m_expr;
        s_p<AST_generic_selection> m_gen_sel;
    };

    // argument-expression-list = assignment-expression, {',', assignment-expression};
    struct AST_argument_expression_list : AST_vector<AST_assignment_expression>
    {
    };

    // constant = integer-constant
    //          | character-constant
    //          | floating-constant
    //          | enumeration-constant;
    struct AST_constant : AST_base
    {
        enum {
            C_INTEGER, C_CHAR, C_FLOATING, C_ENUM
        } m_type;
        string_type m_str;
        string_type m_fix;
    };

    // generic-selection = '_Generic', '(', assignment-expression, ',', generic-assoc-list, ')';
    struct AST_generic_selection : AST_base
    {
        s_p<AST_assignment_expression> m_assign_expr;
        s_p<AST_generic_assoc_list> m_gen_assoc_list;
    };

    // generic-assoc-list = generic-association, {',', generic-association};
    struct AST_generic_assoc_list : AST_vector<AST_generic_association>
    {
    };

    // generic-association = type-name, ':', assignment-expression
    //                     | 'default', ':', assignment-expression;
    struct AST_generic_association : AST_base
    {
        s_p<AST_assignment_expression> m_assign_expr;
        s_p<AST_type_name> m_type_name;
    };

    // designation = designator-list, '=';
    struct AST_designation : AST_base
    {
        s_p<AST_designator_list> m_design_list;
    };

    // designator-list = designator, {designator};
    struct AST_designator_list : AST_vector<AST_designator>
    {
    };

    // designator = '[', constant-expression, ']'
    //            | '.', identifier;
    struct AST_designator : AST_base
    {
        s_p<AST_constant_expression> m_const_expr;
        s_p<AST_identifier> m_ident;
    };

    // statement = labeled-statement
    //           | compound-statement
    //           | expression-statement
    //           | selection-statement
    //           | iteration-statement
    //           | jump-statement
    //           | asm-statement;
    struct AST_statement : AST_base
    {
        enum {
            S_LABEL, S_COMP, S_EXPR, S_SEL, S_ITER, S_JUMP, S_ASM
        } m_type;
        s_p<AST_labeled_statement> m_label_stmt;
        s_p<AST_compound_statement> m_comp_stmt;
        s_p<AST_expression_statement> m_expr_stmt;
        s_p<AST_selection_statement> m_sel_stmt;
        s_p<AST_iteration_statement> m_iter_stmt;
        s_p<AST_jump_statement> m_jump_stmt;
        s_p<AST_asm_statement> m_asm_stmt;
    };

    // labeled-statement = identifier, ':', statement
    //                   | 'case', constant-expression, ':', statement
    //                   | 'default', ':', statement;
    struct AST_labeled_statement : AST_base
    {
        s_p<AST_identifier> m_ident;
        s_p<AST_constant_expression> m_const_expr;
        s_p<AST_statement> m_stmt;
    };

    // expression-statement = [expression], ';';
    struct AST_expression_statement : AST_base
    {
        s_p<AST_expression> m_expr;
    };

    // selection-statement = 'if', '(', expression, ')', statement, 'else', statement
    //                     | 'if', '(', expression, ')', statement
    //                     | 'switch', '(', expression, ')', statement;
    struct AST_selection_statement : AST_base
    {
        enum {
            SS_IF, SS_IF_ELSE, SS_SWITCH
        } m_type;
        s_p<AST_expression> m_expr;
        s_p<AST_statement> m_stmt0, m_stmt1;
    };

    // iteration-statement = 'while', '(', expression, ')', statement
    //                     | 'do', statement, 'while', '(', expression, ')', ';'
    //                     | 'for', '(', [expression], ';', [expression], ';', [expression], ')', statement
    //                     | 'for', '(', declaration, [expression], ';', [expression], ')', statement;
    struct AST_iteration_statement : AST_base
    {
        enum {
            IS_WHILE, IS_DO, IS_FOR
        } m_type;
        s_p<AST_expression> m_expr0, m_expr1, m_expr2;
        s_p<AST_declaration> m_decl;
        s_p<AST_statement> m_stmt;
    };

    // jump-statement = 'goto', identifier, ';'
    //                | 'continue', ';'
    //                | 'break', ';'
    //                | 'return', [expression], ';';
    struct AST_jump_statement : AST_base
    {
        enum {
            JS_GOTO, JS_CONT, JS_BREAK, JS_RETURN
        } m_type;
        s_p<AST_identifier> m_ident;
        s_p<AST_expression> m_expr;
    };

    // asm-statement = '__asm__', ['__volatile__'], '(', ? ... ?, ')', ';'
    //               | '__asm', '{', ? ... ?, '}';
    struct AST_asm_statement : AST_base
    {
        // not supported yet
    };

    /////////////////////////////////////////////////////////////////////////
    // inlines

    inline bool AST_declaration_specifier::is_typedef_name() const
    {
        return m_type == DS_TYPE_SPEC && m_type_spec &&
               m_type_spec->m_type == AST_type_specifier::TS_TYPEDEF_NAME;
    }
} // namespace CodeReverse

/////////////////////////////////////////////////////////////////////////

#endif  // ndef CODEREVERSE_AST_HPP
