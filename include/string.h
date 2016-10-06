#pragma once
#include "api.h"

typedef struct {
	char* Chars;
	int Length;
} string;

#ifdef __cplusplus
extern "C" {
#endif

EMBER_API string StringMake(char* Chars, int Length);
EMBER_API string StringFromCString(char* CString);
EMBER_API int StringEquals(string S1, string S2);
EMBER_API int StringEqualsCString(string S, const char* CString);

#ifdef __cplusplus
}
#endif
