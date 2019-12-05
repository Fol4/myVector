#pragma once
#include<algorithm>

class myvector
{
public:
	explicit myvector(unsigned long long s)
		:sz{ s },
		elem{ new double[s] }
	{
		for (unsigned long long i = 0; i < sz; ++i)
				elem[i] = 0;
	}

	myvector(std::initializer_list<double> lst)
		:sz{ lst.size() },
		elem{ new double[sz] }
	{
		std::copy(lst.begin(), lst.end(), elem);
	}

	myvector(const myvector& nw);
	myvector& operator= (const myvector&);
	double& operator[] (unsigned long long n) { return elem[n]; }
	double operator[] (unsigned long long n) const { return elem[n]; }
	myvector(myvector&& nw);
	myvector& operator= (myvector&&);

	~myvector()
	{
		delete[] elem;
	}

	unsigned long long size() const { return sz; }
	double get(unsigned long long n) const { return elem[n]; }
	void set(unsigned long long n, double v) { elem[n] = v; }

private:
	unsigned long long sz;
	double* elem;
};