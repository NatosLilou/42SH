#include "eval.h"

int eval_rule_case(struct ast_rule_case *ast)
{
    char *pattern = NULL;
    char *real_word = expand(ast->word);
    ast->word = NULL;
    if (ast->case_clause)
    {
        for (size_t i = 0; i < ast->case_clause->pos; i++)
        {
            for (size_t j = 0; j < ast->case_clause->case_item[i]->pos; j++)
            {
                pattern = expand(ast->case_clause->case_item[i]->words[j]);
                ast->case_clause->case_item[i]->words[j] = NULL;

                if (fnmatch(pattern, real_word, 0) == 0)
                {
                    free(pattern);
                    free(real_word);
                    if (ast->case_clause->case_item[i]->compound_list)
                        return eval_compound_list(ast->case_clause->case_item[i]->compound_list);
                    return 0;
                }
                free(pattern);
            }
        }
    }
    free(pattern);
    free(real_word);
    return 0;
}
