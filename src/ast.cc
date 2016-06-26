#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include "ast.h"
#include "parser.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "../include/KaleidoscopeJIT.h"

using namespace ast;

static llvm::LLVMContext context;
static llvm::IRBuilder<> builder{context};
static std::unique_ptr<llvm::Module> module;
static std::map<std::string, llvm::AllocaInst *> named_vals;
static std::map<std::string, llvm::AllocaInst *> const_vals;
static std::map<std::string, long int> arr_starts;
static std::unique_ptr<llvm::orc::KaleidoscopeJIT> jit;
static llvm::BasicBlock *break_bb;

llvm::Function *scanln_fun;
llvm::Function *println_fun;
llvm::Function *print_fun;

/*
 * abstract node class
 */
node::~node() {}

void node::print_spaces(int spaces) const {
    for (auto i = 0; i < spaces; ++i)
        std::cout << " ";
}

/*
 * decl class
 */
decl::~decl() {}

void decl::add_type(std::shared_ptr<type> t) {}

/*
 * decl_list class
 */
decl_list::decl_list(decl *d, decl_list *l)
    : declaration{d}, next{l} {}

llvm::Value *decl_list::gen_ir() {
    auto d = declaration->gen_ir();
    next->gen_ir();
    return d;
}

decl_list::~decl_list() {
    delete declaration;
    delete next;
}

void decl_list::dump(int s) const {
    print_spaces(s);
    std::cout << "decl_list" << std::endl;
    declaration->dump(s + 4);
    next->dump(s);
}

/*
 * null_decl_list class
 */
null_decl_list::null_decl_list() : decl_list{nullptr, nullptr} {}

llvm::Value *null_decl_list::gen_ir() {
    return nullptr;
}

void null_decl_list::dump(int s) const {
    print_spaces(s);
    std::cout << "null_decl_list" << std::endl;
}

/*
 * var_decl_list class
 */
var_decl_list::var_decl_list(decl *d, decl_list *l) : decl_list{d, l} {}

void var_decl_list::add_type(std::shared_ptr<type> t) {
    declaration->add_type(t);
    next->add_type(t);
}

void var_decl_list::dump(int s) const {
    print_spaces(s);
    std::cout << "var_decl_list" << std::endl;
    declaration->dump(s + 4);
    next->dump(s);
}

/*
 * block class
 */
block::block(decl_list *d, stmt *s) : decls{d}, body{s} {}

block::~block() {
    delete decls;
    delete body;
}

llvm::Value *block::gen_ir() {
    decls->gen_ir();
    return body->gen_ir();
}

void block::dump(int s) const {
    print_spaces(s);
    std::cout << "block" << std::endl;
    decls->dump(s + 4);
    body->dump(s + 4);
}

/*
 * expr class
 */
expr::~expr() {}

/*
 * binary class
 */
binary_expr::binary_expr(expr *l, expr *r) : left{l}, right{r} {}

binary_expr::~binary_expr() {
    delete left;
    delete right;
}

/*
 * eq_expr class
 */
eq_expr::eq_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *eq_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateICmpEQ(l, r, "eq");
}

void eq_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "eq_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * ne_expr class
 */
ne_expr::ne_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *ne_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateICmpNE(l, r, "ne");
}

void ne_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "ne_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * lt_expr class
 */
lt_expr::lt_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *lt_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateICmpSLT(l, r, "lt");
}

void lt_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "lt_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * gt_expr class
 */
gt_expr::gt_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *gt_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateICmpSGT(l, r, "gt");
}

void gt_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "gt_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * le_expr class
 */
le_expr::le_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *le_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateICmpSLE(l, r, "le");
}

void le_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "le_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * ge_expr class
 */
ge_expr::ge_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *ge_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateICmpSGE(l, r, "ge");
}

void ge_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "ge_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * add_expr class
 */
add_expr::add_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *add_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateAdd(l, r, "add");
}

void add_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "add_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * sub_expr class
 */
sub_expr::sub_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *sub_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateNSWSub(l, r, "sub");
}

void sub_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "sub_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * or_expr class
 */
or_expr::or_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *or_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateOr(l, r, "or");
}

void or_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "or_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * mul_expr class
 */
mul_expr::mul_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *mul_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateMul(l, r, "mul");
}

