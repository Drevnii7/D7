#pragma once

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) || (defined(_HAS_CXX20) && _HAS_CXX20 == 1)
	#define IS_CPP_20 1
#else
	#define IS_CPP_20 0
#endif