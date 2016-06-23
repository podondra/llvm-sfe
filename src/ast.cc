#include <cstdio>
#include <memory>
#include <string>
#include "ast.h"

using namespace ast;

/* abstract node class */
node::~node() {}

void node::print_spaces(int s) const {
    for (auto i = 0; i < s; ++i)
        printf(" ");
}

/* decl class */
decl::~decl() {}

void decl::add_type(std::shared_ptr<type> t) {}

/* decl_list class */
decl_list::decl_list(decl *d, decl_list *l)
    : declaration{d}, next{l} {}

decl_list::~decl_list() {
    delete declaration;
    delete next;
}

void decl_list::dump(int s) const {
    print_spaces(s);
    printf("decl_list\n");
    declaration->dump(s + 4);
    next->dump(s);
}

/* null_decl_list class */
null_decl_list::null_decl_list() : decl_list{nullptr, nullptr} {}

void null_decl_list::dump(int s) const {
    print_spaces(s);
    printf("null_decl_list\n");
}

/* var_decl_list class */
var_decl_list::var_decl_list(decl *d, decl_list *l) : decl_list{d, l} {}

void var_decl_list::add_type(std::shared_ptr<type> t) {
    declaration->add_type(t);
    next->add_type(t);
}

var_decl_list::~var_decl_list() {}

void var_decl_list::dump(int s) const {
    print_spaces(s);
    printf("var_decl_list\n");
    declaration->dump(s + 4);
    next->dump(s);
}

/* block class */
block::block(decl_list *d, stmt *s) : decls{d}, body{s} {}

block::~block() {
    delete decls;
    delete body;
}

void block::dump(int s) const {
    print_spaces(s);
    printf("block\n");
    decls->dump(s + 4);
    body->dump(s + 4);
}

/* expr class */
expr::~expr() {}

binary_expr::binary_expr(expr *l, expr *r) : left{l}, right{r} {}
binary_expr::~binary_expr() {
    delete left;
    delete right;
}

