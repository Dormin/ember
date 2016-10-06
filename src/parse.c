#define EMBER_API_EXPORTS
#include "api.h"
#include "ast.h"
#include "error.h"
#include "scanner.h"
#include "string.h"
#include "token.h"
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

// Set up recovery point
#define RECOVERY(P) setjmp((P)->JmpBuf)
// Jump to recovery point
#define PANIC(P) longjmp((P)->JmpBuf, 1)

typedef struct {
	string FileName;
	ast* Ast;
	error_list* ErrorList;
	int LastErrorLine;
	scanner Scanner;
	jmp_buf JmpBuf;
} parser;

static token GetToken(parser* P) {
	return P->Scanner.Token;
}

static int GetTokenLine(parser* P) {
	return P->Scanner.TokenLine;
}

static int GetTokenCol(parser* P) {
	return P->Scanner.TokenCol;
}

static string GetTokenStr(parser* P) {
	return P->Scanner.TokenStr;
}

static void Advance(parser* P) {
	ScannerScan(&P->Scanner);
}

static int NextIs(parser* P, token Tok) {
	return GetToken(P) == Tok;
}

static error* ParseError(parser* P, error_kind Kind) {
	error* E = Error(P->ErrorList, Kind);
	if (E == 0) {
		PANIC(P);
	}
	E->FileName = P->FileName;
	return E;
}

static void TokenError(parser* P) {
	if (GetTokenLine(P) == P->LastErrorLine) {
		return;
	}
	error* E = ParseError(P, error_token);
	E->Line = GetTokenLine(P);
	E->Col = GetTokenCol(P);
	E->Token.Token = GetToken(P);
	E->Token.String = GetTokenStr(P);
	P->LastErrorLine = E->Line;
	if (NextIs(P, token_end)) {
		PANIC(P);
	}
}

static void FileError(parser* P) {
	ParseError(P, error_file);
	PANIC(P);
}

static void Expect(parser* P, token Tok) {
	if (!NextIs(P, Tok)) {
		TokenError(P);
	}
	Advance(P);
}

static void ParseFunc(parser* P, ast_node* N) {
	int Line = GetTokenLine(P);
	int Col = GetTokenCol(P);
	Expect(P, token_func);
	string Name = GetTokenStr(P);
	Expect(P, token_ident);
	Expect(P, token_lparen);
	Expect(P, token_rparen);
	Expect(P, token_lbrace);
	Expect(P, token_rbrace);
	AstFileAddFunc(N, Name, Line, Col);
}

static void ParseFuncs(parser* P, ast_node* N) {
	while (!NextIs(P, token_end)) {
		ParseFunc(P, N);
	}
}

static string ReadSrc(parser* P, string FileName) {
	// FileName.Chars is not garanteed to be null-terminated
	// so make null-terminated copy
	char* FN = (char*)malloc(FileName.Length + 1);
	memcpy(FN, FileName.Chars, FileName.Length);
	FN[FileName.Length] = '\0';

	FILE* File = fopen(FN, "rb");
	if (File == 0) {
		FileError(P);
	}

	// Get file length
	fseek(File, 0, SEEK_END);
	long Length = ftell(File);
	rewind(File);

	// Read file
	string S = AstSrcAlloc(P->Ast, Length);
	if (fread(S.Chars, 1, Length, File) != Length) {
		fclose(File);
		FileError(P);
	}

	fclose(File);
	return S;
}

EMBER_API void Parse(string FileName, ast* Ast, error_list* ErrorList) {
	parser P;
	if (!RECOVERY(&P)) {
		P.FileName = FileName;
		P.Ast = Ast;
		P.ErrorList = ErrorList;
		P.LastErrorLine = -1;
		ScannerInit(&P.Scanner, ReadSrc(&P, FileName));
		ast_node* N = AstAddFile(P.Ast, FileName);
		ParseFuncs(&P, N);
	}
}
