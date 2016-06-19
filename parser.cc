#include <stdexcept>
#include <FlexLexer.h>
#include "lexer.h"
#include "parser.h"

void yyParser::yyparse() {
    yylexsymb = yylexer.yylex();
    program();
    if (yylexsymb != LEX_EOI)
        throw std::logic_error{"eoi error"};
}

void yyParser::match(int symb) {
    if (yylexsymb == symb)
        yylexsymb = yylexer.yylex();
    else
        throw std::logic_error{"match error"};
}

void yyParser::program() {
    switch (yylexsymb) {
        case LEX_PROGRAM:
            yylexsymb = yylexer.yylex();
            match(LEX_IDENT);
            match(LEX_SEMICOLON);
            block();
            match(LEX_DOT);
            break;
        default:
            throw std::logic_error{"program error"};
    }
}

void yyParser::block() {
    switch (yylexsymb) {
        case LEX_SEMICOLON:
        case LEX_DOT:
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
        case LEX_BEGIN:
            def_and_decl_list();
            comp_stmt();
            break;
        default:
            throw std::logic_error{"block error"};
    }
}

void yyParser::def_and_decl_list() {
    switch (yylexsymb) {
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
            def_or_decl();
            def_and_decl_list();
            break;
        case LEX_BEGIN:
            break;
        default:
            throw std::logic_error{"def_and_decl_list error"};
    }
}

void yyParser::def_or_decl() {
    switch (yylexsymb) {
        case LEX_CONST:
            const_def_part();
            break;
        case LEX_VAR:
            var_decl_part();
            break;
        case LEX_PROC:
            proc_decl();
            break;
        case LEX_FUNC:
            func_decl();
            break;
        default:
            throw std::logic_error{"def_or_decl error"};
    }
}

void yyParser::const_def_part() {
    switch (yylexsymb) {
        case LEX_CONST:
            yylexsymb = yylexer.yylex();
            const_def();
            const_list();
            break;
        default:
            throw std::logic_error{"const_def_part error"};
    }
}

void yyParser::const_list() {
    switch (yylexsymb) {
        case LEX_IDENT:
            const_def();
            const_list();
            break;
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
        case LEX_BEGIN:
            break;
        default:
            throw std::logic_error{"const_list error"};
    }
}

void yyParser::const_def() {
    switch (yylexsymb) {
        case LEX_IDENT:
            yylexsymb = yylexer.yylex();
            match(LEX_EQ);
            constant();
            match(LEX_SEMICOLON);
            break;
        default:
            throw std::logic_error{"const_def error"};
    }
}

void yyParser::constant() {
    switch (yylexsymb) {
        case LEX_NUMB:
            yylexsymb = yylexer.yylex();
            break;
        case LEX_PLUS:
            yylexsymb = yylexer.yylex();
            match(LEX_NUMB);
            break;
        case LEX_MINUS:
            yylexsymb = yylexer.yylex();
            match(LEX_NUMB);
            break;
        default:
            throw std::logic_error{"constant error"};
    }
}

void yyParser::var_decl_part() {
    switch (yylexsymb) {
        case LEX_VAR:
            yylexsymb = yylexer.yylex();
            var_decl();
            match(LEX_SEMICOLON);
            var_decl_list();
            break;
        default:
            throw std::logic_error{"var_decl_part error"};
    }
}

void yyParser::var_decl_list() {
    switch (yylexsymb) {
        case LEX_IDENT:
            var_decl();
            match(LEX_SEMICOLON);
            var_decl_list();
            break;
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
        case LEX_BEGIN:
            break;
        default:
            throw std::logic_error{"var_decl_list error"};
    }
}

void yyParser::var_decl() {
    switch (yylexsymb) {
        case LEX_IDENT:
            ident_list();
            match(LEX_COLON);
            type();
            break;
        default:
            throw std::logic_error{"var_decl error"};
    }
}

void yyParser::ident_list() {
    switch (yylexsymb) {
        case LEX_IDENT:
            yylexsymb = yylexer.yylex();
            ident_list_0();
            break;
        default:
            throw std::logic_error{"ident_list error"};
    }
}

void yyParser::ident_list_0() {
    switch (yylexsymb) {
        case LEX_COMMA:
            yylexsymb = yylexer.yylex();
            match(LEX_IDENT);
            ident_list_0();
            break;
        case LEX_COLON:
            break;
        default:
            throw std::logic_error{"ident_list_0 error"};
    }
}

