#ifndef parser_h_msj16e76apo2jy7i
#define parser_h_msj16e76apo2jy7i

#include <memory>
#include <string>
#include <FlexLexer.h>
#include "lexer.h"
#include "ast.h"

class yyParser {
    public:
        ast::node *yyparse();
    private:
        yyFlexLexer yylexer;
        int yylexsymb;
        void match(int);
        std::string get_ident();

        ast::node *program();
        ast::block *block();
        ast::decl_list *def_and_decl_list();
        ast::decl *def_or_decl();
        ast::decl_list *const_def_part();
        ast::decl_list *const_list();
        ast::decl *const_def();
        int constant();
        ast::decl_list *var_decl_part();
        ast::decl_list *var_decl_list();
        ast::decl_list *var_decl();
        ast::decl_list *ident_list();
        ast::decl_list *ident_list_0();
        std::shared_ptr<ast::type> type();
        ast::decl_list *proc_decl();
        void proc_decl_0();
        void proc_decl_1();
        ast::decl_list *func_decl();
        void func_decl_0();
        void func_decl_1();
        void formal_param_list();
        void formal_param_sec_list();
        void formal_param_sec_list_0();
        void formal_param_sec();
        ast::compound_stmt *comp_stmt();
        ast::stmt_list *stmt_seq();
        ast::stmt_list *stmt_seq_0();
        ast::stmt *stmt();
        ast::stmt *assign_or_proc_stmt(const std::string &);
        void var_assign(ast::var_access *);
        ast::while_stmt *while_stmt();
        ast::for_stmt *for_stmt();
        ast::if_stmt *if_stmt();
        ast::stmt *else_stmt();
        int dir();
        ast::expr *expr();
        ast::expr *expr_0(ast::expr *);
        ast::expr *simple_expr();
        ast::expr *simple_expr_0(ast::expr *);
        ast::expr *term();
        ast::expr *term_0(ast::expr *);
        ast::expr *factor();
        ast::expr *exp();
        ast::expr *exp_0(ast::expr *);
        ast::expr *primary();
        ast::expr *primary_0(ast::var_access *);
        ast::expr *var_access(ast::var_access *);
        void actual_param_list();
        void actual_param_list_0();
};

#endif /* parser_h_msj16e76apo2jy7i */
