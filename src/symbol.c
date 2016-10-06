#define EMBER_API_EXPORTS
#include "symbol.h"
#include <malloc.h>

#define INITIAL_CAPACITY 256

EMBER_API symbol_table* SymbolTableCreate() {
	symbol_table* T = (symbol_table*)malloc(sizeof(symbol_table));
	T->Size = 0;
	T->Capacity = INITIAL_CAPACITY;
	T->Entries = (symbol*)malloc(T->Capacity * sizeof(symbol));
	return T;
}

EMBER_API void SymbolTableDestroy(symbol_table* T) {
	if (T != 0) {
		free(T->Entries);
		free(T);
	}
}

EMBER_API int SymbolExists(symbol_table* T, string Name) {
	return SymbolGet(T, Name) != 0;
}

EMBER_API void SymbolAdd(symbol_table* T, string Name, ast_node* Node) {
	if (T->Size == T->Capacity) {
		T->Capacity += T->Capacity * 3 / 2;
		T->Entries = (symbol*)realloc(T->Entries, T->Capacity * sizeof(symbol));
	}
	symbol* S = &T->Entries[T->Size++];
	S->Name = Name;
	S->Node = Node;
}

EMBER_API ast_node* SymbolGet(symbol_table* T, string Name) {
	for (int i = 0; i < T->Size; i++) {
		symbol* S = &T->Entries[i];
		if (StringEquals(S->Name, Name)) {
			return S->Node;
		}
	}
	return 0;
}
