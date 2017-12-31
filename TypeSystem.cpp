// TypeSystem.cpp --- CodeReverse type system
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License

#include "TypeSystem.hpp"

/////////////////////////////////////////////////////////////////////////

namespace CodeReverse
{
    /*static*/ TypeFlagsType
    LogType::normalize_flags(TypeFlagsType flags)
    {
        if (flags & T_INT)
        {
            // remove "int" if wordy
            if (flags & (T_SHORT | T_LONG | T_LONGLONG | T_INT128))
            {
                if (!(flags & T_POINTER))
                {
                    flags &= ~T_INT;
                }
            }
        }
        if (flags & T_UNSIGNED)
        {
            if (!(flags & (T_CHAR | T_SHORT | T_LONG | T_LONGLONG |
                           T_INT128 | T_INT)))
            {
                // add "int" for single "unsigned"
                flags |= T_INT;
            }
        }
        if ((flags & T_SIGNED) && !(flags & T_CHAR))
        {
            // delete signed for non-char
            flags &= ~T_SIGNED;
        }
        if (!(flags & (T_VOID | T_BOOL | T_FLOATING | T_COMPLEX | T_IMAGINARY |
                       T_ARRAY | T_POINTER | T_ALIAS | T_TAG | T_FUNC)))
        {
            flags |= T_INT;
        }
        return flags;
    }

    /*static*/ bool LogType::is_integer(TypeFlagsType flags)
    {
        flags = normalize_flags(flags);
        if (is_floating(flags))
            return false;
        return (flags & (T_BOOL | T_CHAR | T_INT | T_INT128));
    }

    /*static*/ bool LogType::is_floating(TypeFlagsType flags)
    {
        flags = normalize_flags(flags);
        return (flags & T_FLOATING);
    }

    /////////////////////////////////////////////////////////////////////////
    // adding types

    TypeID LogScope::add_type(const string_type& name, const LogType& type)
    {
        TypeID tid = LogType::all().size();
        LogType::all().push_back(type);
        LogEntity::all().push_back(...);
        m_type_map[name] = tid;
        return tid;
    }
    TypeID LogScope::add_type(const string_type& name, TypeFlagsType flags, size_t size,
                              const Position& pos)
    {
    }
    TypeID LogScope::add_type(const string_type& name, TypeFlagsType flags, size_t size,
                              int align, const Position& pos)
    {
    }
    TypeID LogScope::add_type(const string_type& name, TypeFlagsType flags, size_t size,
                              int align, int alignas_, const Position& pos)
    {
    }
    TypeID LogScope::add_alias_type(const string_type& name, TypeID tid, const Position& pos)
    {
    }
    TypeID LogScope::add_alias_macro_type(const string_type& name, TypeID tid, const Position& pos)
    {
    }
    VarID LogScope::add_var(const string_type& name, TypeID tid, const Position& pos)
    {
    }
    VarID LogScope::add_var(const string_type& name, TypeID tid, const Position& pos, const Value& value)
    {
    }
    TypeID LogScope::add_const_type(TypeID tid)
    {
        auto& type = LogType::all()[tid];
        LogType new_type;
        new_type.m_sub_id = tid;
        new_type.m_flags = T_CONST;
        new_type.m_sizeof = type.m_sizeof;
        new_type.m_countof = type.m_countof;
        new_type.m_alignof = type.m_alignof;
        new_type.m_scope_id = m_scope_id;
        new_type.m_pos = type.m_pos;
        new_type.m_is_macro = false;
        return add_type("", new_type);
    }
    TypeID LogScope::add_pointer_type(TypeID tid, TypeFlagsType flags, const Position& pos)
    {
    }
    TypeID LogScope::add_array_type(TypeID tid, size_t count, const Position& pos)
    {
    }
    TypeID LogScope::add_func_type(const LogFunc& func, const Position& pos)
    {
    }
    TypeID LogScope::add_struct_type(const LogStruct& struct_, int alignas_, const Position& pos)
    {
    }
    TypeID LogScope::add_enum_type(const LogEnum& enum_, int alignas_, const Position& pos)
    {
    }

    LogScope::LogScope(ScopeID parent_scope_id)
    {
        m_scope_id = all().size();

        if (parent_scope_id == invalid_id())
        {
            TypeID tid;
            Position pos("(predefined)");

            add_type("void", T_VOID, 0, pos);

            add_type("char", T_CHAR, sizeof(char), pos);
            add_type("short", T_SHORT, sizeof(short), pos);
            add_type("long", T_LONG, sizeof(long), pos);
            tid = add_type("long long", T_LONGLONG, sizeof(long long), pos);
            add_alias_type("__int64", tid, pos);

            add_type("int", T_INT, sizeof(int), pos);

            add_type("unsigned char", T_UNSIGNED | T_CHAR, sizeof(char), pos);
            add_type("unsigned short", T_UNSIGNED | T_SHORT, sizeof(short), pos);
            add_type("unsigned long", T_UNSIGNED | T_LONG, sizeof(long), pos);
            tid = add_type("unsigned long long", T_UNSIGNED | T_LONGLONG, sizeof(long long), pos);
            add_alias_type("unsigned __int64", tid, pos);

            add_type("__int128", T_INT128, 128 / 8, pos);
            add_type("unsigned __int128", T_UNSIGNED | T_INT128, 128 / 8, pos);

            add_type("unsigned int", T_UNSIGNED | T_INT, sizeof(int), pos);

            add_type("float", T_FLOAT, sizeof(float), pos);
            add_type("double", T_DOUBLE, sizeof(double), pos);

            add_type("long double", T_LONG | T_DOUBLE, sizeof(long double), pos);
        }
        else
        {
            m_parent_id = parent_scope_id;
            all()[m_parent_id].m_child_scope_ids.insert(m_scope_id);
        }
    }

    TypeID LogScope::name_to_type_id(const string_type& name) const
    {
        auto it = m_type_map.find(name);
        if (it == m_type_map.end())
        {
            for (auto scope_id : m_child_scope_ids)
            {
                auto& scope = all()[scope_id];
                auto id = scope.name_to_type_id(name);
                if (id != invalid_id())
                {
                    return id;
                }
            }
            return invalid_id();
        }
        return it->second;
    }
    EntryID LogScope::name_to_entry_id(const string_type& name) const
    {
        auto it = m_entry_map.find(name);
        if (it == m_entry_map.end())
        {
            for (auto scope_id : m_child_scope_ids)
            {
                auto& scope = all()[scope_id];
                auto id = scope.name_to_entry_id(name);
                if (id != invalid_id())
                {
                    return id;
                }
            }
            return invalid_id();
        }
        return it->second;
    }
    TagID LogScope::name_to_tag_id(const string_type& tag_name) const
    {
        auto it = m_tag_map.find(name);
        if (it == m_tag_map.end())
        {
            for (auto scope_id : m_child_scope_ids)
            {
                auto& scope = all()[scope_id];
                auto id = scope.name_to_tag_id(name);
                if (id != invalid_id())
                {
                    return id;
                }
            }
            return invalid_id();
        }
        return it->second;
    }
    LabelID LogScope::name_to_label_id(const string_type& name) const
    {
        auto it = m_label_map.find(name);
        if (it == m_label_map.end())
        {
            for (auto scope_id : m_child_scope_ids)
            {
                auto& scope = all()[scope_id];
                auto id = scope.name_to_label_id(name);
                if (id != invalid_id())
                {
                    return id;
                }
            }
            return invalid_id();
        }
        return it->second;
    }

/////////////////////////////////////////////////////////////////////////

} // namespace CodeReverse
