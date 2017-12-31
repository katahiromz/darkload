// TypeInfo.hpp --- CodeReverse type system
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#ifndef CODEREVERSE_TYPE_SYSTEM_HPP
#define CODEREVERSE_TYPE_SYSTEM_HPP

#include "Common.hpp"
#include <vector>
#include <set>

/////////////////////////////////////////////////////////////////////////

// NOTE: A tag is a struct, a union or an enum.
// NOTE: An entity is an enum-value, a function, a typedef-name or a variable.

namespace CodeReverse
{
    typedef unsigned long TypeFlagsType;

    typedef size_t ID;
        typedef ID EntityID;    // for the index in LogEntity::all()
        typedef ID FuncID;      // for the index in LogFunc::all()
        typedef ID VarID;       // for the index in LogVar::all()
        typedef ID TypeID;      // for the index in LogType::all()
        typedef ID MacroID;     // for the index in LogMacro::all()
        typedef ID ScopeID;     // for the index in LogScope::all()
        typedef ID TagID;       // for the index in LogTag::all()

    typedef std::unordered_map<string_type, ID> name2id_type;

    inline ID invalid_id(void)
    {
        return ID(-1);
    }

    struct Value
    {
        union
        {
            unsigned __int64        m_uint64 = 0;
            __int64                 m_int64;
            unsigned int            m_uint;
            int                     m_int;
        };
        string_type                 m_str;
    };

    /////////////////////////////////////////////////////////////////////////
    // LogFunc

    struct LogFuncParam
    {
        TypeID      m_type_id = invalid_id();
        string_type m_name;
    };

    struct LogFunc
    {
        bool        m_ellipse = false;
        TypeID      m_return_type = invalid_id();

        enum Convention {
            LFC_CDECL, LFC_STDCALL, LFC_FASTCALL
        } m_convention = LFC_CDECL;

        std::vector<LogFuncParam> m_param_list;

