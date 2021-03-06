#ifndef _ast_h_k38skxu3o2pxj3ua
#define _ast_h_k38skxu3o2pxj3ua

#include <list>
#include <memory>
#include <string>

#include "llvm/IR/Value.h"

namespace ast {

const int DIR_TO = 1;
const int DIR_DOWNTO = -1;

const int TYPE_INT = 1;
const int TYPE_ARR = 2;

/* 
 * node abstract class
 * Base class for all nodes of AST.
 */
class node {
    public:
        virtual ~node();
        virtual void dump(int) const = 0;
        virtual llvm::Value *gen_ir() = 0;
        void print_spaces(int) const;
};

/*
 * type class
 * Base class for types of var_decl class.
 */
class type : public node {
    public:
        virtual ~type();
        virtual int get_type() const = 0;
        virtual int get_size() const = 0;
        virtual int get_from() const = 0;
        virtual llvm::Value *gen_ir();
};

class int_type : public type {
    public:
        virtual int get_type() const;
        virtual int get_size() const;
        virtual int get_from() const;
        virtual void dump(int) const;
};

class array_type : public type {
    protected:
        int from, to;
    public:
        array_type(int, int);
        virtual int get_type() const;
        virtual int get_size() const;
        virtual int get_from() const;
        virtual void dump(int) const;
};

/* base decl class */
class decl : public node {
    public:
        virtual void add_type(std::shared_ptr<type>);
        virtual ~decl();
};

class decl_list : public decl {
    protected:
        decl *declaration;
        decl_list *next;
    public:
        decl_list(decl *, decl_list *);
        virtual ~decl_list();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class null_decl_list : public decl_list {
    public:
        null_decl_list();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class var_decl_list : public decl_list {
    public:
        var_decl_list(decl *, decl_list *);
        virtual void add_type(std::shared_ptr<type>);
        virtual void dump(int) const;
};

/* base stmt class */
class stmt : public node {
    public:
        virtual ~stmt();
        virtual llvm::Value *gen_ir() { return nullptr; }
        virtual void dump(int) const;
};

class stmt_list : public stmt {
    protected:
        stmt *statement;
        stmt_list *next;
    public:
        stmt_list(stmt*, stmt_list*);
        virtual ~stmt_list();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class null_stmt_list : public stmt_list {
    public:
        null_stmt_list();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

/* base block class */
class block : public node {
    protected:
        decl_list *decls;
        stmt *body;
    public:
        block(decl_list *, stmt *);
        virtual ~block();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

/* base expr class */
class expr : public node {
    public:
        virtual ~expr();
};

/* declarations */
class const_decl : public decl {
    protected:
        std::string name;
        int val;
    public:
        const_decl(const std::string&, int);
        llvm::Value *gen_ir(); 
        virtual void dump(int) const;
};

class var_decl : public decl {
    protected:
        std::string name;
        std::shared_ptr<type> var_type;
    public:
        var_decl(const std::string&);
        virtual void add_type(std::shared_ptr<type> t);
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class proc_decl : public decl {
    protected:
        std::string name;
        std::list<std::string> args;
        block *body;
    public:
        proc_decl(const std::string &, std::list<std::string>, block *);
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class func_decl : public decl {
    protected:
        std::string name;
        std::list<std::string> args;
        block *body;
    public:
        func_decl(const std::string &, std::list<std::string>, block *);
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

/* expressions */
class binary_expr : public expr {
    protected:
        expr *left, *right;
    public:
        binary_expr(expr *, expr *);
        virtual ~binary_expr();
};

class eq_expr : public binary_expr {
    public:
        eq_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
}; 

class ne_expr : public binary_expr {
    public:
        ne_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class lt_expr : public binary_expr {
    public:
        lt_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class gt_expr : public binary_expr {
    public:
        gt_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class le_expr : public binary_expr {
    public:
        le_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class ge_expr : public binary_expr {
    public:
        ge_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class add_expr : public binary_expr {
    public:
        add_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class sub_expr : public binary_expr {
    public:
        sub_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class or_expr : public binary_expr {
    public:
        or_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class mul_expr : public binary_expr {
    public:
        mul_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class div_expr : public binary_expr {
    public:
        div_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class mod_expr : public binary_expr {
    public:
        mod_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class and_expr : public binary_expr {
    public:
        and_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class exp_expr : public binary_expr {
    public:
        exp_expr(expr *, expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class unary_expr : public expr {
    protected:
        expr *child;
    public:
        unary_expr(expr *);
        virtual ~unary_expr();
};

class minus_expr : public unary_expr {
    public:
        minus_expr(expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class not_expr : public unary_expr {
    public:
        not_expr(expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

/*
 * proc_call class
 */
class proc_call : public stmt {
    protected:
        const std::string name;
        std::list<expr *> params;
    public:
        proc_call(const std::string &, std::list<expr *>);
        ~proc_call();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

/*
 * call class
 */
class call : public expr {
    protected:
        const std::string name;
        std::list<expr *> params;
    public:
        call(const std::string &, std::list<expr *>);
        ~call();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class var_access : public expr {
    protected:
        std::string name;
        std::list<expr *> idxs;
    public:
        var_access(const std::string &);
        ~var_access();
        void add_idx(expr *);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class var_assign : public node {
    protected:
        std::string name;
        std::list<expr *> idxs;
    public:
        var_assign(const std::string &);
        ~var_assign();
        void add_idx(expr *);
        std::string get_name() const;
        llvm::Value *get_idx() const;
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class numb : public expr {
    protected:
        long int val;
    public:
        numb(long int);
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class null_expr : public expr {
    public:
        virtual void dump(int) const;
};

/* statements */
class compound_stmt : public stmt {
    protected:
        stmt_list *list;
    public:
        compound_stmt(stmt_list *);
        virtual ~compound_stmt();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class assign_stmt : public stmt {
    protected:
        var_assign *var;
        expr *expression;
    public:
        assign_stmt(var_assign *, expr *);
        llvm::Value *gen_ir();
        virtual ~assign_stmt();
        virtual void dump(int) const;
};

class if_stmt : public stmt {
    protected:
        expr *condition;
        stmt *then_stmt, *else_stmt;
    public:
        if_stmt(expr *, stmt *, stmt *);
        virtual ~if_stmt();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class while_stmt : public stmt {
    protected:
        expr *condition;
        stmt *body;
    public:
        while_stmt(expr *, stmt *);
        virtual ~while_stmt();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class for_stmt : public stmt {
    protected:
        const std::string name;
        expr *from, *to;
        const int dir;
        stmt *body;
    public:
        for_stmt(const std::string &, expr *, int, expr *, stmt *);
        virtual ~for_stmt();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class exit_stmt : public stmt {
    public:
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class dec_stmt : public stmt {
    protected:
        var_assign *var;
    public:
        dec_stmt(var_assign *);
        virtual ~dec_stmt();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class inc_stmt : public stmt {
    protected:
        var_assign *var;
    public:
        inc_stmt(var_assign *);
        virtual ~inc_stmt();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class readln_stmt : public stmt {
    protected:
        var_assign *var;
    public:
        readln_stmt(var_assign *);
        virtual ~readln_stmt();
        llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class write_stmt : public stmt {
    protected:
        expr* expression;
    public:
        write_stmt(expr *);
        virtual ~write_stmt();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class writeln_stmt : public stmt {
    protected:
        expr* expression;
    public:
        writeln_stmt(expr *);
        virtual ~writeln_stmt();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class break_stmt : public stmt {
    public:
        break_stmt();
        virtual llvm::Value *gen_ir();
        virtual void dump(int) const;
};

class null_stmt : public stmt {
    public:
        virtual void dump(int) const;
};

}

#endif /* _ast_h_k38skxu3o2pxj3ua */
