#include <fstream>
#include <stdexcept>
#include <FlexLexer.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

yyParser::yyParser(std::ifstream *in)
    : yylexer{in} {}

ast::node *yyParser::yyparse() {
    yylexsymb = yylexer.yylex();
    auto root = program();
    if (yylexsymb != LEX_EOI)
        return nullptr;
    return root;
}

void yyParser::match(int symb) {
    if (yylexsymb == symb)
        yylexsymb = yylexer.yylex();
    else
        std::cout << "match error" << std::endl;
}

std::string yyParser::get_ident() {
    return std::string(yylexer.YYText(), yylexer.YYLeng());
}

ast::node *yyParser::program() {
    match(LEX_PROGRAM);
    match(LEX_IDENT);
    match(LEX_SEMICOLON);
    auto b = block();
    match(LEX_DOT);
    return b;
}

ast::block *yyParser::block() {
    auto d = def_and_decl_list();
    return new ast::block{d, comp_stmt()};
}

ast::decl_list *yyParser::def_and_decl_list() {
    switch (yylexsymb) {
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC: {
            auto l = def_or_decl();
            return new ast::decl_list{l, def_and_decl_list()};
        }
        default:
            return new ast::null_decl_list{};
    }
}

ast::decl *yyParser::def_or_decl() {
    switch (yylexsymb) {
        case LEX_CONST:
            return const_def_part();
        case LEX_VAR:
            return var_decl_part();
        case LEX_PROC:
            return proc_decl();
        case LEX_FUNC:
            return func_decl();
        default:
            std::cout << "def_or_decl error" << std::endl;
            return nullptr;
    }
}

ast::decl_list *yyParser::const_def_part() {
    match(LEX_CONST);
    auto d = const_def();
    return new ast::decl_list{d, const_list()};
}

ast::decl_list *yyParser::const_list() {
    switch (yylexsymb) {
        case LEX_IDENT: {
            auto d = const_def();
            return new ast::decl_list{d, const_list()};
        }
        default:
            return new ast::null_decl_list{};
    }
}

ast::decl *yyParser::const_def() {
    switch (yylexsymb) {
        case LEX_IDENT: {
            auto n = get_ident();
            yylexsymb = yylexer.yylex();
            match(LEX_EQ);
            auto v = constant();
            match(LEX_SEMICOLON);
            return new ast::const_decl{n, v};
        }
        default:
            std::cout << "const_def error" << std::endl;
            return nullptr;
    }
}

int yyParser::constant() {
    switch (yylexsymb) {
        case LEX_NUMB:
            yylexsymb = yylexer.yylex();
            return yynumbval;
        case LEX_PLUS:
            yylexsymb = yylexer.yylex();
            match(LEX_NUMB);
            return yynumbval;
        case LEX_MINUS:
            yylexsymb = yylexer.yylex();
            match(LEX_NUMB);
            return -yynumbval;
        default:
            std::cout << "constant error" << std::endl;
            return -1;
    }
}

ast::decl_list *yyParser::var_decl_part() {
    match(LEX_VAR);
    auto l = var_decl();
    match(LEX_SEMICOLON);
    return new ast::decl_list{l, var_decl_list()};
}

ast::decl_list *yyParser::var_decl_list() {
    switch (yylexsymb) {
        case LEX_IDENT: {
            auto l = var_decl();
            match(LEX_SEMICOLON);
            return new ast::decl_list{l, var_decl_list()};
        }
        default:
            return new ast::null_decl_list{};
    }
}

ast::decl_list *yyParser::var_decl() {
    auto l = ident_list();
    match(LEX_COLON);
    auto t = type();
    l->add_type(t);
    return l;
}

ast::decl_list *yyParser::ident_list() {
    auto n = get_ident(); 
    match(LEX_IDENT);
    return new ast::var_decl_list{new ast::var_decl{n}, ident_list_0()};
}

ast::decl_list *yyParser::ident_list_0() {
    switch (yylexsymb) {
        case LEX_COMMA: {
            yylexsymb = yylexer.yylex();
            auto n = get_ident();
            match(LEX_IDENT);
            return new ast::var_decl_list{new ast::var_decl{n}, ident_list_0()};
        }
        default:
            return new ast::null_decl_list{};
    }
}

std::shared_ptr<ast::type> yyParser::type() {
    switch (yylexsymb) {
        case LEX_INT:
            yylexsymb = yylexer.yylex();
            return std::make_shared<ast::int_type>();
        case LEX_ARRAY: {
            yylexsymb = yylexer.yylex();
            match(LEX_LBRAC);
            auto f = constant();
            match(LEX_DOTDOT);
            auto t = constant();
            match(LEX_RBRAC);
            match(LEX_OF);
            match(LEX_INT);
            return std::make_shared<ast::array_type>(f, t);
        }
        default:
            std::cout << "type error" << std::endl;
            return nullptr;
    }
}