        static std::vector<LogFunc>& all()
        {
            static std::vector<LogFunc> s_log_funcs;
            return s_log_funcs;
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // LogStruct

    struct LogStructMember
    {
        TypeID          m_type_id;
        string_type     m_name;
        int             m_bit_offset = 0;
        int             m_bits = -1;
        bool operator==(const LogStructMember& other) const;
        bool operator!=(const LogStructMember& other) const;
    };

    struct LogStruct
    {
        TagID           m_tag_id = invalid_id();
        TypeID          m_type_id = invalid_id();
        bool            m_is_struct = true;
        int             m_pack = 8;
        int             m_align = 0;
        int             m_alignas = 0;
        bool            m_alignas_explicit = false;
        bool            m_is_complete = false;
        std::vector<LogStructMember>  m_members;

        bool operator==(const LogStruct& other) const;
        bool operator!=(const LogStruct& other) const;

        bool empty() const { return m_members.empty(); }
        size_t size() const { return m_members.size(); }

        static std::vector<LogStruct>& all(void)
        {
            static std::vector<LogStruct> s_log_structs;
            return s_log_structs;
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // LogEnum

    struct LogEnum
    {
        std::unordered_map<string_type, int> m_name2value;
        std::unordered_map<int, string_type> m_value2name;
        bool empty() const
        {
            return m_name2value.empty() && m_value2name.empty();
        }

        static std::vector<LogEnum>& all(void)
        {
            static std::vector<LogEnum> s_log_enums;
            return s_log_enums;
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // LogVar

    struct LogVar
    {
        string_type m_name;
        Value       m_value;
        Position    m_pos;
        bool        m_is_macro = false;
        ScopeID     m_scope_id = 0;

        static std::vector<LogVar>& all(void)
        {
            static std::vector<LogVar> s_log_vars;
            return s_log_vars;
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // LogMacro

    struct LogMacro
    {
        int                         m_num_params = 0;
        bool                        m_ellipsis = false;
        string_type                 m_contents;
        std::vector<string_type>    m_params;
        Position                    m_pos;

        static std::vector<LogMacro>& all(void)
        {
            static std::vector<LogMacro> s_log_macros;
            return s_log_macros;
        }
    };

    /////////////////////////////////////////////////////////////////////////

    struct LogScope
    {
        ScopeID                 m_scope_id = 0;
        ScopeID                 m_parent_id = invalid_id();
        std::set<ScopeID>       m_child_scope_ids;

        std::set<EntityID>      m_entity_ids;
        std::set<TagID>         m_tag_ids;
        std::set<LabelID>       m_label_ids;

        name2id_type            m_entry_map;
        name2id_type            m_tag_map;
        name2id_type            m_label_map;

        EntryID name_to_entry_id(const string_type& name) const;
        TagID name_to_tag_id(const string_type& tag_name) const;
        LabelID name_to_label_id(const string_type& name) const;

        bool has_entry(const string_type& name) const
        {
            return name_to_entry_id(name) != invalid_id();
        }
        bool has_tag(const string_type& name) const
        {
            return name_to_tag_id(name) != invalid_id();
        }
        bool has_label(const string_type& name) const
        {
            return name_to_label_id(name) != invalid_id();
        }

        static std::vector<LogScope>& all(void)
        {
            static std::vector<LogScope> s_log_scopes;
            return s_log_scopes;
        }
    };

    /////////////////////////////////////////////////////////////////////////

    struct LogEntity
    {
        string_type             m_name;

        enum EntryType {
            E_VAR, E_ENUM_VALUE, E_TYPEDEF_NAME, E_FUNC
        }                       m_entry_type;

        TypeID                  m_type_id = invalid_id();
        ID                      m_sub_id = invalid_id();
            // m_sub_id == VarID if E_VAR, E_ENUM_VALUE,
            // m_sub_id == TypeID if E_TYPEDEF_NAME,
            // m_sub_id == FuncID if E_FUNC.
        ScopeID                 m_scope_id = 0;
        Position                m_pos;

        static std::vector<LogEntity>& all(void)
        {
            static std::vector<LogEntity> s_log_entities;
            return s_log_entities;
        }
    };

    /////////////////////////////////////////////////////////////////////////

    struct LogTag
    {
        string_type             m_tag_name;
        TagID                   m_tag_id = invalid_id();
        enum {
            T_STRUCT, T_UNION, T_ENUM
        }                       m_tag_type = T_STRUCT;
        TypeID                  m_type_id = invalid_id();
        ScopeID                 m_scope_id = 0;
        Position                m_pos;

        static std::vector<LogTag>& all(void)
        {
            static std::vector<LogTag> s_log_tags;
            return s_log_tags;
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // LogLabel

    struct LogLabel
    {
        LabelID         m_label_id = invalid_id();
        ScopeID         m_scope_id = 0;
        Position        m_pos;

        static std::vector<LogLabel>& all(void)
        {
            static std::vector<LogLabel> s_log_labels;
            return s_log_labels;
        }
    };

    /////////////////////////////////////////////////////////////////////////

    struct LogType
    {
        string_type                 m_type_name;
        ID                          m_sub_id = invalid_id();

        enum : TypeFlagsType
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
            T_LONGLONG      = 0x00000040,
            T_INT64         = T_LONGLONG,
            T_INT128        = 0x00000080,
            // signed/unsigned
            T_SIGNED        = 0x00000100,
            T_UNSIGNED      = 0x00000200,
            T_SIGN_MASK     = 0x00000300,
            // enum values
            T_ENUM_VALUE    = 0x00000400 | T_INT,
            // floatings
            T_FLOATING      = 0x00000800,
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
            T_PTR64         = T_POINTER | T_INT64,
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
        TypeFlagsType               m_flags = T_INVALID;

        size_t                      m_sizeof = 0;
        size_t                      m_countof = 0;
        size_t                      m_alignof = 8;

        std::vector<TypeID>         m_type_ids;             // for T_FUNC or T_TAG
        std::vector<string_type>    m_type_names;           // for T_FUNC or T_TAG
        std::vector<string_type>    m_member_names;         // for T_FUNC or T_TAG

        ScopeID                     m_scope_id = invalid_id();

        Position                    m_pos;
        bool                        m_is_macro = false;

        static TypeFlagsType normalize_flags(TypeFlagsType flags);
        static bool is_integer(TypeFlagsType flags);
        static bool is_floating(TypeFlagsType flags);

        bool is_integer() const
        {
            return is_integer(m_flags);
        }
        bool is_floating() const
        {
            return is_floating(m_flags);
        }

        static std::vector<LogType>& all(void)
        {
            static std::vector<LogType> s_log_types;
            return s_log_types;
        }
    };
} // namespace CodeReverse

/////////////////////////////////////////////////////////////////////////

#endif  // ndef CODEREVERSE_TYPE_SYSTEM_HPP
