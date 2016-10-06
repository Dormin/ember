#define EMBER_API_EXPORTS
#include "ast.h"
#include <malloc.h>

static void AstNodePoolAlloc(ast* A) {
	ast_node_pool* Pool = (ast_node_pool*)malloc(sizeof(ast_node_pool));
	Pool->NumNodes = 0;
	Pool->Next = A->FirstPool;
	A->FirstPool = Pool;
}

static void AstNodePoolFree(ast_node_pool* Pool) {
	if (Pool != 0) {
		AstNodePoolFree(Pool->Next);
		free(Pool);
	}
}

static ast_node* AstNodeAlloc(ast* A) {
	if (A->FirstPool->NumNodes == AST_NODE_POOL_MAX_NUM_NODES) {
		AstNodePoolAlloc(A);
	}
	ast_node_pool* Pool = A->FirstPool;
	return &Pool->Nodes[Pool->NumNodes++];
}

static ast_node* AstNodeMake(ast* A, ast_node_kind Kind, string FileName, int Line, int Col) {
	ast_node* N = AstNodeAlloc(A);
	N->A = A;
	N->Kind = Kind;
	N->FileName = FileName;
	N->Line = Line;
	N->Col = Col;
	return N;
}

static void AstSrcFree(ast_src* Src) {
	if (Src != 0) {
		AstSrcFree(Src->Next);
		free(Src->Src);
		free(Src);
	}
}

EMBER_API ast* AstCreate() {
	ast* A = (ast*)malloc(sizeof(ast));
	A->FirstFile = 0;
	A->FirstPool = 0;
	A->FirstSrc = 0;
	AstNodePoolAlloc(A);
	return A;
}

EMBER_API void AstDestroy(ast* A) {
	if (A != 0) {
		AstNodePoolFree(A->FirstPool);
		AstSrcFree(A->FirstSrc);
		free(A);
	}
}

EMBER_API string AstSrcAlloc(ast* A, int Length) {
	ast_src* S = (ast_src*)malloc(sizeof(ast_src));
	S->Src = (char*)malloc(Length);
	S->SrcLen = Length;
	S->Next = A->FirstSrc;
	A->FirstSrc = S;
	return StringMake(S->Src, Length);
}

EMBER_API ast_node* AstAddFile(ast* A, string Name) {
	ast_node* N = AstNodeMake(A, ast_node_file, Name, 0, 0);
	N->File.FirstFunc = 0;
	N->File.LastFunc = 0;
	N->File.Next = A->FirstFile;
	A->FirstFile = N;
	return N;
}

EMBER_API ast_node* AstFileAddFunc(ast_node* N, string Name, int Line, int Col) {
	ast_node* N2 = AstNodeMake(N->A, ast_node_func, N->FileName, Line, Col);
	N2->Func.Name = Name;
	N2->Func.Next = 0;
	if (N->File.FirstFunc == 0) {
		N->File.FirstFunc = N2;
	} else {
		N->File.LastFunc->Func.Next = N2;
	}
	N->File.LastFunc = N2;
	return N2;
}
