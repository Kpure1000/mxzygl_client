#ifndef MX_QSTRING_HASH_H
#define MX_QSTRING_HASH_H

#include <QString>
#include <unordered_map>

namespace std
{
template<> struct hash<QString>
{
    std::size_t operator()(const QString& s) const noexcept
    {
        const QChar* str = s.data();
        std::size_t hash = 5381;

        for (int i = 0; i < s.size(); ++i)
            hash = ((hash << 5) + hash) + ((str->row() << 8) | (str++)->cell());

        return hash;
    }
};
}

#endif // MX_QSTRING_HASH_H
