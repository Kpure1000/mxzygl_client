#ifndef MX_ASSETMANAGER_H
#define MX_ASSETMANAGER_H

#include <QObject>

#include <unordered_map>

template<typename keyT>
struct is_basic_type : std::false_type {};
#define IS_BASIC_TYPE(_type) template <> struct is_basic_type<_type> : std::true_type {};

IS_BASIC_TYPE(int8_t)
IS_BASIC_TYPE(int16_t)
IS_BASIC_TYPE(int32_t)
IS_BASIC_TYPE(int64_t)
IS_BASIC_TYPE(uint8_t)
IS_BASIC_TYPE(uint16_t)
IS_BASIC_TYPE(uint32_t)
IS_BASIC_TYPE(uint64_t)
IS_BASIC_TYPE(float)
IS_BASIC_TYPE(double)
IS_BASIC_TYPE(long)
IS_BASIC_TYPE(unsigned long)

template<typename keyT, typename assetT, typename _Enable = void>
class IAssetManager
{};

template<typename keyT, typename assetT>
class IAssetManager<keyT, assetT, typename std::enable_if<!is_basic_type<keyT>::value>::type>
{
public:
    inline assetT get(const keyT &key) { return m_data[key]; }
    inline assetT get(const keyT &key) const { return m_data[key]; }

    inline assetT add(const keyT &key, assetT asset) { m_data.insert({key, asset}); return asset; }
    inline assetT addAny(const keyT &key, assetT asset) { m_data[key] = asset; return asset; }

    inline void remove(const keyT &key) { m_data.erase(m_data.find(key)); }

    inline bool has(const keyT &key) { return m_data.find(key) != m_data.end(); }    

    inline size_t size() const { return m_data.size(); }

    std::vector<keyT> keys() {
        std::vector<keyT> _keys;
        _keys.reserve(m_data.size());
        for (auto &key_v : m_data)
            _keys.emplace_back(key_v.first);
        return _keys;
    }

    std::vector<assetT> values() {
        std::vector<assetT> _vals;
        _vals.reserve(m_data.size());
        for (auto &key_v : m_data)
            _vals.emplace_back(key_v.second);
        return _vals;
    }

protected:
    std::unordered_map<keyT, assetT> m_data;
};

template<typename keyT, typename assetT>
class IAssetManager<keyT, assetT, typename std::enable_if<is_basic_type<keyT>::value>::type>
{
public:
    inline assetT get(keyT key) { return m_data[key]; }
    inline assetT get(keyT key) const { return m_data[key]; }

    inline assetT add(keyT key, assetT asset) { m_data.insert({key, asset}); return asset; }
    inline assetT addAny(keyT key, assetT asset) { m_data[key] = asset; return asset; }

    inline void remove(keyT key) { m_data.erase(m_data.find(key)); }

    inline bool has(keyT key) { return m_data.find(key) != m_data.end(); }

    inline size_t size() const { return m_data.size(); }

    std::vector<keyT> keys() {
        std::vector<keyT> _keys;
        _keys.reserve(m_data.size());
        for (auto &key_v : m_data)
            _keys.emplace_back(key_v.first);
        return _keys;
    }

    std::vector<assetT> values() {
        std::vector<assetT> _vals;
        _vals.reserve(m_data.size());
        for (auto &key_v : m_data)
            _vals.emplace_back(key_v.second);
        return _vals;
    }

protected:
    std::unordered_map<keyT, assetT> m_data;
};


#endif // MX_ASSETMANAGER_H
