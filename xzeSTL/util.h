#pragma once
#ifndef XZESTL_UTIL_H_
#define XZESTL_UTIL_H_

// 这个文件包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等 

#include <cstddef>
#include "algobase.h"

namespace xze
{

    // move

    template <class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    // forward

    template <class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept
    {
        return static_cast<T&&>(arg);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
    {
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T&&>(arg);
    }



    template <class ForwardIter1, class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
    {
        for (; first1 != last1; ++first1, (void) ++first2)
            xze::swap(*first1, *first2);
        return first2;
    }



    //todo
    // --------------------------------------------------------------------------------------
// 类模板 : temporary_buffer
// 进行临时缓冲区的申请与释放
    template <class ForwardIterator, class T>
    class temporary_buffer
    {
    private:
        ptrdiff_t original_len;  // 缓冲区申请的大小
        ptrdiff_t len;           // 缓冲区实际的大小
        T* buffer;        // 指向缓冲区的指针

    public:
        // 构造、析构函数
        temporary_buffer(ForwardIterator first, ForwardIterator last);

        ~temporary_buffer()
        {
            destroy(buffer, buffer + len);
            free(buffer);
        }

    public:

        ptrdiff_t size()           const noexcept { return len; }
        ptrdiff_t requested_size() const noexcept { return original_len; }
        T* begin()                noexcept { return buffer; }
        T* end()                  noexcept { return buffer + len; }

    private:
        void allocate_buffer();
        void initialize_buffer(const T&, std::true_type) {}
        void initialize_buffer(const T& value, std::false_type)
        {
            uninitialized_fill_n(buffer, len, value);
        }

    private:
        temporary_buffer(const temporary_buffer&);
        void operator=(const temporary_buffer&);
    };

    // 构造函数
    template <class ForwardIterator, class T>
    temporary_buffer<ForwardIterator, T>::
        temporary_buffer(ForwardIterator first, ForwardIterator last)
    {
        try
        {
            len = distance(first, last);
            allocate_buffer();
            if (len > 0)
            {
                initialize_buffer(*first, std::is_trivially_default_constructible<T>());
            }
        }
        catch (...)
        {
            free(buffer);
            buffer = nullptr;
            len = 0;
        }
    }

    // allocate_buffer 函数
    template <class ForwardIterator, class T>
    void temporary_buffer<ForwardIterator, T>::allocate_buffer()
    {
        original_len = len;
        if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
            len = INT_MAX / sizeof(T);
        while (len > 0)
        {
            buffer = static_cast<T*>(malloc(len * sizeof(T)));
            if (buffer)
                break;
            len /= 2;  // 申请失败时减少申请空间大小
        }
    }











    // --------------------------------------------------------------------------------------
    // pair

    // 结构体模板 : pair
    // 两个模板参数分别表示两个数据的类型
    // 用 first 和 second 来分别取出第一个数据和第二个数据
    template <class Ty1, class Ty2>
    struct pair
    {
        typedef Ty1    first_type;
        typedef Ty2    second_type;

        first_type first;    // 保存第一个数据
        second_type second;  // 保存第二个数据

        // default constructiable
        template <class Other1 = Ty1, class Other2 = Ty2,
            typename = typename std::enable_if<
            std::is_default_constructible<Other1>::value&&
            std::is_default_constructible<Other2>::value, void>::type>
        constexpr pair()
            : first(), second()
        {
        }

        // implicit constructiable for this type
        template <class U1 = Ty1, class U2 = Ty2,
            typename std::enable_if<
            std::is_copy_constructible<U1>::value&&
            std::is_copy_constructible<U2>::value&&
            std::is_convertible<const U1&, Ty1>::value&&
            std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
        constexpr pair(const Ty1& a, const Ty2& b)
            : first(a), second(b)
        {
        }

        // explicit constructible for this type
        template <class U1 = Ty1, class U2 = Ty2,
            typename std::enable_if<
            std::is_copy_constructible<U1>::value&&
            std::is_copy_constructible<U2>::value &&
            (!std::is_convertible<const U1&, Ty1>::value ||
                !std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
        explicit constexpr pair(const Ty1& a, const Ty2& b)
            : first(a), second(b)
        {
        }

        pair(const pair& rhs) = default;
        pair(pair&& rhs) = default;

        // implicit constructiable for other type
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value&&
            std::is_convertible<Other1&&, Ty1>::value&&
            std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
        constexpr pair(Other1&& a, Other2&& b)
            : first(xze::forward<Other1>(a)),
            second(xze::forward<Other2>(b))
        {
        }

        // explicit constructiable for other type
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value &&
            (!std::is_convertible<Other1, Ty1>::value ||
                !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
        explicit constexpr pair(Other1&& a, Other2&& b)
            : first(xze::forward<Other1>(a)),
            second(xze::forward<Other2>(b))
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, const Other1&>::value&&
            std::is_constructible<Ty2, const Other2&>::value&&
            std::is_convertible<const Other1&, Ty1>::value&&
            std::is_convertible<const Other2&, Ty2>::value, int>::type = 0>
        constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
            second(other.second)
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, const Other1&>::value&&
            std::is_constructible<Ty2, const Other2&>::value &&
            (!std::is_convertible<const Other1&, Ty1>::value ||
                !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
        explicit constexpr pair(const pair<Other1, Other2>& other)
            : first(other.first),
            second(other.second)
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value&&
            std::is_convertible<Other1, Ty1>::value&&
            std::is_convertible<Other2, Ty2>::value, int>::type = 0>
        constexpr pair(pair<Other1, Other2>&& other)
            : first(xze::forward<Other1>(other.first)),
            second(xze::forward<Other2>(other.second))
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
            typename std::enable_if<
            std::is_constructible<Ty1, Other1>::value&&
            std::is_constructible<Ty2, Other2>::value &&
            (!std::is_convertible<Other1, Ty1>::value ||
                !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
        explicit constexpr pair(pair<Other1, Other2>&& other)
            : first(xze::forward<Other1>(other.first)),
            second(xze::forward<Other2>(other.second))
        {
        }

        // copy assign for this pair
        pair& operator=(const pair& rhs)
        {
            if (this != &rhs)
            {
                first = rhs.first;
                second = rhs.second;
            }
            return *this;
        }

        // move assign for this pair
        pair& operator=(pair&& rhs)
        {
            if (this != &rhs)
            {
                first = xze::move(rhs.first);
                second = xze::move(rhs.second);
            }
            return *this;
        }

        // copy assign for other pair
        template <class Other1, class Other2>
        pair& operator=(const pair<Other1, Other2>& other)
        {
            first = other.first;
            second = other.second;
            return *this;
        }

        // move assign for other pair
        template <class Other1, class Other2>
        pair& operator=(pair<Other1, Other2>&& other)
        {
            first = xze::forward<Other1>(other.first);
            second = xze::forward<Other2>(other.second);
            return *this;
        }

        ~pair() = default;

        void swap(pair& other)
        {
            if (this != &other)
            {
                xze::swap(first, other.first);
                xze::swap(second, other.second);
            }
        }

    };

    // 重载比较操作符 
    template <class Ty1, class Ty2>
    bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class Ty1, class Ty2>
    bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template <class Ty1, class Ty2>
    bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Ty1, class Ty2>
    bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return rhs < lhs;
    }

    template <class Ty1, class Ty2>
    bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Ty1, class Ty2>
    bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
    {
        return !(lhs < rhs);
    }

    // 重载  swap
    template <class Ty1, class Ty2>
    void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs)
    {
        lhs.swap(rhs);
    }

    // 全局函数，让两个数据成为一个 pair
    template <class Ty1, class Ty2>
    pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
    {
        return pair<Ty1, Ty2>(xze::forward<Ty1>(first), xze::forward<Ty2>(second));
    }

}

#endif // 

