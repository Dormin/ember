#pragma once
#include "api.h"
#include "ast.h"
#include "error.h"
#include "string.h"
#include "symbol.h"

#ifdef __cplusplus
extern "C" {
#endif

EMBER_API void Analyze(string FileName, ast* Ast, symbol_table* SymbolTable, error_list* ErrorList);

#ifdef __cplusplus
}
#endif