void yyParser::type() {
    switch (yylexsymb) {
        case LEX_INT:
            yylexsymb = yylexer.yylex();
            break;
        case LEX_ARRAY:
            yylexsymb = yylexer.yylex();
            match(LEX_LBRAC);
            constant();
            match(LEX_DOTDOT);
            constant();
            match(LEX_RBRAC);
            match(LEX_OF);
            match(LEX_INT);
            break;
        default:
            throw std::logic_error{"type error"};
    }
}

void yyParser::proc_decl() {
    switch (yylexsymb) {
        case LEX_PROC:
            yylexsymb = yylexer.yylex();
            match(LEX_IDENT);
            proc_decl_0();
            match(LEX_SEMICOLON);
            break;
        default:
            throw std::logic_error{"proc_decl error"};
    }
}

void yyParser::proc_decl_0() {
    switch (yylexsymb) {
        case LEX_SEMICOLON:
            yylexsymb = yylexer.yylex();
            proc_decl_1();
            break;
        case LEX_LRBRAC:
            formal_param_list();
            match(LEX_SEMICOLON);
            proc_decl_1();
            break;
        default:
            throw std::logic_error{"proc_decl_0 error"};
    }
}

void yyParser::proc_decl_1() {
    switch (yylexsymb) {
        case LEX_FORW:
            yylexsymb = yylexer.yylex();
            break;
        case LEX_SEMICOLON:
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
        case LEX_BEGIN:
            block();
            break;
        default:
            throw std::logic_error{"proc_decl_1 error"};
    }
}

void yyParser::func_decl() {
    switch (yylexsymb) {
        case LEX_FUNC:
            yylexsymb = yylexer.yylex();
            match(LEX_IDENT);
            func_decl_0();
            match(LEX_SEMICOLON);
            break;
        default:
            throw std::logic_error{"func_decl error"};
    }
}

void yyParser::func_decl_0() {
    switch (yylexsymb) {
        case LEX_COLON:
            yylexsymb = yylexer.yylex();
            match(LEX_INT);
            match(LEX_SEMICOLON);
            func_decl_1();
            break;
        case LEX_LRBRAC:
            formal_param_list();
            match(LEX_COLON);
            match(LEX_INT);
            match(LEX_SEMICOLON);
            func_decl_1();
            break;
        default:
            throw std::logic_error{"func_decl_0 error"};
    }
}

void yyParser::func_decl_1() {
    switch (yylexsymb) {
        case LEX_FORW:
            yylexsymb = yylexer.yylex();
            break;
        case LEX_SEMICOLON:
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
        case LEX_BEGIN:
            block();
            break;
        default:
            throw std::logic_error{"func_decl_1 error"};
    }
}

void yyParser::formal_param_list() {
    switch (yylexsymb) {
        case LEX_LRBRAC:
            yylexsymb = yylexer.yylex();
            formal_param_sec_list();
            match(LEX_RRBRAC);
            break;
        default:
            throw std::logic_error{"formal_param_list error"};
    }
}

void yyParser::formal_param_sec_list() {
    switch (yylexsymb) {
        case LEX_IDENT:
            formal_param_sec();
            formal_param_sec_list_0();
            break;
        default:
            throw std::logic_error{"formal_param_sec_list error"};
    }
}

void yyParser::formal_param_sec_list_0() {
    switch (yylexsymb) {
        case LEX_SEMICOLON:
            yylexsymb = yylexer.yylex();
            formal_param_sec();
            formal_param_sec_list_0();
            break;
        case LEX_RRBRAC:
            break;
        default:
            throw std::logic_error{"formal_param_sec_list_0 error"};
    }
}

void yyParser::formal_param_sec() {
    switch (yylexsymb) {
        case LEX_IDENT:
            ident_list();
            match(LEX_COLON);
            type();
            break;
        default:
            throw std::logic_error{"formal_param_sec error"};
    }
}

void yyParser::comp_stmt() {
    switch (yylexsymb) {
        case LEX_BEGIN:
            yylexsymb = yylexer.yylex();
            stmt_seq();
            match(LEX_END);
            break;
        default:
            throw std::logic_error{"comp_stmt error"};
    }
}