ast::decl *yyParser::proc_decl() {
    match(LEX_PROC);
    auto n = get_ident();
    match(LEX_IDENT);
    auto p = proc_decl_0(n);
    match(LEX_SEMICOLON);
    return p;
}

ast::proc_decl *yyParser::proc_decl_0(const std:: string &n) {
    switch (yylexsymb) {
        case LEX_SEMICOLON: {
            yylexsymb = yylexer.yylex();
            auto b = proc_decl_1();
            return new ast::proc_decl{n, std::list<std::string>{}, b};
        }
        case LEX_LRBRAC: {
            auto f = formal_param_list();
            match(LEX_SEMICOLON);
            auto b = proc_decl_1();
            return new ast::proc_decl{n, f, b};
        }
        default:
            std::cout << "proc_decl_0 error" << std::endl;
            return nullptr;
    }
}

ast::block *yyParser::proc_decl_1() {
    switch (yylexsymb) {
        case LEX_FORW:
            yylexsymb = yylexer.yylex();
            return nullptr;
        case LEX_SEMICOLON:
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
        case LEX_BEGIN:
            return block();
        default:
            std::cout << "proc_decl_1 error" << std::endl;
            return nullptr;
    }
}

ast::decl *yyParser::func_decl() {
    match(LEX_FUNC);
    auto n = get_ident();
    match(LEX_IDENT);
    auto f = func_decl_0(n);
    match(LEX_SEMICOLON);
    return f;
}

ast::func_decl *yyParser::func_decl_0(const std::string &n) {
    switch (yylexsymb) {
        case LEX_COLON: {
            yylexsymb = yylexer.yylex();
            match(LEX_INT);
            match(LEX_SEMICOLON);
            auto b = func_decl_1();
            return new ast::func_decl{n, std::list<std::string>{}, b};
        }
        case LEX_LRBRAC: {
            auto f = formal_param_list();
            match(LEX_COLON);
            match(LEX_INT);
            match(LEX_SEMICOLON);
            auto b = func_decl_1();
            return new ast::func_decl{n, f, b};
        }
        default:
            std::cout << "func_decl_0 error" << std::endl;
            return nullptr;
    }
}

ast::block *yyParser::func_decl_1() {
    switch (yylexsymb) {
        case LEX_FORW:
            yylexsymb = yylexer.yylex();
            return nullptr;
        case LEX_SEMICOLON:
        case LEX_CONST:
        case LEX_VAR:
        case LEX_PROC:
        case LEX_FUNC:
        case LEX_BEGIN:
            return block();
        default:
            std::cout << "func_decl_1 error" << std::endl;
            return nullptr;
    }
}

std::list<std::string> yyParser::formal_param_list() {
    match(LEX_LRBRAC);
    auto l = formal_param_sec_list();
    match(LEX_RRBRAC);
    return l;
}

std::list<std::string> yyParser::formal_param_sec_list() {
    auto l = std::list<std::string>{};
    formal_param_sec(l);
    formal_param_sec_list_0(l);
    return l;
}

void yyParser::formal_param_sec_list_0(std::list<std::string> &l) {
    switch (yylexsymb) {
        case LEX_SEMICOLON:
            yylexsymb = yylexer.yylex();
            formal_param_sec(l);
            formal_param_sec_list_0(l);
            return;
        default:
            return;
    }
}

void yyParser::formal_param_sec(std::list<std::string> &l) {
    l.push_back(get_ident());
    yylexsymb = yylexer.yylex();
    match(LEX_COLON);
    type();
}

ast::compound_stmt *yyParser::comp_stmt() {
    match(LEX_BEGIN);
    auto s = stmt_seq();
    match(LEX_END);
    return new ast::compound_stmt{s};
}

ast::stmt_list *yyParser::stmt_seq() {
    auto s = stmt();
    return new ast::stmt_list{s, stmt_seq_0()};
}

ast::stmt_list *yyParser::stmt_seq_0() {
    switch (yylexsymb) {
        case LEX_SEMICOLON: {
            yylexsymb = yylexer.yylex();
            auto s = stmt();
            return new ast::stmt_list{s, stmt_seq_0()};
        }
        default:
            return new ast::null_stmt_list{};
    }
}

