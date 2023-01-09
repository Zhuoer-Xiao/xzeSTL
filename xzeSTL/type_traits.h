#pragma once
#ifndef XZESTL_TYPETRAITS_H
#define XZESTL_TYPETRAITS_H
//首先traits萃取器的目的是为了获得iterator所指向元素的type，
//把iterator丢进去，把元素type萃取出来。
//type_traits提供了在编译期进行计算、判断、转换、查询等功能；
//type_traits提供了编译期的true和false；

namespace xze {
//C++ 程序从编写完毕到执行分为四个阶段：预处理、 编译、汇编和链接 4 个阶段，\
得到可执行程序之后就可以运行了。需要额外强调的是，常量表达式和非常量表达式的计算时机不同，\
非常量表达式只能在程序运行阶段计算出结果，但是常量表达式的计算往往发生在程序的编译阶段，\
这可以极大提高程序的执行效率，因为表达式只需要在编译阶段计算一次，\
节省了每次程序运行时都需要计算一次的时间。\
在 C++11 中添加了 constexpr 关键字之后就可以在程序中使用它来修改常量表达式，\
用来提高程序的执行效率。在使用中建议将 const 和 constexpr 的功能区分开，\
即凡是表达“只读”语义的场景都使用 const，表达“常量”语义的场景都使用 constexpr。

//integral_constant的作用是定义类型的常量
template <class T, T v>
struct m_integral_constant
{
	static constexpr T value = v;
};

template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;
// using代替typedef，c++11特性​
typedef m_bool_constant<true>  m_true_type;
typedef m_bool_constant<false> m_false_type;

//is_same用来判断两个type是否相同。通过偏特化，若两个type相同，自动匹配到下面
template<typename T, typename U>
struct m_is_same : public m_false_type {};
template<typename T>
struct m_is_same<T,T>:public m_true_type{};

}

#endif // !XZESTL_TYPETRAITS_H