void mul_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "mul_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * div_expr class
 */
div_expr::div_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *div_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateSDiv(l, r, "div");
}

void div_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "div_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * mod_expr class
 */
mod_expr::mod_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *mod_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateSRem(l, r, "mod");
}

void mod_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "mod_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * and_expr class
 */
and_expr::and_expr(expr *l, expr *r) : binary_expr{l, r} {}

llvm::Value *and_expr::gen_ir() {
    auto l = left->gen_ir();
    auto r = right->gen_ir();
    if (l == nullptr || r == nullptr)
        return nullptr;
    return builder.CreateAnd(l, r, "and");
}

void and_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "and_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * exp_expr class
 */
exp_expr::exp_expr(expr *b, expr *e) : binary_expr{b, e} {}

llvm::Value *exp_expr::gen_ir() {
    return nullptr;
}

void exp_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "exp_expr" << std::endl;
    left->dump(s + 4);
    right->dump(s + 4);
}

/*
 * unary_expr class
 */
unary_expr::unary_expr(expr *c) : child{c} {}
unary_expr::~unary_expr() {
    delete child;
}

/*
 * minus_expr class
 */
minus_expr::minus_expr(expr *c) : unary_expr{c} {}

void minus_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "minus_expr" << std::endl;
    child->dump(s + 4);
}

llvm::Value *minus_expr::gen_ir() {
    auto c = child->gen_ir();
    if (c == nullptr)
        return nullptr;
    return builder.CreateNeg(c, "minus");
}

/*
 * not_expr
 */
not_expr::not_expr(expr *c) : unary_expr{c} {}

llvm::Value *not_expr::gen_ir() {
    auto c = child->gen_ir();
    if (c == nullptr)
        return nullptr;
    return builder.CreateNot(c, "not");
}

void not_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "not_expr" << std::endl;
    child->dump(s + 4);
}

/*
 * null_expr
 */
void null_expr::dump(int s) const {
    print_spaces(s);
    std::cout << "null_expr" << std::endl;
}

/*
 * proc_call class
 */
proc_call::proc_call(const std::string &n, std::list<expr *> p)
    : name{n}, params{std::move(p)} {}

proc_call::~proc_call() {
    for (auto e : params)
        delete e; } 
llvm::Value *proc_call::gen_ir() {
    auto fun = module->getFunction(name);
    auto p = std::vector<llvm::Value *>{};
    for (auto e : params)
        p.push_back(e->gen_ir());
    return builder.CreateCall(fun, p);
}

void proc_call::dump(int s) const {
    print_spaces(s);
    std::cout << "proc_call name: " << name << std::endl;
    for (auto e : params)
        e->dump(s + 4);
}

/*
 * call class
 */
call::call(const std::string &n, std::list<expr *> p)
    : name{n}, params{std::move(p)} {}

call::~call() {
    for (auto e : params)
        delete e;
}

llvm::Value *call::gen_ir() {
    auto fun = module->getFunction(name);
    auto p = std::vector<llvm::Value *>{};
    for (auto e : params)
        p.push_back(e->gen_ir());
    return builder.CreateCall(fun, p, "call");
}

void call::dump(int s) const {
    print_spaces(s);
    std::cout << "call name: " << name << std::endl;
    for (auto e : params)
        e->dump(s + 4);
}

/*
 * var_access class
 */
var_access::var_access(const std::string &n) : name{n} {}

var_access::~var_access() {
    for (auto e : idxs)
        delete e;
}

void var_access::add_idx(expr *e) {
    idxs.push_back(e);
}

llvm::Value *var_access::gen_ir() {
    auto var = const_vals[name];
    if (!idxs.empty()) {
        auto pos = builder.CreateAdd(idxs.front()->gen_ir(), llvm::ConstantInt::get(llvm::IntegerType::getInt64Ty(context), arr_starts[name]));
        auto index = builder.CreateInBoundsGEP(
                var,
                { llvm::ConstantInt::get(llvm::IntegerType::getInt64Ty(context), 0),
                  pos }
                );
        return builder.CreateLoad(index);
    }
    return builder.CreateLoad(var, name.c_str());
}

void var_access::dump(int s) const {
    print_spaces(s);
    std::cout << "var_access name: " << name << std::endl;
    for (auto e : idxs)
        e->dump(s + 4);
}

