#pragma once
#include "api.h"
#include "ast.h"
#include "error.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

EMBER_API void Parse(string FileName, ast* Ast, error_list* ErrorList);

#ifdef __cplusplus
}
#endif
