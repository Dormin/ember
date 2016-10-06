#define EMBER_API_EXPORTS
#include "api.h"
#include "ast.h"
#include "error.h"
#include "parse.h"
#include "string.h"
#include "symbol.h"
#include <setjmp.h>
#include <malloc.h>

// Set up recovery point
#define RECOVERY(A) setjmp((A)->JmpBuf)
// Jump to recovery point
#define PANIC(A) longjmp((A)->JmpBuf, 1)

typedef struct {
	ast* Ast;
	symbol_table* SymbolTable;
	error_list* ErrorList;
	jmp_buf JmpBuf;
} analyzer;

static error* AnalyzeError(analyzer* A, error_kind Kind, ast_node* N) {
	error* E = Error(A->ErrorList, Kind);
	if (E == 0) {
		PANIC(A);
	}
	E->FileName = N->FileName;
	E->Line = N->Line;
	E->Col = N->Col;
	return E;
}

static void RedefinedError(analyzer* A, string Name, ast_node* N) {
	error* E = AnalyzeError(A, error_redefined, N);
	E->Redefined.Name = Name;
}

static void AnalyzeDecl(analyzer* A, ast_node* N) {
	if (SymbolExists(A->SymbolTable, N->Func.Name)) {
		RedefinedError(A, N->Func.Name, N);
	} else {
		SymbolAdd(A->SymbolTable, N->Func.Name, N);
	}
}

static void AnalyzeDecls(analyzer* A, ast_node* N) {
	ast_node* Curr = N->File.FirstFunc;
	while (Curr != 0) {
		AnalyzeDecl(A, Curr);
		Curr = Curr->Func.Next;
	}
}

static void AnalyzeFile(analyzer* A, ast_node* N) {
	AnalyzeDecls(A, N);
}

static void AnalyzeFiles(analyzer* A) {
	AnalyzeFile(A, A->Ast->FirstFile);
}

EMBER_API void Analyze(string FileName, ast* Ast, symbol_table* SymbolTable, error_list* ErrorList) {
	Parse(FileName, Ast, ErrorList);
	if (ErrorList->Length > 0) {
		return;
	}
	analyzer A;
	if (!RECOVERY(&A)) {
		A.Ast = Ast;
		A.SymbolTable = SymbolTable;
		A.ErrorList = ErrorList;
		AnalyzeFiles(&A);
	}
}