/*
 * var_assign class
 */
var_assign::var_assign(const std::string &n) : name{n} {}

var_assign::~var_assign() {
    for (auto e : idxs)
        delete e;
}

std::string var_assign::get_name() const {
    return name;
}

void var_assign::add_idx(expr *e) {
    idxs.push_back(e);
}

llvm::Value *var_assign::get_idx() const {
    if (idxs.empty())
        return nullptr; 
    return idxs.front()->gen_ir();
}

llvm::Value *var_assign::gen_ir() {
    return named_vals[name];
}

void var_assign::dump(int s) const {
    print_spaces(s);
    std::cout << "var_assign name: " << name << std::endl;
    for (auto e : idxs)
        e->dump(s + 4);
}

/*
 * numb class
 */
numb::numb(long int v) : val{v} {}

llvm::Value *numb::gen_ir() {
    return llvm::ConstantInt::getSigned(llvm::IntegerType::getInt64Ty(context), val);
}

void numb::dump(int s) const {
    print_spaces(s);
    std::cout << "numb: " << val << std::endl;
}

/*
 * stmt class
 */
stmt::~stmt() {}

void stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "stmt" << std::endl;
}

/*
 * stmt_list class
 */
stmt_list::stmt_list(stmt* s, stmt_list* l) : statement{s}, next{l} {}

stmt_list::~stmt_list() {
    delete statement;
    delete next;
}

llvm::Value *stmt_list::gen_ir() {
    auto s = statement->gen_ir();
    next->gen_ir();
    return s;
}

void stmt_list::dump(int s) const {
    print_spaces(s);
    std::cout << "stmt_list" << std::endl;
    statement->dump(s + 4);
    next->dump(s);
}

/*
 * null_stmt_list class
 */
null_stmt_list::null_stmt_list() : stmt_list{nullptr, nullptr} {}

llvm::Value *null_stmt_list::gen_ir() {
    return nullptr;
}
void null_stmt_list::dump(int s) const {
    print_spaces(s);
    std::cout << "null_stmt_list" << std::endl;
}

/*
 * const_decl class
 */
const_decl::const_decl(const std::string& n, int v) : name{n}, val{v} {}

llvm::Value *const_decl::gen_ir() {
    if (const_vals.count(name) != 0)
        return nullptr;
    auto a = builder.CreateAlloca(llvm::Type::getInt64Ty(context), 0, name.c_str());
    const_vals[name] = a;
    auto v = llvm::ConstantInt::getSigned(llvm::IntegerType::getInt64Ty(context), val);
    builder.CreateStore(v, a);
    return a;
}

void const_decl::dump(int s) const {
    print_spaces(s);
    std::cout << "const_decl name: " << name << " val: " << val << std::endl;
}

/*
 * var_decl class
 */
var_decl::var_decl(const std::string& n) : name{n}, var_type{nullptr} {}

void var_decl::add_type(std::shared_ptr<type> t) {
    var_type = t;
}

llvm::Value *var_decl::gen_ir() {
    if (named_vals.count(name) != 0)
        return nullptr;
    llvm::AllocaInst *a = nullptr;
    if (var_type->get_type() == TYPE_INT) {
        a = builder.CreateAlloca(llvm::Type::getInt64Ty(context),
                nullptr, name.c_str());
    } else if (var_type->get_type() == TYPE_ARR) {
        a = builder.CreateAlloca(llvm::ArrayType::get(
                    llvm::Type::getInt64Ty(context), var_type->get_size()
                    ), nullptr, name.c_str());
        arr_starts[name] = -var_type->get_from();
    }
    named_vals[name] = a;
    const_vals[name] = a;
    return a;
}

void var_decl::dump(int s) const {
    print_spaces(s);
    std::cout << "var name: " << name << std::endl;
    var_type->dump(s + 4);
}

/*
 * type class
 */
type::~type() {}

llvm::Value *type::gen_ir() { return nullptr; }

/*
 * int_type class
 */
int int_type::get_type() const {
    return TYPE_INT;
}

int int_type::get_size() const {
    return 0;
}

int int_type::get_from() const {
    return 0;
}

void int_type::dump(int s) const {
    print_spaces(s);
    std::cout << "int_type" << std::endl;
}

/*
 * array_type class
 */
