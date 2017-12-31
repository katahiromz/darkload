// TypeInfo.cpp --- CodeReverse type system
// Copyright (C) 2017 Katayama Hirofumi MZ. License: MIT License

#include "TypeSystem.hpp"

TypedValue::TypedValue() : m_flags(T_INT), m_uint64(0), m_str("0")
{
}

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
    return (flags & (T_BOOL | T_CHAR | T_INT);
}

/*static*/ bool LogType::is_floating(TypeFlagsType flags)
{
    flags = normalize_flags(flags);
    return (flags & T_FLOATING);
}
