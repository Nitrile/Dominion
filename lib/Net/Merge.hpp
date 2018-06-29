#pragma once
#include <memory>
#include <vector>

class Merge
{
public:
	typedef std::unique_ptr<char[], void(*)(void*)> mptr;
	Merge(unsigned int num);
	void insert(mptr p, unsigned int pos, unsigned char size);
	bool complete();
	mptr merge();
	unsigned int size();

// private:
	mptr _buf;
	unsigned int _pack;
	unsigned int _size;
	std::vector<unsigned char> _vec;

};