array_type::array_type(int f, int t) : from{f}, to{t} {}

int array_type::get_type() const {
    return TYPE_ARR;
}

int array_type::get_size() const {
    return to - from + 1;
}

int array_type::get_from() const {
    return from;
}

void array_type::dump(int s) const {
    print_spaces(s);
    std::cout << "array_type" << std::endl;
}

/*
 * proc_decl class
 */
proc_decl::proc_decl(const std::string &n, std::list<std::string> a, block *b)
    : name{n}, args{std::move(a)}, body{b} {}

llvm::Value *proc_decl::gen_ir() {
    auto prev_bb = builder.GetInsertBlock();

    auto fun = module->getFunction(name);
    if (fun == nullptr) {
        auto arguments = std::vector<llvm::Type *>(args.size(), llvm::Type::getInt64Ty(context));
        auto fun_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context), arguments, false);
        fun = llvm::Function::Create(fun_type, llvm::Function::ExternalLinkage, name, module.get());
        auto it = args.begin();
        for (auto &arg : fun->args())
            arg.setName(*(it++));
    }

    if (body != nullptr) {
        auto backup_named = std::map<std::string, llvm::AllocaInst *>(named_vals);
        auto backup_const = std::map<std::string, llvm::AllocaInst *>(const_vals);
        auto backup_arr = std::map<std::string, long int>(arr_starts);
        named_vals.clear();
        const_vals.clear();
        arr_starts.clear();

        auto bb = llvm::BasicBlock::Create(context, "entry", fun);
        builder.SetInsertPoint(bb);

        auto a = builder.CreateAlloca(llvm::Type::getInt64Ty(context), nullptr, name.c_str());
        named_vals[name] = a;
        const_vals[name] = a;
        for (auto &arg : fun->args()) {
            a = builder.CreateAlloca(llvm::Type::getInt64Ty(context), nullptr, arg.getName());
            builder.CreateStore(&arg, a);
            named_vals[arg.getName()] = a;
            const_vals[arg.getName()] = a;
        }

        body->gen_ir();
        builder.CreateRetVoid();
        verifyFunction(*fun);

        named_vals = backup_named;
        const_vals = backup_const;
        arr_starts = backup_arr;
    }

    builder.SetInsertPoint(prev_bb);

    return fun;
}

void proc_decl::dump(int s) const {
    print_spaces(s);
    std::cout << "proc_decl name: " << name << " args:";
    for (auto &a : args)
        std::cout << " " << a;
    std::cout << std::endl;
    if (body != nullptr) {
        body->dump(s + 4);
    } else {
        print_spaces(s + 4);
        std::cout << "forward" << std::endl;
    }
}

/*
 * func_decl class
 */
func_decl::func_decl(const std::string &n, std::list<std::string> a, block *b)
    : name{n}, args{std::move(a)}, body{b} {}

llvm::Value *func_decl::gen_ir() {
    auto prev_bb = builder.GetInsertBlock();

    auto fun = module->getFunction(name);
    if (fun == nullptr) {
        auto arguments = std::vector<llvm::Type *>(args.size(), llvm::Type::getInt64Ty(context));
        auto fun_type = llvm::FunctionType::get( llvm::Type::getInt64Ty(context), arguments, false);
        fun = llvm::Function::Create(fun_type, llvm::Function::ExternalLinkage, name, module.get());
        auto it = args.begin();
        for (auto &arg : fun->args())
            arg.setName(*(it++));
    }

    if (body != nullptr) {
        auto backup_named = std::map<std::string, llvm::AllocaInst *>(named_vals);
        auto backup_const = std::map<std::string, llvm::AllocaInst *>(const_vals);
        auto backup_arr = std::map<std::string, long int>(arr_starts);
        named_vals.clear();
        const_vals.clear();
        arr_starts.clear();

        auto bb = llvm::BasicBlock::Create(context, "entry", fun);
        builder.SetInsertPoint(bb);

        auto a = builder.CreateAlloca(llvm::Type::getInt64Ty(context), nullptr, name.c_str());
        named_vals[name] = a;
        const_vals[name] = a;
        for (auto &arg : fun->args()) {
            a = builder.CreateAlloca(llvm::Type::getInt64Ty(context), nullptr, arg.getName());
            builder.CreateStore(&arg, a);
            named_vals[arg.getName()] = a;
            const_vals[arg.getName()] = a;
        }

        body->gen_ir();
        auto ret_val = builder.CreateLoad(named_vals[name], name.c_str());
        builder.CreateRet(ret_val);
        verifyFunction(*fun);

        named_vals = backup_named;
        const_vals = backup_const;
        arr_starts = backup_arr;
    }

    builder.SetInsertPoint(prev_bb);

    return fun;
}

