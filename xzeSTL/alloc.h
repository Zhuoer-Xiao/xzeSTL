#pragma once
#ifndef XZESTL_ALLOC_H
#define XZESTL_ALLOC_H
//更高效的双层分配，区块超过128b使用1级分配器，小于使用2级
#if 0
#	include<new>
#	define _THROW_BAD_ALLOC throw_bad_alloc
#elif !defined(_THROW_BAD_ALLOC)
#	include<iostream>
#	define _THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl; exit(1)
#endif
namespace xze {
	
	//一级分配器，调用c的malloc和realloc实现
template<int inst>
class _malloc_alloc_template {
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*, size_t);
	static void(*_malloc_alloc_oom_handler)();

public:
	static void* allocate(size_t n) {
		//直接使用malloc分配，如果失败使用oom-malloc
		void* result = std::malloc(n);
		if (result == 0) {
			result = oom_malloc(n);
		}
		return result;
	}

	static void deallocate(void*p,size_t) {
		free(p);
	}

	static void* reallocate(void *p,size_t ,size_t new_sz) {
		//直接使用malloc分配，如果失败使用oom-malloc
		void* result = std::realloc(p,new_sz);
		if (result == 0) {
			result = oom_realloc(p,new_sz);
		}
		return result;
	}
	//设定outofmemory时的handler，传入函数指针
	static void (*set_malloc_handler(void(*f)()))() {
		void(*old)() = _malloc_alloc_oom_handler;
		_malloc_alloc_oom_handler = f;
		return (old);
	}

	
	

};
template<int inst>
void(*_malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = 0;
template<int inst>
void* _malloc_alloc_template<inst>::oom_malloc(size_t n) {
	void(*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (my_malloc_handler == 0) {
			//这里会调用协程处理，本实现暂不考虑
			_THROW_BAD_ALLOC;
		}
		(*my_malloc_handler)();
		result = std::malloc(n);
		if (result) {
			return result;
		}
	}
}
template<int inst>
void* _malloc_alloc_template<inst>::oom_realloc(void*p,size_t n) {
	void(*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (my_malloc_handler == 0) {
			//这里会调用协程处理，本实现暂不考虑
			_THROW_BAD_ALLOC;
		}
		(*my_malloc_handler)();
		result = realloc(p,n);
		if (result) {
			return result;
		}
	}
}
typedef _malloc_alloc_template<0> malloc_alloc;


//第二级分配器，维护一个链表来组织内存碎片

//最小，最大块大小和freelist数量
enum{_ALIGN=8};
enum{_MAX_BYTES=128};
enum{_NFREELISTS=_MAX_BYTES/_ALIGN};
template<bool threads,int inst>
class _default_alloc_template {
private:
	//地址对齐，8的倍数,使用位运算
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+_ALIGN - 1) & ~(_ALIGN - 1));
	}

	union obj
	{
		union obj* free_list_link;
		char client_data[1];
	};

	//创建freelist
	static obj* volatile free_list[_NFREELISTS];
	//请求来时判断使用第几个freelist
	static size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+_ALIGN-1)/_ALIGN-1);
	}

	//返回一个大小为n的对象，并可能加入大小为n的其他区块到freelist
	static void* refill(size_t n) {
		int nobjs = 20;
		//此处传递nobjs的引用，返回结果为实际分配的块数
		char* chunk = chunk_alloc(n, nobjs);
		obj* volatile* my_free_list;
		obj* result;
		obj* current_obj, * next_obj;
		int i;
		if (nobjs == 1)return chunk;
		my_free_list = FREELIST_INDEX(n) + free_list;
		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);
		for (i = 1;; ++i) {
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i) {
				current_obj->free_list_link = 0;
				break;
			}
			else {
				current_obj->free_list_link = next_obj;
			}
		}
		return result;
	}
	//配置一大块空间，可容纳nobjs个大小为size的区块，也可能低于nobjs
	static char* chunk_alloc(size_t n, int& nobjs);
	//维护内存池属性
	static char* start_free;
	static char* end_free;
	static size_t heap_size;
public:
	static void* allocate(size_t n) {
		obj* volatile* my_free_list;
		obj* result;
		//大于最大选择一级配置器
		if (n > _MAX_BYTES) {
			return(malloc_alloc::allocate(n));
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		result = my_free_list;
		//如果没找到则重新填充freelist
		if (result == 0) {
			void* r = refill(ROUND_UP(n));
			return r;
		}
		*my_free_list = result->free_list_link;
		return result;
	}
	static void deallocate(void* p, size_t n) {
		if (n > _MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
		}
		obj* q = (obj*)p;
		obj* volatile* my_free_list;
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;

	}
	static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
		deallocate(p, old_sz);
		p = allocate(new_sz);
		return p;
	}

};

template<bool threads,int inst>
char* _default_alloc_template<threads, inst>::start_free = 0;
template<bool threads, int inst>
char* _default_alloc_template<threads, inst>::end_free = 0;
template<bool threads, int inst>
size_t _default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads, int inst>
typename _default_alloc_template<threads, inst>::obj* volatile _default_alloc_template<threads, inst>::free_list[_NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };

template<bool threads,int inst>
char* _default_alloc_template<threads,inst>::chunk_alloc(size_t size,int& nobjs) {
	char* result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left > total_bytes) {//剩余空间够分
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else if (bytes_left >= size) {//至少能分一个
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else {//一个都分不了，把零头提出来给其他大小的freelist
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);//新申请大小
		if (bytes_left > 0) {
			obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		start_free = (char*)std::malloc(bytes_to_get);
		if (0 == start_free) {//heap空间不足
			int i;
			obj* volatile* my_free_list, * p;
			for (i = size; i <= _MAX_BYTES; i += _ALIGN) {
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != 0) {
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					//拆分大块完成后重新分配
					return (chunk_alloc(size, nobjs));
				}
			}
			end_free = 0;//二级分配器已无内存可用
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);//用一级分配
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return (chunk_alloc(size, nobjs));
	}
}
//一个简单的包装
template<class T,class Alloc>
class simple_alloc {
public:
	static T* allocate(size_t n) {
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void) {
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n) {
		if (0 != n) {
			Alloc::deallocate(p, n * sizeof(T));
		}
	}
	static void deallocate(T* p) {
		Alloc::deallocate(p, sizeof(T));
	}
};
typedef _default_alloc_template<0, 0> alloc;
}


#endif // !XZESTL_ALLOC_H