ast::stmt *yyParser::stmt() {
    switch (yylexsymb) {
        case LEX_IDENT: {
            auto n = get_ident();
            yylexsymb = yylexer.yylex();
            return assign_or_proc_stmt(n);
        }
        case LEX_BEGIN:
            return comp_stmt();
        case LEX_EXIT:
            yylexsymb = yylexer.yylex();
            return new ast::exit_stmt{};
        case LEX_INC: {
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            auto n = get_ident();
            match(LEX_IDENT);
            auto v = new ast::var_assign{n};
            var_assign(v);
            match(LEX_RRBRAC);
            return new ast::inc_stmt{v};
        }
        case LEX_DEC: {
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            auto n = get_ident();
            match(LEX_IDENT);
            auto v = new ast::var_assign{n};
            var_assign(v);
            match(LEX_RRBRAC);
            return new ast::dec_stmt{v};
        }
        case LEX_READLN: {
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            auto n = get_ident();
            match(LEX_IDENT);
            auto v = new ast::var_assign{n};
            var_assign(v);
            match(LEX_RRBRAC);
            return new ast::readln_stmt{v};
        }
        case LEX_WRITELN: {
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            auto e = expr();
            match(LEX_RRBRAC);
            return new ast::writeln_stmt{e};
        }
        case LEX_WRITE: {
            yylexsymb = yylexer.yylex();
            match(LEX_LRBRAC);
            auto e = expr();
            match(LEX_RRBRAC);
            return new ast::write_stmt{e};
        }
        case LEX_BRK: {
            yylexsymb = yylexer.yylex();
            return new ast::break_stmt{};
        }
        case LEX_WHILE:
            return while_stmt();
        case LEX_FOR:
            return for_stmt();
        case LEX_IF:
            return if_stmt();
        default:
            return new ast::null_stmt{};
    }
}

ast::stmt *yyParser::assign_or_proc_stmt(const std::string& n) {
    switch (yylexsymb) {
        case LEX_LBRAC:
        case LEX_ASSIGN: {
            auto v = new ast::var_assign{n};
            var_assign(v);
            return new ast::assign_stmt{v, expr()};
        }
        case LEX_LRBRAC: {
            yylexsymb = yylexer.yylex();
            auto l = actual_param_list();
            match(LEX_RRBRAC);
            return new ast::proc_call{n, l};
        }
        default:
            return new ast::null_stmt{};
    }
}

void yyParser::var_assign(ast::var_assign *v) {
    switch (yylexsymb) {
        case LEX_LBRAC:
            yylexsymb = yylexer.yylex();
            v->add_idx(expr());
            match(LEX_RBRAC);
            var_assign(v);
            return;
        case LEX_ASSIGN:
            yylexsymb = yylexer.yylex();
            return;
        default:
            return;
    }
}

ast::while_stmt *yyParser::while_stmt() {
    match(LEX_WHILE);
    auto e = expr();
    match(LEX_DO);
    return new ast::while_stmt{e, stmt()};
}

ast::for_stmt *yyParser::for_stmt() {
    match(LEX_FOR);
    auto n = get_ident();
    match(LEX_IDENT);
    match(LEX_ASSIGN);
    auto f = expr();
    auto d = dir();
    auto t = expr();
    match(LEX_DO);
    return new ast::for_stmt{n, f, d, t, stmt()};
}

ast::if_stmt *yyParser::if_stmt() {
    match(LEX_IF);
    auto e = expr();
    match(LEX_THEN);
    auto s = stmt();
    return new ast::if_stmt{e, s, else_stmt()};
}

ast::stmt *yyParser::else_stmt() {
    switch (yylexsymb) {
        case LEX_ELSE:
            yylexsymb = yylexer.yylex();
            return stmt();
        default:
            return new ast::null_stmt{};
    }
}

int yyParser::dir() {
    switch (yylexsymb) {
        case LEX_TO:
            yylexsymb = yylexer.yylex();
            return ast::DIR_TO;
        case LEX_DOWNTO:
            yylexsymb = yylexer.yylex();
            return ast::DIR_DOWNTO;
        default:
            std::cout << "dir error" << std::endl;
            return -1;
    }
}

ast::expr *yyParser::expr() {
    return expr_0(simple_expr());
}

