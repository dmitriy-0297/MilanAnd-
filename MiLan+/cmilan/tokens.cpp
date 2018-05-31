#include "tokens.h"
static const char * tokenNames_[] = {
	"end of file",
	"illegal token",
	"identifier",
	"number",
	"'BEGIN'",
	"'END'",
	"'IF'",
	"'THEN'",
	"'ELSE'",
	"'FI'",
	"'WHILE'",
	"'DO'",
	"'OD'",
	"'WRITE'",
	"'READ'",
	"':='",
	"'+' or '-'",
	"'*' or '/'",
	"comparison operator",
	"'('",
	"')'",
	"';'",
	"'Ternary'",
	"'Ternary-variant'",
};

const char * tokenToString(Token t)
{
	return tokenNames_[t];
}

