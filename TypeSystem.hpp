// TypeInfo.hpp --- CodeReverse type system
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#ifndef CODEREVERSE_TYPE_SYSTEM_HPP
#define CODEREVERSE_TYPE_SYSTEM_HPP

#include "Common.hpp"

namespace CodeReverse
{
    struct Scope;
    struct Entity;
    struct Tag;
    struct Type;

    struct Scope
    {
        size_t m_scope_id;
        size_t m_parent_id;
        std::vector<size_t> m_child_scope_ids;
        std::vector<size_t> m_entity_ids;
        std::vector<size_t> m_tag_ids;
    };

    // NOTE: An entity is a variable, a typedef-name, a enum-value or a function.
    struct Entity
    {
        size_t m_entity_id;
        string_type m_entity_name;
        enum {
            E_VAR, E_TYPEDEF_NAME, E_ENUM_VALUE, E_FUNC
        } m_kind;
        size_t m_type_id;
        union
        {
            int m_int;
            unsigned int m_uint;
            __int64 m_int64;
            unsigned __int64 m_uint64;
        };
    };

    // NOTE: A tag is a struct, a union or an enum.
    struct Tag
    {
        string_type m_tag_name;
        enum {
            T_STRUCT, T_UNION, T_ENUM
        } m_type;
        size_t m_type_id;
        string_type m_type_name;
        size_t m_scope_id;
    };

    struct Label
    {
        string_type m_label_name;
        string_type m_func_name;
    };

    struct Type
    {
        size_t m_type_id;
        string_type m_type_name;
        size_t m_parent_type_id;
        enum Flags : unsigned long
        {
            // invalid
            T_INVALID       = 0,
            // void
            T_VOID          = 0x00000001,
            // integers
            T_BOOL          = 0x00000002,
            T_CHAR          = 0x00000004,
            T_SHORT         = 0x00000008,
            T_INT           = 0x00000010,
            T_LONG          = 0x00000020,
            T_LONGLONG      = 0x00000040 | T_INT,
            T_INT64         = T_LONGLONG,
            T_INTEGER       = 0x00000080,
            // signed/unsigned
            T_SIGNED        = 0x00000100,
            T_UNSIGNED      = 0x00000200,
            T_SIGN_MASK     = 0x00000300,
            // enum values
            T_ENUM_VALUE    = 0x00000400,
            // floatings
            T_FLOATING      = 0x00000800;
            T_FLOAT         = T_FLOATING,
            T_DOUBLE        = 0x00001000 | T_FLOATING,
            T_FLOAT80       = 0x00001800 | T_FLOATING,
            T_FLOAT128      = 0x00002000 | T_FLOATING,
            T_FLOATING_MASK = T_FLOAT | T_DOUBLE | T_FLOAT80 | T_FLOAT128,
            // complex, imaginary
            T_COMPLEX       = 0x00004000,
            T_IMAGINARY     = 0x00008000,
            // array, pointer, alias
            T_ARRAY         = 0x00010000,
            T_POINTER       = 0x00020000,
            T_ALIAS         = 0x00040000,
            // const, volatile, etc.
            T_CONST         = 0x00080000,
            T_VOLATILE      = 0x00100000,
            T_ATOMIC        = 0x00200000,
            T_THREAD_LOCAL  = 0x00400000,
            T_EXTERN        = 0x00800000,
            T_STATIC        = 0x01000000,
            T_RESTRICT      = 0x02000000,
            // tags
            T_TAG           = 0x04000000,
            T_STRUCT        = T_TAG,
            T_UNION         = 0x08000000 | T_TAG,
            T_ENUM          = 0x10000000 | T_TAG,
            T_TAG_MASK      = T_STRUCT | T_UNION | T_ENUM,
            // function
            T_FUNC          = 0x20000000,
            T_FUNC_PTR      = T_FUNC | T_POINTER,
            // calling conventions
            T_CDECL         = T_FUNC,
            T_FASTCALL      = 0x40000000 | T_FUNC,
            T_STDCALL       = 0x80000000 | T_FUNC,
            T_CALL_MASK     = T_CDECL | T_FASTCALL | T_STDCALL,
        };
        unsigned long m_flags;
        size_t m_sizeof;
        size_t m_countof;
        size_t m_alignof;

        std::vector<size_t> m_type_ids;                     // for T_FUNC or T_TAG
        // NOTE: If T_ENUM, m_type_ids becomes enum values.

        std::vector<string_type> m_type_names;              // for T_FUNC or T_TAG
        std::vector<string_type> m_member_names;            // for T_FUNC or T_TAG
        size_t m_scope_id;
    };
} // namespace CodeReverse

#endif  // ndef CODEREVERSE_TYPE_SYSTEM_HPP
