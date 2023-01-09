#pragma once
#ifndef XZESTL_FUNCTION_H
#define XZESTL_FUNCTION_H
#include <cstddef>
//���ļ�ʵ�ַº���������ָ���޷���STL�����������������ʵ�ַº���
//�ú���ʵ��hash����
namespace xze{


//һԪ�����Ĳ����ͱ�ͷ���ֵ
template <class Arg,class Result>
struct unary_function {
	typedef Arg argument_type;
	typedef Result result_type;
};
//��Ԫ�����Ĳ����ͱ�ͷ���ֵ
template <class Arg1,class Arg2,class Result>
struct binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};


//������º���
//��
template<class T>
struct plus:public binary_function<T,T,T> {
	T operator()(const T& x, const T& y)const {
		return x + y;
	}
};
//��
template<class T>
struct minus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x - y;
	}
};
//��
template<class T>
struct multiplies :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x * y;
	}
};
//��
template<class T>
struct divides :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x / y;
	}
};
//ģȡ
template<class T>
struct modulus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x % y;
	}
};
//��
template<class T>
struct negate :public unary_function<T, T> {
	T operator()(const T& x)const {
		return -x;
	}
};


//��ϵ�����
// ����
template <class T>
struct equal_to :public binary_function<T,T,bool> {
	bool operator()(const T& x, const T& y)const { return x == y; }
};
// ������
template <class T>
struct not_equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x != y; }
};
// ����
template <class T>
struct greater :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x > y; }
};
// ���ڵ���
template <class T>
struct greater_equal :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x >= y; }
};
// С��
template <class T>
struct less :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x < y; }
};
//С�ڵ���
template <class T>
struct less_equal :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const { return x <= y; }
};

//�߼�����
// ��
template<class T>
class logic_and:public binary_function<T,T,bool> {
	bool operator()(const T& x, const T& y)const {
		return x && y;
	}
};
//��
template<class T>
class logic_or :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const {
		return x || y;
	}
};
//��
template<class T>
class logic_not :public unary_function<T, bool> {
	bool operator()(const T& x)const {
		return !x;
	}
};

//֤ͬ:�κκ���ͨ���˺������ı�
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


//ѡ����������һ��pair�����ص�һ��Ԫ��
template <class Pair>
struct select1st :public unary_function<Pair,typename Pair::first_type> {
	const typename Pair::first_type& operator()(const Pair& x)const {
		return x.first;
	}
};
//���صڶ���
template <class Pair>
struct select2nd :public unary_function<Pair, typename Pair::second_type> {
	const typename Pair::second_type& operator()(const Pair& x)const {
		return x.second;
	}
};
//Ͷ�亯�������ص�һ������ڶ�����
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


//��ϣ����
//�󲿷�hashʲôҲû��
template<class Key>
struct hash{};
//�����ַ��������һ���򵥵�ת������
inline size_t _stl_hash_string(const char* s) {
	unsigned long h = 0;
	for (; *s; ++s) {
		h = 5 * h + *s;
	}
	return size_t(h);
}

// ���ָ���ƫ�ػ��汾
template <class T>
struct hash<T*>
{
	size_t operator()(T* p) const noexcept
	{
		return reinterpret_cast<size_t>(p);
	}
};

// �����������ͣ�ֻ�Ƿ���ԭֵ
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

// string,float,double�޷�ֱ�Ӵ����Զ���hash����
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
