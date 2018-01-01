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

    LogScope::LogScope(ScopeID parent_scope_id)
    {
        m_scope_id = all().size();
        m_parent_id = parent_scope_id;

        if (parent_scope_id == invalid_id())
        {
            init_default_scope();
        }
        else
        {
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
