#define EMBER_API_EXPORTS
#include "scanner.h"

static void Next(scanner* S) {
	if (S->Ch == '\n') {
		S->Line++;
		S->Col = 0;
	}
	S->Ch = S->Src.Chars[++S->Pos];
	S->Col++;
	S->TokenStr.Length++;
}

EMBER_API void ScannerInit(scanner* S, string Src) {
	S->Src = Src;
	S->Ch = 0;
	S->Pos = -1;
	S->Line = 1;
	S->Col = 0;
	S->Token = token_illegal;
	S->TokenLine = 0;
	S->TokenCol = 0;
	S->TokenStr = StringMake(0, 0);
	Next(S);
	ScannerScan(S);
}

static int IsAtEnd(scanner* S) {
	return S->Pos >= S->Src.Length;
}

static int IsLetter(char Ch) {
	return 'a' <= Ch && Ch <= 'z' || 'A' <= Ch && Ch <= 'Z' || Ch == '_';
}

static int IsDigit(char Ch) {
	return '0' <= Ch && Ch <= '9';
}

static int TokenStrIs(scanner* S, const char* CString) {
	return StringEqualsCString(S->TokenStr, CString);
}

EMBER_API void ScannerScan(scanner* S) {
	// Skip whitespace
	while (!IsAtEnd(S) && (S->Ch == ' ' || S->Ch == '\t' || S->Ch == '\n' || S->Ch == '\r')) {
		Next(S);
	}

	S->Token = token_illegal;
	S->TokenLine = S->Line;
	S->TokenCol = S->Col;
	S->TokenStr.Chars = &S->Src.Chars[S->Pos];
	S->TokenStr.Length = 0;

	if (IsAtEnd(S)) {

		S->Token = token_end;

	} else if (IsLetter(S->Ch)) {
		// Scan keyword/identifier
		while (!IsAtEnd(S) && (IsLetter(S->Ch) || IsDigit(S->Ch))) {
			Next(S);
		}

		if (TokenStrIs(S, "func")) {
			S->Token = token_func;
		} else {
			S->Token = token_ident;
		}

	} else {

		char Ch = S->Ch;
		Next(S);

		switch (Ch) {
		case '(':
			S->Token = token_lparen;
			break;
		case ')':
			S->Token = token_rparen;
			break;
		case '{':
			S->Token = token_lbrace;
			break;
		case '}':
			S->Token = token_rbrace;
			break;
		}
	}
}