ast::expr *yyParser::expr_0(ast::expr *e) {
    switch (yylexsymb) {
        case LEX_EQ:
            yylexsymb = yylexer.yylex();
            return new ast::eq_expr{e, simple_expr()};
        case LEX_NE:
            yylexsymb = yylexer.yylex();
            return new ast::ne_expr{e, simple_expr()};
        case LEX_LT:
            yylexsymb = yylexer.yylex();
            return new ast::lt_expr{e, simple_expr()};
        case LEX_GT:
            yylexsymb = yylexer.yylex();
            return new ast::gt_expr{e, simple_expr()};
        case LEX_LE:
            yylexsymb = yylexer.yylex();
            return new ast::le_expr{e, simple_expr()};
        case LEX_GE:
            yylexsymb = yylexer.yylex();
            return new ast::ge_expr{e, simple_expr()};
        default:
            return e;
    }
}

ast::expr *yyParser::simple_expr() {
    return simple_expr_0(term());
}

ast::expr *yyParser::simple_expr_0(ast::expr *e) {
    switch (yylexsymb) {
        case LEX_PLUS:
            yylexsymb = yylexer.yylex();
            return simple_expr_0(new ast::add_expr{e, term()});
        case LEX_MINUS:
            yylexsymb = yylexer.yylex();
            return simple_expr_0(new ast::sub_expr{e, term()});
        case LEX_OR:
            yylexsymb = yylexer.yylex();
            return simple_expr_0(new ast::or_expr{e, term()});
        default:
            return e;
    }
}

ast::expr *yyParser::term() {
    return term_0(factor());
}

ast::expr *yyParser::term_0(ast::expr *e) {
    switch (yylexsymb) {
        case LEX_MUL:
            yylexsymb = yylexer.yylex();
            return term_0(new ast::mul_expr{e, factor()});
        case LEX_DIV:
            yylexsymb = yylexer.yylex();
            return term_0(new ast::div_expr{e, factor()});
        case LEX_MOD:
            yylexsymb = yylexer.yylex();
            return term_0(new ast::mod_expr{e, factor()});
        case LEX_AND:
            yylexsymb = yylexer.yylex();
            return term_0(new ast::and_expr{e, factor()});
        default:
            return e;
    }
}

ast::expr *yyParser::factor() {
    switch (yylexsymb) {
        case LEX_PLUS:
            yylexsymb = yylexer.yylex();
            return factor();
        case LEX_MINUS:
            yylexsymb = yylexer.yylex();
            return new ast::minus_expr{factor()};
        case LEX_IDENT:
        case LEX_NUMB:
        case LEX_LRBRAC:
        case LEX_NOT:
            return exp();
        default:
            std::cout << "factor error" << std::endl;
            return nullptr;
    }
}

ast::expr *yyParser::exp() {
    return exp_0(primary());
}

ast::expr *yyParser::exp_0(ast::expr *e) {
    switch (yylexsymb) {
        case LEX_EXP:
            yylexsymb = yylexer.yylex();
            return new ast::exp_expr{e, exp()};
        default:
            return e;
    }
}

ast::expr *yyParser::primary() {
    switch (yylexsymb) {
        case LEX_IDENT: {
            auto n = get_ident();
            yylexsymb = yylexer.yylex();
            return primary_0(n);
        }
        case LEX_NUMB:
            yylexsymb = yylexer.yylex();
            return new ast::numb{yynumbval};
        case LEX_LRBRAC: {
            yylexsymb = yylexer.yylex();
            auto e = expr();
            match(LEX_RRBRAC);
            return e;
        }
        case LEX_NOT:
            yylexsymb = yylexer.yylex();
            return new ast::not_expr{primary()};
        default:
            std::cout << "primary error" << std::endl;
            return nullptr;
    }
}

ast::expr *yyParser::primary_0(const std::string &n) {
    switch (yylexsymb) {
        case LEX_LRBRAC: {
            yylexsymb = yylexer.yylex();
            auto l = actual_param_list();
            match(LEX_RRBRAC);
            return new ast::call{n, l};
        }
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
            return var_access(new ast::var_access{n});
        default:
            std::cout << "primary_0 error" << std::endl;
            return nullptr;
    }
}

ast::expr *yyParser::var_access(ast::var_access *v) {
    switch (yylexsymb) {
        case LEX_LBRAC:
            yylexsymb = yylexer.yylex();
            v->add_idx(expr());
            match(LEX_RBRAC);
            return var_access(v);
        default:
            return v;
    }
}

std::list<ast::expr *> yyParser::actual_param_list() {
    auto l = std::list<ast::expr *>{};
    l.push_back(expr());
    actual_param_list_0(l);
    return l;
}

void yyParser::actual_param_list_0(std::list<ast::expr *> &l) {
    switch (yylexsymb) {
        case LEX_COMMA:
            yylexsymb = yylexer.yylex();
            l.push_back(expr());
            actual_param_list_0(l);
            return;
        default:
            return;
    }
}
