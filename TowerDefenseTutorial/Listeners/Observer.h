#pragma once

template<typename... Args>
class Observer
{
public:
	
	virtual void observe(Args... args) = 0;
};