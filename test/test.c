#include "ember.h"
#include <stdio.h>

#define MAX_NUM_ERRORS 10

typedef struct {
	const char* FileName;
} success_test;

typedef struct {
	const char* FileName;
	error_kind ErrorKind;
} error_test;

static success_test SuccessTests[] = {
	{ "test/success/main.e" },
	{ "test/success/multiple_funcs.e" }
};

static error_test ErrorTests[] = {
	{ "test/error/func_redefined.e", error_redefined },
	{ "test/error/missing_file.e", error_file }
};

#define NUM_SUCCESS_TESTS (sizeof(SuccessTests) / sizeof(success_test))
#define NUM_ERROR_TESTS (sizeof(ErrorTests) / sizeof(error_test))

typedef enum {
	test_type_success,
	test_type_error
} test_type;

static void PrintErrors(error_list* ErrorList) {
	for (int i = 0; i < ErrorList->Length; i++) {
		error* E = &ErrorList->Entries[i];

		switch (E->Kind) {
		case error_file: {
			printf("could not read file %.*s\n", E->FileName.Length, E->FileName.Chars);
		} break;
		case error_token: {
			printf( "%.*s:%d:%d: unexpected ",
				E->FileName.Length, E->FileName.Chars,
				E->Line, E->Col
				);
			if (E->Token.Token == token_end) {
				printf("end of file\n");
			} else {
				printf("%.*s\n", E->Token.String.Length, E->Token.String.Chars);
			}
		} break;
		case error_redefined: {
			printf("%.*s:%d:%d: %.*s is already defined\n",
				E->FileName.Length, E->FileName.Chars,
				E->Line, E->Col,
				E->Redefined.Name.Length, E->Redefined.Name.Chars
				);
		} break;
		}
	}
}

static int DoTest(test_type Type, string FileName, error_kind ErrorKind) {
	ast* Ast = AstCreate();
	symbol_table* SymbolTable = SymbolTableCreate();
	error_list* ErrorList = ErrorListCreate(MAX_NUM_ERRORS);

	printf("%.*s... ", FileName.Length, FileName.Chars);
	Analyze(FileName, Ast, SymbolTable, ErrorList);
	
	int Passed = (Type == test_type_success && ErrorList->Length == 0) ||
		(Type == test_type_error && ErrorList->Length == 1 &&
		ErrorList->Entries[0].Kind == ErrorKind);

	if (Passed) {
		printf("passed!\n");
	} else {
		printf("FAILED!\n");
		PrintErrors(ErrorList);
	}

	ErrorListDestroy(ErrorList);
	SymbolTableDestroy(SymbolTable);
	AstDestroy(Ast);
	return Passed;
}

int main() {

	// Success tests
	for (int i = 0; i < NUM_SUCCESS_TESTS; i++) {
		success_test* Test = &SuccessTests[i];
		string FileName = StringFromCString((char*)Test->FileName);
		if (!DoTest(test_type_success, FileName, 0)) {
			return 1;
		}
	}

	// Error tests
	for (int i = 0; i < NUM_ERROR_TESTS; i++) {
		error_test* Test = &ErrorTests[i];
		string FileName = StringFromCString((char*)Test->FileName);
		if (!DoTest(test_type_error, FileName, Test->ErrorKind)) {
			return 1;
		}
	}

	return 0;
}
