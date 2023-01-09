#pragma once
#ifndef XZESTL_FUNCTION_H
#define XZESTL_FUNCTION_H
#include <cstddef>
//该文件实现仿函数，函数指针无法与STL其他组件合作，于是实现仿函数
//该函数实现hash函数
namespace xze{


//一元函数的参数型别和返回值
template <class Arg,class Result>
struct unary_function {
	typedef Arg argument_type;
	typedef Result result_type;
};
//二元函数的参数型别和返回值
template <class Arg1,class Arg2,class Result>
struct binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};


//算术类仿函数
//加
template<class T>
struct plus:public binary_function<T,T,T> {
	T operator()(const T& x, const T& y)const {
		return x + y;
	}
};
//减
template<class T>
struct minus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x - y;
	}
};
//乘
template<class T>
struct multiplies :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x * y;
	}
};
//除
template<class T>
struct divides :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x / y;
	}
};
//模取
template<class T>
struct modulus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x % y;
	}
};
//否定
template<class T>
struct negate :public unary_function<T, T> {
	T operator()(const T& x)const {
		return -x;
	}
};


//关系运算符
// 等于
template <class T>
struct equal_to :public binary_function<T,T,bool> {
	bool operator()(const T& x, const T& y)const { return x == y; }
};
// 不等于
template <class T>
struct not_equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x != y; }
};
// 大于
template <class T>
struct greater :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x > y; }
};
// 大于等于
template <class T>
struct greater_equal :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x >= y; }
};
// 小于
template <class T>
struct less :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x < y; }
};
//小于等于
template <class T>
struct less_equal :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x <= y; }
};

//逻辑运算
// 与
template<class T>
class logic_and:public binary_function<T,T,bool> {
	bool operator()(const T& x, const T& y)const {
		return x && y;
	}
};
//或
template<class T>
class logic_or :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const {
		return x || y;
	}
};
//非
template<class T>
class logic_not :public unary_function<T, bool> {
	bool operator()(const T& x)const {
		return !x;
	}
};

//证同:任何函数通过此函数不改变
template<class T>
struct identity :public unary_function<T, T> {
	const T& operator()(const T& x) { return x; }
};

template<class T>
inline T identity_element(plus<T>) {
	return T(0);
}
template<class T>
inline T identity_element(minus<T>) {
	return T(0);
}
template<class T>
inline T identity_element(multiplies<T>) {
	return T(1);
}
template<class T>
inline T identity_element(divides<T>) {
	return T(1);
}


//选择函数，接收一个pair，返回第一个元素
template <class Pair>
struct select1st :public unary_function<Pair,typename Pair::first_type> {
	const typename Pair::first_type& operator()(const Pair& x)const {
		return x.first;
	}
};
//返回第二个
template <class Pair>
struct select2nd :public unary_function<Pair, typename Pair::second_type> {
	const typename Pair::second_type& operator()(const Pair& x)const {
		return x.second;
	}
};
//投射函数，传回第一参数或第二参数
template<class Arg1,class Arg2>
struct project1st :public binary_function<Arg1,Arg2,Arg1> {
	Arg1 operator()(const Arg1& x, const Arg2&) {
		return x;
	}
};
template<class Arg1, class Arg2>
struct project2nd :public binary_function<Arg1, Arg2, Arg2> {
	Arg1 operator()(const Arg1&, const Arg2&y) {
		return y;
	}
};


//哈希函数
//大部分hash什么也没做
template<class Key>
struct hash{};
//对于字符串，设计一个简单的转换函数
inline size_t _stl_hash_string(const char* s) {
	unsigned long h = 0;
	for (; *s; ++s) {
		h = 5 * h + *s;
	}
	return size_t(h);
}

// 针对指针的偏特化版本
template <class T>
struct hash<T*>
{
	size_t operator()(T* p) const noexcept
	{
		return reinterpret_cast<size_t>(p);
	}
};

// 对于整型类型，只是返回原值
#define TRIVIAL_HASH_FCN(Type)         \
template <> struct hash<Type>                \
{                                            \
  size_t operator()(Type val) const noexcept \
  { return static_cast<size_t>(val); }       \
};

TRIVIAL_HASH_FCN(bool)

TRIVIAL_HASH_FCN(char)

TRIVIAL_HASH_FCN(signed char)

TRIVIAL_HASH_FCN(unsigned char)

TRIVIAL_HASH_FCN(wchar_t)

TRIVIAL_HASH_FCN(char16_t)

TRIVIAL_HASH_FCN(char32_t)

TRIVIAL_HASH_FCN(short)

TRIVIAL_HASH_FCN(unsigned short)

TRIVIAL_HASH_FCN(int)

TRIVIAL_HASH_FCN(unsigned int)

TRIVIAL_HASH_FCN(long)

TRIVIAL_HASH_FCN(unsigned long)

TRIVIAL_HASH_FCN(long long)

TRIVIAL_HASH_FCN(unsigned long long)

#undef MYSTL_TRIVIAL_HASH_FCN

// string,float,double无法直接处理，自定义hash函数
inline size_t bitwise_hash(const unsigned char* first, size_t count)
{
#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
	const size_t fnv_offset = 14695981039346656037ull;
	const size_t fnv_prime = 1099511628211ull;
#else
	const size_t fnv_offset = 2166136261u;
	const size_t fnv_prime = 16777619u;
#endif
	size_t result = fnv_offset;
	for (size_t i = 0; i < count; ++i)
	{
		result ^= (size_t)first[i];
		result *= fnv_prime;
	}
	return result;
}

template <>
struct hash<float>
{
	size_t operator()(const float& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(float));
	}
};

template <>
struct hash<double>
{
	size_t operator()(const double& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(double));
	}
};

template <>
struct hash<long double>
{
	size_t operator()(const long double& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(long double));
	}
};
}
#endif // !XZESTL_FUNCTION_H
