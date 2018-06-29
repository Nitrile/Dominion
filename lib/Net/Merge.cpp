#include "Merge.hpp"
#include <memory.h>
#include <iostream>

Merge::Merge(unsigned int num):
	_buf((char*)malloc(num*248), free),
	_pack(0U),
	_size(0U),
	_vec(num, 0U)
{
}

void Merge::insert(mptr p, unsigned int pos, unsigned char size)
{
	if ((this->_vec)[pos] != 0)
	{
		this->_size -= (this->_vec)[pos];
		(this->_pack)--;
	}
	(this->_vec)[pos] = size;
	(this->_pack)++;
	this->_size += (unsigned int) size;
	memcpy((this->_buf).get() + pos*248, p.get(), size);
}

bool Merge::complete()
{
	return (this->_pack) == (this->_vec).size();
}

Merge::mptr Merge::merge()
{
	return std::move(this->_buf);
}

unsigned int Merge::size()
{
	return this->_size;
}