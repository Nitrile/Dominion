#pragma once

#include <memory>

class Msg
{
public:
	typedef std::unique_ptr<char[], void(*)(void*)> mptr;
	Msg();
	Msg(mptr buf, unsigned int buf_size, unsigned int user);
	Msg(const Msg&) = delete;
	Msg(Msg&& m);

	Msg& operator= (const Msg&) = delete;
	Msg& operator= (Msg&& m);

	mptr _buf;
	unsigned int _size;
	unsigned int _user;
};