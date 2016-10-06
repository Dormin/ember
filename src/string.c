#define EMBER_API_EXPORTS
#include "string.h"
#include <string.h>

EMBER_API string StringMake(char* Chars, int Length) {
	string S;
	S.Chars = Chars;
	S.Length = Length;
	return S;
}

EMBER_API string StringFromCString(char* CString) {
	string S;
	S.Chars = CString;
	S.Length = strlen(CString);
	return S;
}

EMBER_API int StringEquals(string S1, string S2) {
	return S1.Length == S2.Length && strncmp(S1.Chars, S2.Chars, S2.Length) == 0;
}

EMBER_API int StringEqualsCString(string S, const char* CString) {
	unsigned int CLength = strlen(CString);
	return S.Length == CLength && strncmp(S.Chars, CString, CLength) == 0;
}