eq_expr::eq_expr(expr *l, expr *r) : binary_expr{r, l} {}
eq_expr::~eq_expr() {}
void eq_expr::dump(int s) const {
    print_spaces(s);
    printf("\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

ne_expr::ne_expr(expr *l, expr *r) : binary_expr{r, l} {}
ne_expr::~ne_expr() {}
void ne_expr::dump(int s) const {
    print_spaces(s);
    printf("\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

lt_expr::lt_expr(expr *l, expr *r) : binary_expr{r, l} {}
lt_expr::~lt_expr() {}
void lt_expr::dump(int s) const {
    print_spaces(s);
    printf("lt_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

gt_expr::gt_expr(expr *l, expr *r) : binary_expr{r, l} {}
gt_expr::~gt_expr() {}
void gt_expr::dump(int s) const {
    print_spaces(s);
    printf("gt_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

le_expr::le_expr(expr *l, expr *r) : binary_expr{r, l} {}
le_expr::~le_expr() {}
void le_expr::dump(int s) const {
    print_spaces(s);
    printf("le_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

ge_expr::ge_expr(expr *l, expr *r) : binary_expr{r, l} {}
ge_expr::~ge_expr() {}
void ge_expr::dump(int s) const {
    print_spaces(s);
    printf("ge_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

add_expr::add_expr(expr *l, expr *r) : binary_expr{r, l} {}
add_expr::~add_expr() {}
void add_expr::dump(int s) const {
    print_spaces(s);
    printf("add_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

sub_expr::sub_expr(expr *l, expr *r) : binary_expr{r, l} {}
sub_expr::~sub_expr() {}
void sub_expr::dump(int s) const {
    print_spaces(s);
    printf("sub_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

or_expr::or_expr(expr *l, expr *r) : binary_expr{r, l} {}
or_expr::~or_expr() {}
void or_expr::dump(int s) const {
    print_spaces(s);
    printf("or_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

mul_expr::mul_expr(expr *l, expr *r) : binary_expr{r, l} {}
mul_expr::~mul_expr() {}
void mul_expr::dump(int s) const {
    print_spaces(s);
    printf("mul_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

div_expr::div_expr(expr *l, expr *r) : binary_expr{r, l} {}
div_expr::~div_expr() {}
void div_expr::dump(int s) const {
    print_spaces(s);
    printf("div_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

mod_expr::mod_expr(expr *l, expr *r) : binary_expr{r, l} {}
mod_expr::~mod_expr() {}
void mod_expr::dump(int s) const {
    print_spaces(s);
    printf("mod_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

and_expr::and_expr(expr *l, expr *r) : binary_expr{r, l} {}
and_expr::~and_expr() {}
void and_expr::dump(int s) const {
    print_spaces(s);
    printf("and_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

exp_expr::exp_expr(expr *b, expr *e) : binary_expr{b, e} {}
exp_expr::~exp_expr() {}
void exp_expr::dump(int s) const {
    print_spaces(s);
    printf("exp_expr\n");
    left->dump(s + 4);
    right->dump(s + 4);
}

/* unary_expr class */
unary_expr::unary_expr(expr *c) : child{c} {}
unary_expr::~unary_expr() {
    delete child;
}

minus_expr::minus_expr(expr *c) : unary_expr{c} {}
minus_expr::~minus_expr() {}
void minus_expr::dump(int s) const {
    print_spaces(s);
    printf("minus_expr\n");
    child->dump(s + 4);
}

not_expr::not_expr(expr *c) : unary_expr{c} {}
not_expr::~not_expr() {}
void not_expr::dump(int s) const {
    print_spaces(s);
    printf("not_expr\n");
    child->dump(s + 4);
}

void null_expr::dump(int s) const {
    print_spaces(s);
    printf("null_expr\n");
}

/* var_access class */
var_access::var_access(const std::string &n) : name{n} {}

var_access::~var_access() {
    for (auto e : idxs)
        delete e;
}

void var_access::add_idx(expr *e) {
    idxs.push_back(e);
}

void var_access::dump(int s) const {
    print_spaces(s);
    printf("var_access name: '%s'\n", name.c_str());
    for (auto e : idxs)
        e->dump(s + 4);
}

/* numb class */
numb::numb(long int v) : val{v} {}

void numb::dump(int s) const {
    print_spaces(s);
    printf("numb val: %ld\n", val);
}

/* stmt class */
stmt::~stmt() {}

void stmt::dump(int s) const {
    print_spaces(s);
    printf("stmt\n");
}

/* stmt_list class */
stmt_list::stmt_list(stmt* s, stmt_list* l) : statement{s}, next{l} {}

stmt_list::~stmt_list() {
    delete statement;
    delete next;
}

void stmt_list::dump(int s) const {
    print_spaces(s);
    printf("stmt_list\n");
    statement->dump(s + 4);
    next->dump(s);
}

/* null_stmt_list class */
null_stmt_list::null_stmt_list() : stmt_list{nullptr, nullptr} {}

void null_stmt_list::dump(int s) const {
    print_spaces(s);
    printf("null_stmt_list\n");
}

/* const_decl class */
const_decl::const_decl(const std::string& n, int v) : name{n}, val{v} {}

const_decl::~const_decl() {}

void const_decl::dump(int s) const {
    print_spaces(s);
    printf("const_decl name: '%s' val: %d\n", name.c_str(), val);
}

/* var_decl class */
var_decl::var_decl(const std::string& n) : name{n}, var_type{nullptr} {}

var_decl::~var_decl() {}

void var_decl::add_type(std::shared_ptr<type> t) {
    var_type = t;
}

void var_decl::dump(int s) const {
    print_spaces(s);
    printf("var name: '%s'\n", name.c_str());
    var_type->dump(s + 4);
}

/* type class */
type::~type() {}

/* type class */
int_type::~int_type() {}

void int_type::dump(int s) const {
    print_spaces(s);
    printf("int_type\n");
}

/* type class */
array_type::~array_type() {}

void array_type::dump(int s) const {
    print_spaces(s);
    printf("array_type\n");
}

/* proc_decl class */
proc_decl::proc_decl() {}

/* func_decl class */
func_decl::func_decl() {}

/* compound_stmt class */
compound_stmt::compound_stmt(stmt_list *l) : list{l} {}

compound_stmt::~compound_stmt() {
    delete list;
}

void compound_stmt::dump(int s) const {
    print_spaces(s);
    printf("compound_stmt\n");
    list->dump(s + 4);
}

/* assign_stmt class */
assign_stmt::assign_stmt(var_access *v , expr *e) : var{v}, expression{e} {}

assign_stmt::~assign_stmt() {
    delete var;
    delete expression;
}

void assign_stmt::dump(int s) const {
    print_spaces(s);
    printf("assign_stmt\n");
    var->dump(s + 4);
    expression->dump(s + 4);
}

/* if_stmt class */
if_stmt::if_stmt(expr *c, stmt *t, stmt *e)
    : condition{c}, then_stmt{t}, else_stmt{e} {}

if_stmt::~if_stmt() {
    delete condition;
    delete then_stmt;
    delete else_stmt;
}

void if_stmt::dump(int s) const {
    print_spaces(s);
    printf("if_stmt\n");
    then_stmt->dump(s + 4);
    else_stmt->dump(s + 4);
}

/* while class */
while_stmt::while_stmt(expr *c, stmt *b) : condition{c}, body{b} {}

while_stmt::~while_stmt() {
    delete condition;
    delete body;
}

void while_stmt::dump(int s) const {
    print_spaces(s);
    printf("while_stmt\n");
    condition->dump(s + 4);
    body->dump(s + 4);
}

/* for_stmt class */
for_stmt::for_stmt(const std::string &n, expr *f, int d, expr *t, stmt *b)
    : name{n}, from{f}, to{t}, dir{d}, body{b} {}

for_stmt::~for_stmt() {
    delete from;
    delete to;
    delete body;
}

void for_stmt::dump(int s) const {
    print_spaces(s);
    printf("for_stmt name: '%s' dir: %d\n", name.c_str(), dir);
    from->dump(s + 4);
    to->dump(s + 4);
    body->dump(s + 4);
}

/* exit class */
void exit_stmt::dump(int s) const {
    print_spaces(s);
    printf("exit_stmt\n");
}

/* readln_stmt class */
readln_stmt::readln_stmt(var_access *v) : var{v} {}

readln_stmt::~readln_stmt() {
    delete var;
}

void readln_stmt::dump(int s) const {
    print_spaces(s);
    printf("readln_stmt\n");
    var->dump(s + 4);
}

/* write class */
write_stmt::write_stmt(expr *e) : expression{e} {}
write_stmt::~write_stmt() {
    delete expression;
}

void write_stmt::dump(int s) const {
    print_spaces(s);
    printf("write_stmt\n");
    expression->dump(s + 4);
}

/* writeln_stmt class */
writeln_stmt::writeln_stmt(expr *e) : expression{e} {}
writeln_stmt::~writeln_stmt() {
    delete expression;
}

void writeln_stmt::dump(int s) const {
    print_spaces(s);
    printf("writeln_stmt\n");
    expression->dump(s + 4);
}

/* null_stmt class */
null_stmt::~null_stmt() {}

void null_stmt::dump(int s) const {
    print_spaces(s);
    printf("null_stmt\n");
}
