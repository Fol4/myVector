#pragma once
#include <iostream>
#include<algorithm>
#include <memory>
#include <initializer_list>
#include <iterator>
using namespace std;



template <typename T, typename alloc>
struct vectorBase
{
	using size_type = size_t;
	alloc a;
	T* elem;
	size_type sz;
	size_type space;

	vectorBase(alloc a, size_type n)
		: a{ a }, elem{ a.allocate(n) }, sz{ n }, space{ n }{}

	~vectorBase() {
		a.deallocate(elem, space);
	}
};

template<typename T, typename alloc = allocator<T>>
class myvector : private vectorBase<T, alloc>
{
public:
	using size_type = typename vectorBase<T, alloc>::size_type;

	explicit myvector(size_type n, T d = T(), alloc a = alloc());

	myvector(initializer_list<T> lst, const alloc& a = alloc());

	myvector(const alloc& a = alloc()) : vectorBase<T, alloc>(a, 0) {}

	myvector(const myvector& arg);
	myvector& operator= (const myvector& v);

	myvector(myvector&& a);
	myvector& operator= (myvector&& a);

	~myvector();

	T& at(size_type n);
	const T& at(size_type n) const;

	T& operator[] (size_type n);
	const T& operator[] (size_type n) const;

	size_type size() const { return this->sz; }

	size_type capacity() const { return this->space; }

	void reserve(size_type newalloc);

	void resize(size_type& newsize, T val = T());

	void push_back(const T& d);
};

template <typename T, typename alloc>
myvector<T, alloc>::myvector(size_type n, T d, alloc a)
	:vectorBase<T, alloc>(a, n)
{
	for (size_type i = 0; i < n; ++i)
		this->a.construct(&this->elem[i], d);
}

template <typename T, typename alloc>
myvector<T, alloc>::myvector(initializer_list<T> lst, const alloc& a)
	:vectorBase<T, alloc>(a, lst.size())
{
	for (size_type i = 0; i < lst.size(); ++i)
		this->a.construct(&this->elem[i], *(lst.begin() + i));
}

template <typename T, typename alloc>
myvector<T, alloc>::~myvector()
{
	for (size_type i = 0; i < this->sz; ++i)
		this->a.destroy(this->elem + i);
}

template <typename T, typename alloc>
myvector<T, alloc>::myvector(const myvector& arg)
	:vectorBase<T, alloc>(arg.a, arg.sz)
{
	for (size_type i = 0; i < arg.sz; ++i)
		this->a.construct(&this->elem[i], arg[i]);
}

template <typename T, typename alloc>
myvector<T, alloc>& myvector<T, alloc>::operator=(const myvector<T, alloc>& arg)
{
	if (this == &arg)
		return *this;
	vectorBase<T, alloc> b(this->a, arg.sz);
	uninitialized_copy(&arg[0], &arg[arg.size()], b.elem);
	swap(this->elem, b.elem);
	swap(this->sz, b.sz);
	swap(this->space, b.space);
	for (size_type i = 0; i < b.sz; ++i)
		b.a.destroy(b.elem + i);//free memory used for vector before
	return *this;
}

template <typename T, typename alloc>
myvector<T, alloc>::myvector(myvector&& arg)
	:vectorBase<T, alloc>(arg.a, arg.sz)
{
	arg.sz = 0;
	arg.elem = nullptr;
	arg.space = 0;
	std::cout << endl << "yes" << endl << endl;
}

template <typename T, typename alloc>
myvector<T, alloc>& myvector<T, alloc>::operator=(myvector<T, alloc>&& arg)
{
	swap(this->elem, arg.elem);
	swap(this->sz, arg.sz);
	swap(this->space, arg.space);
	delete[] arg.elem;
	std::cout << endl << "yes" << endl << endl;
	return *this;
}

template <typename T, typename alloc>
void myvector<T, alloc>::reserve(size_type newalloc)
{
	if (newalloc <= this->space) return;
	vectorBase<T, alloc> b(this->a, newalloc);
	uninitialized_copy(&this->elem[0], &this->elem[this->sz], b.elem);
	for (size_type i = 0; i < this->sz; ++i)
	{
		this->a.destroy(&this->elem[i]);
	}
	b.sz = this->sz;
	swap(this->elem, b.elem);
	swap(this->sz, b.sz);
	swap(this->space, b.space);
}

template <typename T, typename alloc>
void myvector<T, alloc>::push_back(const T& val)
{
	if (!(this->space))
		reserve(8);
	else if (this->sz == this->space)
		reserve(2 * this->space);
	this->a.construct(&this->elem[this->sz], val);
	++this->sz;
}

template <typename T, typename alloc>
void myvector<T, alloc>::resize(size_type& newsize, T val)
{
	reserve(newsize);
	for (size_type i = this->sz; i < newsize; ++i) this->a.construct(&this->elem[i], val);
	for (size_type i = newsize; i < this->sz; ++i) this->a.destroy(&this->elem[i]);
	this->sz = newsize;
}

template <typename T, typename alloc>
T& myvector<T, alloc>::at(size_type n)
{
	if (n < 0 || this->sz <= n) throw out_of_range("Out of range");
	return this->elem[n];
}

template <typename T, typename alloc>
const T& myvector<T, alloc>::at(size_type n) const
{
	if (n < 0 || this->sz <= n) throw out_of_range("Out of range");
	return this->elem[n];
}

template <typename T, typename alloc>
T& myvector<T, alloc>::operator[] (size_type n)
{
	return this->elem[n];
}

template <typename T, typename alloc>
const T& myvector<T, alloc>::operator[] (size_type n) const
{
	return this->elem[n];
}

template <class T>
void print(const myvector<T>& src)
{
	for (auto i = 0; i < src.size(); ++i) {
		cout << src[i] << " ";
	}
	cout << endl;
}