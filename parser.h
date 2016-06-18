#ifndef parser_h_msj16e76apo2jy7i
#define parser_h_msj16e76apo2jy7i

#include <FlexLexer.h>
#include "lexer.h"

class yyParser {
    public:
        void yyparse();
    private:
        yyFlexLexer yylexer;
        int yylexsymb;
        void match(int);

        void program();
        void block();
        void def_and_decl_list();
        void def_or_decl();
        void const_def_part();
        void const_list();
        void const_def();
        void constant();
        void var_decl_part();
        void var_decl_list();
        void var_decl();
        void ident_list();
        void ident_list_0();
        void type();
        void proc_decl();
        void proc_decl_0();
        void proc_decl_1();
        void func_decl();
        void func_decl_0();
        void func_decl_1();
        void formal_param_list();
        void formal_param_sec_list();
        void formal_param_sec_list_0();
        void formal_param_sec();
        void comp_stmt();
        void stmt_seq();
        void stmt_seq_0();
        void stmt();
        void assign_or_proc_stmt();
        void var_assign();
        void while_stmt();
        void for_stmt();
        void if_stmt();
        void else_stmt();
        void dir();
        void expr();
        void expr_0();
        void simple_expr();
        void simple_expr_0();
        void term();
        void term_0();
        void factor();
        void exp();
        void exp_0();
        void primary();
        void primary_0();
        void var_access();
        void var_access_0();
        void actual_param_list();
        void actual_param_list_0();
};

#endif /* parser_h_msj16e76apo2jy7i */
