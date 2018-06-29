#include "Msg.hpp"
#include <memory>

Msg::Msg():
	_buf(nullptr, free), _size(0), _user(0)
{
}

Msg::Msg(mptr buf, unsigned int buf_size, unsigned int user):
	_buf(std::move(buf)), _size(buf_size), _user(user)
{
}

Msg::Msg(Msg&& m):
	_buf(std::move(m._buf)), _size(m._size), _user(m._user)
{
}

Msg& Msg::operator= (Msg&& m)
{
	this->_buf = std::move(m._buf);
	this->_size = m._size;
	this->_user = m._user;
	return *this;
}