void func_decl::dump(int s) const {
    print_spaces(s);
    std::cout << "func_decl name: " << name << " args:";
    for (auto &a : args)
        std::cout << " " << a;
    std::cout << std::endl;
    if (body != nullptr) {
        body->dump(s + 4);
    } else {
        print_spaces(s + 4);
        std::cout << "forward" << std::endl;
    }
}

/*
 * compound_stmt class
 */
compound_stmt::compound_stmt(stmt_list *l) : list{l} {}

compound_stmt::~compound_stmt() {
    delete list;
}

llvm::Value *compound_stmt::gen_ir() {
    return list->gen_ir();
}

void compound_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "compound_stmt" << std::endl;
    list->dump(s + 4);
}

/*
 * assign_stmt class
 */
assign_stmt::assign_stmt(var_assign *v , expr *e) : var{v}, expression{e} {}

assign_stmt::~assign_stmt() {
    delete var;
    delete expression;
}

llvm::Value *assign_stmt::gen_ir() {
    auto variable = var->gen_ir();
    auto e = expression->gen_ir();
    auto index = var->get_idx();
    if (index == nullptr) {
        builder.CreateStore(e, variable);
    } else {
        auto pos = builder.CreateAdd(index, llvm::ConstantInt::get(llvm::IntegerType::getInt64Ty(context), arr_starts[var->get_name()]));
        auto position = builder.CreateInBoundsGEP(variable,
                {llvm::ConstantInt::get(llvm::IntegerType::getInt64Ty(context), 0), pos});
        builder.CreateStore(e, position);
    }
    return e;
}

void assign_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "assign_stmt" << std::endl;
    var->dump(s + 4);
    expression->dump(s + 4);
}

/*
 * if_stmt class
 */
if_stmt::if_stmt(expr *c, stmt *t, stmt *e)
    : condition{c}, then_stmt{t}, else_stmt{e} {}

if_stmt::~if_stmt() {
    delete condition;
    delete then_stmt;
    delete else_stmt;
}

llvm::Value *if_stmt::gen_ir() {
    auto c = condition->gen_ir();
    auto fun = builder.GetInsertBlock()->getParent();
    auto then_bb = llvm::BasicBlock::Create(context, "then", fun);
    auto else_bb = llvm::BasicBlock::Create(context, "else");
    auto con_bb = llvm::BasicBlock::Create(context, "ifcon");

    builder.CreateCondBr(c, then_bb, else_bb);

    builder.SetInsertPoint(then_bb);
    then_stmt->gen_ir();
    builder.CreateBr(con_bb);

    fun->getBasicBlockList().push_back(else_bb);
    builder.SetInsertPoint(else_bb);
    else_stmt->gen_ir();
    builder.CreateBr(con_bb);

    fun->getBasicBlockList().push_back(con_bb);
    builder.SetInsertPoint(con_bb);

    return con_bb;
}

void if_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "if_stmt" << std::endl;
    then_stmt->dump(s + 4);
    else_stmt->dump(s + 4);
}

/*
 * while class
 */
while_stmt::while_stmt(expr *c, stmt *b) : condition{c}, body{b} {}

while_stmt::~while_stmt() {
    delete condition;
    delete body;
}

llvm::Value *while_stmt::gen_ir() {
    auto fun = builder.GetInsertBlock()->getParent();

    auto cond = llvm::BasicBlock::Create(context, "cond", fun);
    auto loop = llvm::BasicBlock::Create(context, "loop", fun);
    auto after = llvm::BasicBlock::Create(context, "after");
    auto backup_break = break_bb;
    break_bb = after;

    builder.CreateBr(cond);
    builder.SetInsertPoint(cond);
    auto c = condition->gen_ir();
    builder.CreateCondBr(c, loop, after);

    builder.SetInsertPoint(loop);
    body->gen_ir();
    builder.CreateBr(cond);

    fun->getBasicBlockList().push_back(after);
    builder.SetInsertPoint(after);

    break_bb = backup_break;

    return after;
}
void while_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "while_stmt" << std::endl;
    condition->dump(s + 4);
    body->dump(s + 4);
}

