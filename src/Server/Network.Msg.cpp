#include "Network.hpp"
#include <memory>

Network::Msg::Msg():
	_buf(nullptr, free), _size(0), _user(0)
{
}

Network::Msg::Msg(mptr buf, unsigned int buf_size, unsigned int user):
	_buf(nullptr, free), _size(0), _user(0)
{
	this->_buf = std::move(buf);
	this->_size = buf_size;
	this->_user = user;
}

Network::Msg::mptr Network::Msg::ptr()
{
	return std::move(this->_buf);
}

unsigned int Network::Msg::size()
{
	return this->_size;
}

unsigned int Network::Msg::user()
{
	return this->_user;
}

Network::Msg& Network::Msg::operator= (Msg&& m)
{
	this->_buf = std::move(m._buf);
	this->_size = m._size;
	this->_user = m._user;
	return *this;
}

Network::Msg::Msg(Msg&& m):
	_buf(nullptr, free), _size(0), _user(0)
{
	this->_buf = std::move(m._buf);
	this->_size = m._size;
	this->_user = m._user;
}