void yyParser::stmt_seq() {
    switch (yylexsymb) {
        case LEX_BEGIN:
        case LEX_EXIT:
        case LEX_READLN:
        case LEX_WRITE:
        case LEX_WRITELN:
        case LEX_IF:
        case LEX_WHILE:
        case LEX_FOR:
        case LEX_IDENT:
        case LEX_END:
            stmt();
            stmt_seq_0();
            break;
        default:
            throw std::logic_error{"stmt_seq error"};
    }
}

void yyParser::stmt_seq_0() {
    switch (yylexsymb) {
        case LEX_SEMICOLON:
            yylexsymb = yylexer.yylex();
            stmt();
            stmt_seq_0();
            break;
        case LEX_END:
            break;
        default:
            throw std::logic_error{"stmt_seq_0 error"};
    }
}

void yyParser::stmt() {
    switch (yylexsymb) {
        case LEX_IDENT:
            yylexsymb = yylexer.yylex();
            assign_or_proc_stmt();
            break;
        case LEX_BEGIN:
            comp_stmt();
            break;
        case LEX_EXIT:
            yylexsymb = yylexer.yylex();
            break;
        case LEX_READLN:
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            match(LEX_IDENT);
            var_access();
            match(LEX_RRBRAC);
            break;
        case LEX_WRITELN:
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            expr();
            match(LEX_RRBRAC);
            break;
        case LEX_WRITE:
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            expr();
            match(LEX_RRBRAC);
            break;
        case LEX_WHILE:
            while_stmt();
            break;
        case LEX_FOR:
            for_stmt();
            break;
        case LEX_IF:
            if_stmt();
            break;
        case LEX_SEMICOLON:
        case LEX_ELSE:
        case LEX_END:
            break;
        default:
            throw std::logic_error{"stmt error"};
    }
}

void yyParser::assign_or_proc_stmt() {
    switch (yylexsymb) {
        case LEX_SEMICOLON:
        case LEX_END:
        case LEX_ELSE:
            break;
        case LEX_LBRAC:
        case LEX_ASSIGN:
            var_assign();
            expr();
            break;
        case LEX_LRBRAC:
            yylexsymb = yylexer.yylex();
            actual_param_list();
            match(LEX_RRBRAC);
            break;
        default:
            throw std::logic_error{"assign_or_stmt error"};
    }
}

void yyParser::var_assign() {
    switch (yylexsymb) {
        case LEX_LBRAC:
            yylexsymb = yylexer.yylex();
            expr();
            match(LEX_RBRAC);
            var_assign();
            break;
        case LEX_ASSIGN:
            yylexsymb = yylexer.yylex();
            break;
        default:
            throw std::logic_error{"var_assign error"};
    }
}

void yyParser::while_stmt() {
    switch (yylexsymb) {
        case LEX_WHILE:
            yylexsymb = yylexer.yylex();
            expr();
            match(LEX_DO);
            stmt();
            break;
        default:
            throw std::logic_error{"while_stmt error"};
    }
}

void yyParser::for_stmt() {
    switch (yylexsymb) {
        case LEX_FOR:
            yylexsymb = yylexer.yylex();
            match(LEX_IDENT);
            match(LEX_ASSIGN);
            expr();
            dir();
            expr();
            match(LEX_DO);
            stmt();
            break;
        default:
            throw std::logic_error{"for_stmt error"};
    }
}

void yyParser::if_stmt() {
    switch (yylexsymb) {
        case LEX_IF:
            yylexsymb = yylexer.yylex();
            expr();
            match(LEX_THEN);
            stmt();
            else_stmt();
            break;
        default:
            throw std::logic_error{"if_stmt error"};
    }
}

void yyParser::else_stmt() {
    switch (yylexsymb) {
        case LEX_SEMICOLON:
        case LEX_END:
            break;
        case LEX_ELSE:
            yylexsymb = yylexer.yylex();
            stmt();
            break;
        default:
            throw std::logic_error{"else_stmt error"};
    }
}

void yyParser::dir() {
    switch (yylexsymb) {
        case LEX_TO:
            yylexsymb = yylexer.yylex();
            break;
        case LEX_DOWNTO:
            yylexsymb = yylexer.yylex();
            break;
        default:
            throw std::logic_error{"dir error"};
    }
}