/*
 * for_stmt class
 */
for_stmt::for_stmt(const std::string &n, expr *f, int d, expr *t, stmt *b)
    : name{n}, from{f}, to{t}, dir{d}, body{b} {}

for_stmt::~for_stmt() {
    delete from;
    delete to;
    delete body;
}

llvm::Value *for_stmt::gen_ir() {
    /* get current function */
    auto fun = builder.GetInsertBlock()->getParent();

    /* look up index variable and store from expression */
    auto v = named_vals[name];
    auto f = from->gen_ir();
    builder.CreateStore(f, v);

    auto cond = llvm::BasicBlock::Create(context, "cond", fun);
    auto loop = llvm::BasicBlock::Create(context, "loop", fun);
    auto after = llvm::BasicBlock::Create(context, "after");
    auto backup_break = break_bb;
    break_bb = after;
    builder.CreateBr(cond);

    builder.SetInsertPoint(cond);

    auto cur_val = builder.CreateLoad(v, name.c_str());
    auto step = llvm::ConstantInt::getSigned(
            llvm::IntegerType::getInt64Ty(context), dir);
    auto t = to->gen_ir();
    if (dir == DIR_TO)
        t = builder.CreateICmpSLE(cur_val, t, "le");
    else
        t = builder.CreateICmpSGE(cur_val, t, "ge");

    builder.CreateCondBr(t, loop, after);

    builder.SetInsertPoint(loop);
    body->gen_ir();

    cur_val = builder.CreateLoad(v, name.c_str());
    auto next_val = builder.CreateAdd(cur_val, step, "nextval");
    builder.CreateStore(next_val, v);
    builder.CreateBr(cond);

    fun->getBasicBlockList().push_back(after);
    builder.SetInsertPoint(after);

    break_bb = backup_break;

    return after;
}

void for_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "for_stmt name: " << name << " dir: " << dir << std::endl;
    from->dump(s + 4);
    to->dump(s + 4);
    body->dump(s + 4);
}

/*
 * exit class
 */
llvm::Value *exit_stmt::gen_ir() {
    auto fun = builder.GetInsertBlock()->getParent()->getName();
    auto ret = builder.CreateLoad(named_vals[fun], "exit");
    return builder.CreateRet(ret);
}

void exit_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "exit_stmt" << std::endl;
}

/*
 * dec_stmt class
 */
dec_stmt::dec_stmt(var_assign *v) : var{v} {}

dec_stmt::~dec_stmt() {
    delete var;
}


llvm::Value *dec_stmt::gen_ir() {
    auto v = var->gen_ir();
    auto c = builder.CreateLoad(v, "dec");
    auto n = builder.CreateAdd(c, llvm::ConstantInt::getSigned(
                llvm::IntegerType::getInt64Ty(context), -1), "dec");
    return builder.CreateStore(n, v);
}

void dec_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "dec_stmt" << std::endl;
    var->dump(s + 4);
}

/*
 * inc_stmt class
 */
inc_stmt::inc_stmt(var_assign *v) : var{v} {}

inc_stmt::~inc_stmt() {
    delete var;
}


llvm::Value *inc_stmt::gen_ir() {
    auto v = var->gen_ir();
    auto c = builder.CreateLoad(v, "inc");
    auto n = builder.CreateAdd(c, llvm::ConstantInt::getSigned(
                llvm::IntegerType::getInt64Ty(context), 1), "inc");
    
    return builder.CreateStore(n, v);
}

void inc_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "dec_stmt" << std::endl;
    var->dump(s + 4);
}

/*
 * readln_stmt class
 */
readln_stmt::readln_stmt(var_assign *v) : var{v} {}

readln_stmt::~readln_stmt() {
    delete var;
}

extern "C" int64_t scanln() {
    int64_t x;
    scanf("%ld", &x);
    return x;
}

llvm::Value *readln_stmt::gen_ir() {
    auto v = var->gen_ir();
    auto c = builder.CreateCall(scanln_fun,
            std::vector<llvm::Value *>{}, "scanln");
    return builder.CreateStore(c, v);
}

