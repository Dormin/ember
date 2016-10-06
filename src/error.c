#define EMBER_API_EXPORTS
#include "error.h"
#include <malloc.h>

EMBER_API error_list* ErrorListCreate(int MaxLength) {
	error_list* L = (error_list*)malloc(sizeof(error_list));
	L->MaxLength = MaxLength;
	L->Length = 0;
	L->Entries = (error*)malloc(MaxLength * sizeof(error));
	return L;
}

EMBER_API void ErrorListDestroy(error_list* L) {
	if (L != 0) {
		free(L->Entries);
		free(L);
	}
}

EMBER_API error* Error(error_list* L, error_kind Kind) {
	if (L->Length >= L->MaxLength) {
		return 0;
	}
	error* E = &L->Entries[L->Length++];
	E->Kind = Kind;
	return E;
}
