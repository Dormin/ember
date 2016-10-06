#pragma once
#include "api.h"
#include "ast.h"
#include "string.h"

// IMPORTANT!!
// The symbol table is currently implemented as a linear search table,
// at some point it should be replaced by a hash table implementation.

typedef struct {
	string Name;
	ast_node* Node;
} symbol;

typedef struct {
	int Size;
	int Capacity;
	symbol* Entries;
} symbol_table;

#ifdef __cplusplus
extern "C" {
#endif

EMBER_API symbol_table* SymbolTableCreate();
EMBER_API void SymbolTableDestroy(symbol_table* T);
EMBER_API int SymbolExists(symbol_table* T, string Name);
EMBER_API void SymbolAdd(symbol_table* T, string Name, ast_node* Node);
EMBER_API ast_node* SymbolGet(symbol_table* T, string Name);

#ifdef __cplusplus
}
#endif
