#pragma once

typedef enum {
	token_illegal,
	token_end,
	token_ident,
	token_func,
	token_lparen,
	token_rparen,
	token_lbrace,
	token_rbrace
} token;
