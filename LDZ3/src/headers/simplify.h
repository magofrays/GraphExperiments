#ifndef SIMPLIFY_H
#define SIMPLIFY_H

class syntaxNode;

syntaxNode *simplify_both(syntaxNode *node);
syntaxNode *simplify_right(syntaxNode *node);
syntaxNode *simplify_left(syntaxNode *node, bool one_var = false);
syntaxNode *simplify_mul(syntaxNode *node);
syntaxNode *simplify_add(syntaxNode *node);
syntaxNode *simplify_sub(syntaxNode *node);
syntaxNode *simplify_minus(syntaxNode *node);
syntaxNode *simplify_pow(syntaxNode *node);
syntaxNode *simplify_div(syntaxNode *node);
syntaxNode *simplify_sin(syntaxNode *node);
syntaxNode *simplify_cos(syntaxNode *node);
syntaxNode *simplify_arcsin(syntaxNode *node);
syntaxNode *simplify_arccos(syntaxNode *node);
syntaxNode *simplify_tg(syntaxNode *node);
syntaxNode *simplify_ctg(syntaxNode *node);
syntaxNode *simplify_arctg(syntaxNode *node);
syntaxNode *simplify_arcctg(syntaxNode *node);
syntaxNode *simplify_sqrt(syntaxNode *node);
syntaxNode *simplify_exp(syntaxNode *node);
syntaxNode *simplify_ln(syntaxNode *node);
syntaxNode *simplify(syntaxNode *node);

#endif