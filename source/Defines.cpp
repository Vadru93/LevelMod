#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Defines.h"

void _printf(const char* str, ...)
{
	//#if 1
	va_list va_alist;
	va_start(va_alist, str);
	char logbuf[512] = { 0 };
	_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), str, va_alist);
	va_end(va_alist);
	printf(logbuf);
	//printf(str, ...);
//#endif
}