void yyParser::expr() {
    switch (yylexsymb) {
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_IDENT:
        case LEX_NUMB:
        case LEX_LRBRAC:
        case LEX_NOT:
            simple_expr();
            expr_0();
            break;
        default:
            throw std::logic_error{"expr error"};
    }
}

void yyParser::expr_0() {
    switch (yylexsymb) {
        case LEX_EQ:
            yylexsymb = yylexer.yylex();
            simple_expr();
            break;
        case LEX_NE:
            yylexsymb = yylexer.yylex();
            simple_expr();
            break;
        case LEX_LT:
            yylexsymb = yylexer.yylex();
            simple_expr();
            break;
        case LEX_GT:
            yylexsymb = yylexer.yylex();
            simple_expr();
            break;
        case LEX_LE:
            yylexsymb = yylexer.yylex();
            simple_expr();
            break;
        case LEX_GE:
            yylexsymb = yylexer.yylex();
            simple_expr();
            break;
        case LEX_RRBRAC:
        case LEX_SEMICOLON:
        case LEX_END:
        case LEX_DO:
        case LEX_TO:
        case LEX_DOWNTO:
        case LEX_RBRAC:
        case LEX_COMMA:
        case LEX_THEN:
        case LEX_ELSE:
            break;
        default:
            throw std::logic_error{"expr_0 error"};
    }
}
void yyParser::simple_expr() {
    switch (yylexsymb) {
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_IDENT:
        case LEX_NUMB:
        case LEX_LRBRAC:
        case LEX_NOT:
            term();
            simple_expr_0();
            break;
        default:
            throw std::logic_error{"simple_expr error"};
    }
}

void yyParser::simple_expr_0() {
    switch (yylexsymb) {
        case LEX_PLUS:
            yylexsymb = yylexer.yylex();
            term();
            simple_expr_0();
            break;
        case LEX_MINUS:
            yylexsymb = yylexer.yylex();
            term();
            simple_expr_0();
            break;
        case LEX_OR:
            yylexsymb = yylexer.yylex();
            term();
            simple_expr_0();
            break;
        case LEX_EQ:
        case LEX_NE:
        case LEX_LT:
        case LEX_GT:
        case LEX_LE:
        case LEX_GE:
        case LEX_RRBRAC:
        case LEX_SEMICOLON:
        case LEX_END:
        case LEX_DO:
        case LEX_TO:
        case LEX_DOWNTO:
        case LEX_RBRAC:
        case LEX_COMMA:
        case LEX_THEN:
        case LEX_ELSE:
            break;
        default:
            throw std::logic_error{"simple_expr_0 error"};
    }
}

void yyParser::term() {
    switch (yylexsymb) {
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_IDENT:
        case LEX_NUMB:
        case LEX_LRBRAC:
        case LEX_NOT:
            factor();
            term_0();
            break;
        default:
            throw std::logic_error{"term error"};
    }
}

void yyParser::term_0() {
    switch (yylexsymb) {
        case LEX_MUL:
            yylexsymb = yylexer.yylex();
            factor();
            term_0();
            break;
        case LEX_DIV:
            yylexsymb = yylexer.yylex();
            factor();
            term_0();
            break;
        case LEX_MOD:
            yylexsymb = yylexer.yylex();
            factor();
            term_0();
            break;
        case LEX_AND:
            yylexsymb = yylexer.yylex();
            factor();
            term_0();
            break;
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_OR:
        case LEX_EQ:
        case LEX_NE:
        case LEX_LT:
        case LEX_GT:
        case LEX_LE:
        case LEX_GE:
        case LEX_RRBRAC:
        case LEX_SEMICOLON:
        case LEX_END:
        case LEX_DO:
        case LEX_TO:
        case LEX_DOWNTO:
        case LEX_RBRAC:
        case LEX_COMMA:
        case LEX_THEN:
        case LEX_ELSE:
            break;
        default:
            throw std::logic_error{"term_0 error"};
    }
}

void yyParser::factor() {
    switch (yylexsymb) {
        case LEX_PLUS:
            yylexsymb = yylexer.yylex();
            factor();
            break;
        case LEX_MINUS:
            yylexsymb = yylexer.yylex();
            factor();
            break;
        case LEX_IDENT:
        case LEX_NUMB:
        case LEX_LRBRAC:
        case LEX_NOT:
            exp();
            break;
        default:
            throw std::logic_error{"factor error"};
    }
}

