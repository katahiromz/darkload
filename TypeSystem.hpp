// TypeInfo.hpp --- CodeReverse type system
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License
#ifndef CODEREVERSE_TYPE_SYSTEM_HPP
#define CODEREVERSE_TYPE_SYSTEM_HPP

#include "Common.hpp"
#include <vector>
#include <set>

/////////////////////////////////////////////////////////////////////////

namespace CodeReverse
{
    typedef size_t ID;
    typedef ID EntityID;
    typedef ID FuncID;
    typedef ID MacroID;
    typedef ID ScopeID;
    typedef ID TagID;
    typedef ID TypeID;
    typedef ID VarID;
    typedef unsigned long TypeFlagsType;

    inline ID get_invalid_id(void)
    {
        return ID(-1);
    }

    typedef Value
    {
        TypeFlagsType               m_flags;
        union
        {
            __int64                 m_int64;
            unsigned __int64        m_uint64;
            int                     m_int;
            unsigned int            m_uint;
        };
        string_type                 m_value;
    };

    /////////////////////////////////////////////////////////////////////////

    TypeID entity_id_to_type_id(EntityID type_id);
    TypeID type_id_to_entity_id(EntityID type_id);

    /////////////////////////////////////////////////////////////////////////
    // LogFunc

    struct LogFuncParam
    {
        TypeID      m_type_id;
        string_type m_name;

        LogFuncParam(TypeID tid, const string_type& name)
            : m_type_id(tid), m_name(name)
        {
        }
    };

    struct LogFunc
    {
        bool        m_ellipse;
        TypeID      m_return_type;
        enum Convention {
            LFC_CDECL, LFC_STDCALL, LFC_FASTCALL
        } m_convention;
        std::vector<LogFuncParam> m_param_list;

        LogFunc() : m_ellipse(false), m_return_type(0), m_convention(LFC_CDECL)
        {
        }
    };
    std::vector<LogFunc>& get_log_funcs(void);

    /////////////////////////////////////////////////////////////////////////
    // LogStruct

    struct LogStructMember
    {
        TypeID          m_type_id;
        string_type     m_name;
        int             m_bit_offset;
        int             m_bits;
        LogStructMember() = default;
        LogStructMember(TypeID tid, const string_type& name,
            int bit_offset = 0, int bits = -1) :
                m_type_id(tid), m_name(name),
                    m_bit_offset(bit_offset), m_bits(bits) { }
        bool operator==(const LogStructMember& other) const;
        bool operator!=(const LogStructMember& other) const;
    };

    struct LogStruct
    {
        TypeID          m_type_id;
        bool            m_is_struct;
        int             m_pack;
        int             m_align;
        int             m_alignas;
        bool            m_alignas_explicit;
        bool            m_is_complete;
        std::vector<LogStructMember>  m_members;

        LogStruct(bool is_struct = true) :
            m_is_struct(is_struct), m_pack(8), m_align(0), m_alignas(0),
            m_alignas_explicit(false), m_is_complete(false) { }
        bool operator==(const LogStruct& other) const;
        bool operator!=(const LogStruct& other) const;

        bool empty() const { return m_members.empty(); }
        size_t size() const { return m_members.size(); }
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
    };
    std::vector<LogEnum>& get_log_enums(void);
    EnumID name_to_enum_id(const string_type& name);

    /////////////////////////////////////////////////////////////////////////
    // LogVar

    struct LogVar
    {
        string_type m_name;
        Value       m_value;
        Position    m_pos;
        bool        m_is_macro;
        ScopeID     m_scope_id;
    };
    std::vector<LogVar>& get_log_vars(void);
    VarID name_to_var_id(const string_type& name);

    /////////////////////////////////////////////////////////////////////////
    // LogMacro

    struct LogMacro
    {
        int                         m_num_params;
        bool                        m_ellipsis;
        std::string                 m_contents;
        std::vector<std::string>    m_params;
        Position                    m_pos;
        LogMacro() : m_num_params(0), m_ellipsis(false) { }
    };
    std::vector<LogMacro>& get_log_macros(void);
    MacroID name_to_macro_id(const string_type& name);

    /////////////////////////////////////////////////////////////////////////

    struct Scope
    {
        ScopeID                 m_scope_id;
        size_t                  m_parent_id;
        std::set<ScopeID>       m_child_scope_ids;
        std::set<EntityID>      m_entity_ids;
        std::set<TagID>         m_tag_ids;
        static ScopeID get_next()
        {
            static ScopeID s_id = 0;
            return s_id++;
        }
    };
    FuncID scope_id_to_func_id(ScopeID sid);
    ScopeID func_id_to_scope_id(FuncID fid);

    // NOTE: An entity is a variable, a typedef-name, a enum-value or a function.
    struct Entity
    {
        EntityID                m_entity_id;
        string_type             m_entity_name;
        enum Kind {
            E_VAR, E_TYPEDEF_NAME, E_ENUM_VALUE, E_FUNC
        }                       m_kind;
        TypeID                  m_type_id;
        ScopeID                 m_scope_id;
        union
        {
            int                 m_int;
            unsigned int        m_uint;
            __int64             m_int64;
            unsigned __int64    m_uint64;
        };
    };
    Entity& entity_id_to_entity(EntityID entity_id);

    struct StructMember
    {
    };
    struct Struct;

    // NOTE: A tag is a struct, a union or an enum.
    struct Tag
    {
        string_type m_tag_name;
        enum {
            T_STRUCT, T_UNION, T_ENUM
        } m_type;
        string_type m_type_name;
        TypeID m_type_id;
        ScopeID m_scope_id;
    };

    struct Label
    {
        string_type m_label_name;
        string_type m_func_name;
    };

    struct LogType
    {
        TypeID              m_type_id;
        string_type         m_type_name;
        ID                  m_sub_id;

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
        TypeFlagsType m_flags;

        size_t m_sizeof, m_countof, m_alignof;

        std::vector<TypeID>         m_type_ids;             // for T_FUNC or T_TAG
        std::vector<string_type>    m_type_names;           // for T_FUNC or T_TAG
        std::vector<string_type>    m_member_names;         // for T_FUNC or T_TAG
        ScopeID                     m_scope_id;

        Position                    m_pos;
        bool                        m_is_macro;

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
    };
} // namespace CodeReverse

#endif  // ndef CODEREVERSE_TYPE_SYSTEM_HPP
