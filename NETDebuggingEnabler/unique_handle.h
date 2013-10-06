#pragma once
#include <wtypes.h>
class unique_handle
{
public:
	unique_handle(HANDLE handle);
	~unique_handle();
	HANDLE get() const
	{
		return this->handle;
	}
private:
	HANDLE handle;
};

