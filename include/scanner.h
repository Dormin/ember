#pragma once
#include "api.h"
#include "string.h"
#include "token.h"

typedef struct {
	string Src;
	char Ch;
	int Pos;
	int Line;
	int Col;
	token Token;
	int TokenLine;
	int TokenCol;
	string TokenStr;
} scanner;

#ifdef __cplusplus
extern "C" {
#endif

EMBER_API void ScannerInit(scanner* S, string Src);
EMBER_API void ScannerDeinit(scanner* S);
EMBER_API void ScannerScan(scanner* S);

#ifdef __cplusplus
}
#endif
