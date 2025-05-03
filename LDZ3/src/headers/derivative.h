#ifndef DERIVATIVE_H
#define DERIVATIVE_H
#include "syntax_node.h"
#include "simplify.h"
#include "decoder.h"

syntaxNode *diff_mul(syntaxNode *node);
syntaxNode *diff_div(syntaxNode *node);

syntaxNode *diff_exp(syntaxNode *node);
syntaxNode *diff_pow(syntaxNode *node);

syntaxNode *diff_ln(syntaxNode *node);
syntaxNode *diff_sin(syntaxNode *node);
syntaxNode *diff_cos(syntaxNode *node);
syntaxNode *diff_minus(syntaxNode *node);

syntaxNode *diff(syntaxNode *node);

#endif