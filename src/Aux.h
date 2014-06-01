#ifndef __STUBMTP_AUX_H__
#define __STUBMTP_AUX_H__

#define STUBMTP_UNUSED(var) (void)var

#define STUBMTP_DISABLE_COPY(name)            \
    name(const name &)                = delete; \
    name(name &&)                     = delete; \
    name & operator = (const name &)  = delete; \
    name & operator = (name &&)       = delete;

#define STUBMTP_DEFAULT_COPY(name)               \
    name(const name &)                = default; \
    name(name &&)                     = default; \
    name & operator = (const name &)  = default; \
    name & operator = (name &&)       = default;


#define STUBMTP_FLAGS(type)                                                      \
    inline type operator | (const type lhs, const type rhs)                      \
    {                                                                            \
        return static_cast<type>(static_cast<int>(lhs) | static_cast<int>(rhs)); \
    }                                                                            \
    inline void operator |= (type & lhs, const type rhs)                         \
    {                                                                            \
        lhs = lhs | rhs;                                                         \
    }                                                                            \
    inline type operator & (const type lhs, const type rhs)                      \
    {                                                                            \
        return static_cast<type>(static_cast<int>(lhs) & static_cast<int>(rhs)); \
    }                                                                            \
    inline void operator &= (type & lhs, const type rhs)                         \
    {                                                                            \
        lhs = lhs & rhs;                                                         \
    }

#endif // __STUBMTP_AUX_H__
