#include "unique_handle.h"


unique_handle::unique_handle(HANDLE handle)
{
	this->handle = handle;
}


unique_handle::~unique_handle()
{
	if (this->handle != INVALID_HANDLE_VALUE && this->handle != 0)
		CloseHandle(this->handle);
}
