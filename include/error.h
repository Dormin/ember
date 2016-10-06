#pragma once
#include "api.h"
#include "string.h"
#include "token.h"

typedef enum {
	error_file,
	error_token,
	error_redefined
} error_kind;

typedef struct {
	error_kind Kind;
	string FileName;
	int Line;
	int Col;

	union {

		struct {
			token Token;
			string String;
		} Token;

		struct {
			string Name;
		} Redefined;
	};

} error;

typedef struct {
	int MaxLength;
	int Length;
	error* Entries;
} error_list;

#ifdef __cplusplus
extern "C" {
#endif

EMBER_API error_list* ErrorListCreate(int MaxLength);
EMBER_API void ErrorListDestroy(error_list* L);
EMBER_API error* Error(error_list* L, error_kind Kind);

#ifdef __cplusplus
}
#endif
