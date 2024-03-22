#pragma once

template<typename... Args>
class Listener
{
public:
	virtual void notify(Args... args) = 0;
};