void readln_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "readln_stmt" << std::endl;
    var->dump(s + 4);
}

/*
 * write_stmt class
 */
write_stmt::write_stmt(expr *e) : expression{e} {}
write_stmt::~write_stmt() {
    delete expression;
}

extern "C" void print(int64_t x) {
    printf("%ld", x);
}

llvm::Value *write_stmt::gen_ir() {
    return builder.CreateCall(print_fun,
            std::vector<llvm::Value *>{expression->gen_ir()}, "");
}

void write_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "write_stmt" << std::endl;
    expression->dump(s + 4);
}

/*
 * writeln_stmt class
 */
writeln_stmt::writeln_stmt(expr *e) : expression{e} {}

writeln_stmt::~writeln_stmt() {
    delete expression;
}

extern "C" void println(int64_t x) {
    printf("%ld\n", x);
}

llvm::Value *writeln_stmt::gen_ir() {
    return builder.CreateCall(println_fun,
            std::vector<llvm::Value *>{expression->gen_ir()}, "");
}

void writeln_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "writeln_stmt" << std::endl;
    expression->dump(s + 4);
}

/*
 * break_stmt class
 */
break_stmt::break_stmt() {}

llvm::Value *break_stmt::gen_ir() {
    if (break_bb == nullptr) {
        std::cout << "break_stmt error" << std::endl;
        return nullptr;
    }
    return builder.CreateBr(break_bb);
}

void break_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "break_stmt" << std::endl;
}

/*
 * null_stmt class
 */
void null_stmt::dump(int s) const {
    print_spaces(s);
    std::cout << "null_stmt" << std::endl;
}

/*
 * main fun
 */
void define_base_func() {
    scanln_fun = llvm::Function::Create(
            llvm::FunctionType::get(llvm::Type::getInt64Ty(context),
                std::vector<llvm::Type *>{},
                false), llvm::Function::ExternalLinkage, "scanln", module.get());

    println_fun = llvm::Function::Create(
            llvm::FunctionType::get(llvm::Type::getVoidTy(context),
                std::vector<llvm::Type *>(1, llvm::Type::getInt64Ty(context)),
                false), llvm::Function::ExternalLinkage, "println", module.get());

    print_fun = llvm::Function::Create(
            llvm::FunctionType::get(llvm::Type::getVoidTy(context),
                std::vector<llvm::Type *>(1, llvm::Type::getInt64Ty(context)),
                false), llvm::Function::ExternalLinkage, "print", module.get());
}

int main(int argc, char **argv) {
    if (argc != 2) /* only 1 file to compile */
        return EXIT_FAILURE;
    auto in = new std::ifstream{argv[1]};
    if (in == nullptr) /* cannot opein file */
        return EXIT_FAILURE;

    auto parser = yyParser{in};

    /* create AST */
    auto root = parser.yyparse();
    root->dump(0);

    /* init objects */
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    jit = llvm::make_unique<llvm::orc::KaleidoscopeJIT>();

    module = llvm::make_unique<llvm::Module>("module", llvm::getGlobalContext());
    module->setDataLayout(jit->getTargetMachine().createDataLayout());

    /* define writeln, write and readln */
    define_base_func();

    auto fun_type = llvm::FunctionType::get(llvm::Type::getInt8Ty(context),
            std::vector<llvm::Type *>{}, false);
    auto fun = llvm::Function::Create(fun_type,
            llvm::Function::ExternalLinkage, "main", module.get());

    /* parse and generate LLVM IR */
    auto basic_block = llvm::BasicBlock::Create(context, "main_block", fun);
    builder.SetInsertPoint(basic_block);
    root->gen_ir();
    builder.CreateRet(llvm::ConstantInt::getSigned(
                llvm::IntegerType::getInt8Ty(context), 0));
    verifyFunction(*fun);

    module->dump(); /* print generated llvm ir */

    auto h = jit->addModule(std::move(module));

    auto symbol = jit->findSymbol("main");
    assert(symbol && "func not found");
    int (*fun_ptr)() = (int (*)())(intptr_t)symbol.getAddress();
    std::cout << "evaluation:" << std::endl;
    fun_ptr();

    jit->removeModule(h);

    delete root;
    delete in;
    return EXIT_SUCCESS;
}
