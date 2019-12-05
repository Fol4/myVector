#pragma once
#include<algorithm>
#include <memory>
#include <iterator>

template <typename T, typename alloc>
struct vectorBase
{
	alloc allo;
	T* elem;
	size_t sz;
	size_t space;

	vectorBase(alloc a, size_t n)
		: allo{ a }, elem{ a.allocate(n) }, sz{ n }, space{ n }{}

	vectorBase(vectorBase&& arg);

	vectorBase& operator=(vectorBase&& arg);

	~vectorBase() { allo.deallocate(elem, space); }
};

template <typename T, typename alloc>
vectorBase<T, alloc>::vectorBase(vectorBase&& arg)
{
	arg.sz = 0;
	arg.elem = nullptr;
	arg.space = 0;
}

template <typename T, typename alloc>
vectorBase<T, alloc>& vectorBase<T, alloc>::operator=(vectorBase&& arg)
{
	swap(this->elem, arg.elem);
	swap(this->sz, arg.sz);
	swap(this->space, arg.space);
	return *this;
}

template<typename T, typename alloc = std::allocator<T>>
class myvector : private vectorBase<T, alloc>
{
public:
	explicit myvector(size_t n, T d = T(), alloc allo = alloc());

	myvector(std::initializer_list<T> lst, const alloc& allo = alloc());

	myvector(const alloc& allo = alloc()) : vectorBase<T, alloc>(allo, 1) {}

	myvector(const myvector& arg);
	myvector& operator= (const myvector& v);

	myvector(myvector&& a) = default;
	myvector& operator= (myvector && a) = default;

	~myvector();

	T& at(size_t n);
	const T& at(size_t n) const;

	T& operator[] (size_t n);
	const T& operator[] (size_t n) const;

	size_t size() const { return this->sz; }

	size_t capacity() const { return this->space; }

	void reserve(size_t newalloc);

	void resize(size_t & newsize, T val = T());

	void push_back(const T & d);
};

template <typename T, typename alloc>
myvector<T, alloc>::myvector(size_t n, T d, alloc allo)
	:vectorBase<T, alloc>(allo, n)
{
	for (int i = 0; i < n; ++i)
		this->allo.construct(&this->elem[i], d);
}

template <typename T, typename alloc>
myvector<T, alloc>::myvector(std::initializer_list<T> lst, const alloc& allo)
	:vectorBase<T, alloc>(allo, lst.size())
{
	for (int i = 0; i < lst.size(); ++i)
		this->allo.construct(&this->elem[i], *(lst.begin() + 1));
}

template <typename T, typename alloc>
myvector<T, alloc>::~myvector()
{
	for (int i = 0; i < this->sz; ++i)
		this->allo.destroy(this->elem + i);
}

template <typename T, typename alloc>
myvector<T, alloc>::myvector(const myvector& arg)
	:vectorBase<T, alloc>(arg.allo, arg.sz)
{
	for (int i = 0; i < arg.sz; ++i)
		this->allo.construct(&this->elem[i], arg[i]);
}

template <typename T, typename alloc>
myvector<T, alloc>& myvector<T, alloc>::operator=(const myvector<T, alloc>& arg)
{
	if (this == &arg)
		return *this;
	vectorBase<T, alloc> b{ this->allo, arg.sz };
	uninitialized_copy(arg.begin(), arg.end(), b.elem);
	swap(this->elem, b.elem);
	swap(this->sz, b.sz);
	swap(this->space, b.space);
	for (size_t i = 0; i < b.sz; ++i)
		b.allo.destroy(b.elem + i);//free memory used for vector before
	return *this;
}

template <typename T, typename alloc>
void myvector<T, alloc>::reserve(size_t newalloc)
{
	if (newalloc <= this->space) return;
	vectorBase<T, alloc> b(this->allo, newalloc);
	uninitialized_copy(b.elem, &b.elem[this->sz], this->elem);
	for (size_t i = 0; i < this->sz; ++i)
	{
		this->allo.destroy(&this->elem[i]);
	}
	swap(this->sz, b.sz);
	swap(this->space, b.space);
	swap(this->allo, b.allo);
	swap(this->elem, b.elem);
}
template <typename T, typename alloc>
void myvector<T, alloc>::push_back(const T& val)
{
	if (!this->space)
		reserve(8);
	else if (this->sz == this->space)
		reserve(2 * this->space);
	this->allo.construct(&this->elem[this->sz], val);
	++this->sz;
}

template <typename T, typename alloc>
void myvector<T, alloc>::resize(size_t& newsize, T val)
{
	reserve(newsize);
	for (size_t i = this->sz; i < newsize; ++i) this->allo.construct(&this->elem[i], val);
	for (size_t i = newsize; i < this->sz; ++i) this->allo.destroy(&this->elem[i]);
	this->sz = newsize;
}

template <typename T, typename alloc>
T& myvector<T, alloc>::at(size_t n)
{
	if (n < 0 || this->sz <= n) throw sstd::out_of_range("Out of range");
	return this->elem[n];
}

template <typename T, typename alloc>
const T& myvector<T, alloc>::at(size_t n) const
{
	if (n < 0 || this->sz <= n) throw std::out_of_range("Out of range");
	return this->elem[n];
}

template <typename T, typename alloc>
T& myvector<T, alloc>::operator[] (size_t n)
{
	return this->elem[n];
}

template <typename T, typename alloc>
const T& myvector<T, alloc>::operator[] (size_t n) const
{
	return this->elem[n];
}