
#pragma once

#include <functional>
#include <optional>

namespace dbcppp
{
    template <class T, class P, typename F>
    class Iterator final
    {
    public:
        using self_t            = Iterator<T, P, F>;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = value_type*;
        using reference         = value_type&;

        constexpr Iterator(P parent, F get, std::size_t i) noexcept
            : _parent(parent)
            , _get(std::move(get))
            , _i(i)
        {}
        inline reference operator*() const
        {
            return (_parent->*_get)(_i);
        }
        inline pointer operator->() const
        {
            return &(_parent->*_get)(_i);
        }
        constexpr self_t& operator++() noexcept
        {
            _i++;
            return *this;
        }
        constexpr self_t operator+(std::size_t o) const noexcept
        {
            return {_parent, _get, _i + o};
        }
        constexpr self_t operator-(std::size_t o) const noexcept
        {
            return {_parent, _get, _i - o};
        }
        constexpr difference_type operator-(const self_t& rhs) const noexcept
        {
            return _i - rhs._i;
        }
        constexpr self_t& operator+=(std::size_t o) noexcept
        {
            _i += o;
            return *this;
        }
        constexpr self_t& operator-=(std::size_t o) noexcept
        {
            _i -= o;
            return *this;
        }

        constexpr bool operator==(const self_t& rhs) const noexcept
        {
            return _i == rhs._i;
        }
        constexpr bool operator!=(const self_t& rhs) const noexcept
        {
            return !(*this == rhs);
        }

    private:
        P _parent;
        F _get;
        std::size_t _i;
    };
    template<class T, class P, typename F>
    class Iterable final
    {
    public:
        using value_type = const T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = Iterator<T, P, F>;
        using const_iterator = Iterator<const T, P, F>;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        constexpr Iterable(P parent, F get, size_t size) noexcept
            : _parent(std::move(parent))
            , _get(std::move(get))
            , _size(std::move(size))
        {}
        constexpr iterator begin() noexcept
        {
            return iterator(_parent, _get, 0);
        }
        constexpr iterator end() noexcept
        {
            return iterator(_parent, _get, _size);
        }
        constexpr const_iterator begin() const noexcept
        {
            return const_iterator(_parent, _get, 0);
        }
        constexpr const_iterator end() const noexcept
        {
            return const_iterator(_parent, _get, _size);
        }

    private:
        P _parent;
        F _get;
        size_t _size;
    };
}
#define DBCPPP_MAKE_ITERABLE(ClassName, Name, Type)                                                       \
    inline Iterable<const Type, const ClassName*, decltype(&ClassName::Name##_Get)> Name() const noexcept \
    {                                                                                                     \
        return {this, &ClassName::Name##_Get, Name##_Size()};                                             \
    }