void yyParser::exp() {
    switch (yylexsymb) {
        case LEX_IDENT:
        case LEX_NUMB:
        case LEX_LRBRAC:
        case LEX_NOT:
            primary();
            exp_0();
            break;
        default:
            throw std::logic_error{"exp error"};
    }
}

void yyParser::exp_0() {
    switch (yylexsymb) {
        case LEX_EXP:
            yylexsymb = yylexer.yylex();
            exp();
            break;
        case LEX_MUL:
        case LEX_DIV:
        case LEX_MOD:
        case LEX_AND:
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_OR:
        case LEX_EQ:
        case LEX_NE:
        case LEX_LT:
        case LEX_GT:
        case LEX_LE:
        case LEX_GE:
        case LEX_RRBRAC:
        case LEX_SEMICOLON:
        case LEX_END:
        case LEX_DO:
        case LEX_TO:
        case LEX_DOWNTO:
        case LEX_RBRAC:
        case LEX_COMMA:
        case LEX_THEN:
        case LEX_ELSE:
            break;
        default:
            throw std::logic_error{"exp_0 error"};
    }
}

void yyParser::primary() {
    switch (yylexsymb) {
        case LEX_IDENT:
            yylexsymb = yylexer.yylex();
            primary_0();
            break;
        case LEX_NUMB:
            yylexsymb = yylexer.yylex();
            break;
        case LEX_LRBRAC:
            yylexsymb = yylexer.yylex();
            expr();
            match(LEX_RRBRAC);
            break;
        case LEX_NOT:
            yylexsymb = yylexer.yylex();
            primary();
            break;
        default:
            throw std::logic_error{"primary error"};
    }
}

void yyParser::primary_0() {
    switch (yylexsymb) {
        case LEX_LRBRAC:
            yylexsymb = yylexer.yylex();
            actual_param_list();
            match(LEX_RRBRAC);
            break;
        case LEX_LBRAC:
        case LEX_EXP:
        case LEX_MUL:
        case LEX_DIV:
        case LEX_MOD:
        case LEX_AND:
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_OR:
        case LEX_EQ:
        case LEX_NE:
        case LEX_LT:
        case LEX_GT:
        case LEX_LE:
        case LEX_GE:
        case LEX_RRBRAC:
        case LEX_SEMICOLON:
        case LEX_END:
        case LEX_DO:
        case LEX_TO:
        case LEX_DOWNTO:
        case LEX_RBRAC:
        case LEX_COMMA:
        case LEX_THEN:
        case LEX_ELSE:
            var_access();
            break;
        default:
            throw std::logic_error{"primary_0 error"};
    }
}

void yyParser::var_access() {
    switch (yylexsymb) {
        case LEX_LBRAC:
            yylexsymb = yylexer.yylex();
            expr();
            match(LEX_RBRAC);
            var_access();
            break;
        case LEX_EXP:
        case LEX_MUL:
        case LEX_DIV:
        case LEX_MOD:
        case LEX_AND:
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_OR:
        case LEX_EQ:
        case LEX_NE:
        case LEX_LT:
        case LEX_GT:
        case LEX_LE:
        case LEX_GE:
        case LEX_RRBRAC:
        case LEX_SEMICOLON:
        case LEX_END:
        case LEX_DO:
        case LEX_TO:
        case LEX_DOWNTO:
        case LEX_ASSIGN:
        case LEX_RBRAC:
        case LEX_COMMA:
        case LEX_THEN:
        case LEX_ELSE:
            break;
        default:
            throw std::logic_error{"var_access error"};
    }
}

void yyParser::actual_param_list() {
    switch (yylexsymb) {
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_IDENT:
        case LEX_NUMB:
        case LEX_LRBRAC:
        case LEX_NOT:
            expr();
            actual_param_list_0();
            break;
        default:
            throw std::logic_error{"actual_param_list error"};
    }
}

void yyParser::actual_param_list_0() {
    switch (yylexsymb) {
        case LEX_COMMA:
            yylexsymb = yylexer.yylex();
            expr();
            actual_param_list_0();
            break;
        case LEX_RRBRAC:
            break;
        default:
            throw std::logic_error{"actual_param_list_0 error"};
    }
}
