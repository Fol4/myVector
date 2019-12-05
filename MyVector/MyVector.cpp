#include "MyVector.h"

myvector::myvector(const myvector& nw)
	:sz{ nw.sz },
	elem{ new double[nw.sz] }
{
	std::copy(nw.elem, nw.elem + sz, elem);//    copy(arg,arg`+sz,elem);
}

myvector& myvector::operator= (const myvector& rhs)
{
	double* p = new double[rhs.sz];
	std::copy(rhs.elem, rhs.elem + rhs.sz, p);
	delete[] elem;
	elem = p;
	sz = rhs.sz;
	return *this;
}

myvector::myvector(myvector&& nw)
	:sz{ nw.sz },
	elem{ nw.elem }
{
	nw.sz = 0;
	nw.elem = nullptr;
}

myvector& myvector::operator= (myvector&& rhs)
{
	delete[] elem;
	elem = rhs.elem;
	sz = rhs.sz;
	rhs.sz = 0;
	rhs.elem = nullptr;
	return *this;
}