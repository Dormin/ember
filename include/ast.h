#pragma once
#include "api.h"
#include "string.h"

#define AST_NODE_POOL_MAX_NUM_NODES 256

typedef enum {
	ast_node_file,
	ast_node_func
} ast_node_kind;

typedef struct ast ast;
typedef struct ast_node ast_node;
typedef struct ast_node_pool ast_node_pool;
typedef struct ast_src ast_src;

struct ast {
	ast_node* FirstFile;
	ast_node_pool* FirstPool;
	ast_src* FirstSrc;
};

struct ast_node {
	ast* A;
	ast_node_kind Kind;
	string FileName;
	int Line;
	int Col;
	
	union {

		struct {
			ast_node* FirstFunc;
			ast_node* LastFunc;
			ast_node* Next;
		} File;

		struct {
			string Name;
			ast_node* Next;
		} Func;
	};
};

struct ast_node_pool {
	ast_node_pool* Next;
	int NumNodes;
	ast_node Nodes[AST_NODE_POOL_MAX_NUM_NODES];
};

struct ast_src {
	char* Src;
	int SrcLen;
	ast_src* Next;
};

#ifdef __cplusplus
extern "C" {
#endif

EMBER_API ast* AstCreate();
EMBER_API void AstDestroy(ast* A);
EMBER_API string AstSrcAlloc(ast* A, int Length);
EMBER_API ast_node* AstAddFile(ast* A, string Name);
EMBER_API ast_node* AstFileAddFunc(ast_node* N, string Name, int Line, int Col);

#ifdef __cplusplus
}
#endif
