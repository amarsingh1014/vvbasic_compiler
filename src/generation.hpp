#pragma once 

#include "./parser.hpp"
#include<unordered_map>

class Generator {
    public:
    inline Generator(NodeProg prog) : m_prog(std::move(prog)) {}



    void gen_expr (const NodeExpr expr) {
         struct ExprVisitor {
            Generator* gen;
            

                void operator() (const NodeExprIntLit& expr_intlit) {
                       gen->ss << "     mov rax, " << expr_intlit.INT_LIT.value.value() <<"\n";     
                       gen->push("rax");
             }

                void operator() (const NodeExprIdent& expr_ident) 
                {  
                        if (!gen->m_vars.contains(expr_ident.ident.value.value())) {
                            std::cerr << "Undeclared identifier: " << expr_ident.ident.value.value() << std::endl;
                            exit(EXIT_FAILURE);
                        } 
                        const auto& var = gen->m_vars.at(expr_ident.ident.value.value());
                        std::stringstream offset;
                        offset << " QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1) * 8 <<"]";
                        gen->push(offset.str());       
                    }
               };

            ExprVisitor visitor {.gen = this};
            std::visit(visitor, expr.var);
    }


    void gen_stmt(const NodeStmt& stmt) {
        struct StmtVisitor {
                Generator* gen;
                
                void operator() (const NodeStmtExit& stmt_exit) {
                 gen->gen_expr(stmt_exit.expr);   

                 gen->ss << "     mov rax, 60\n";
                 gen->ss << "     pop rdi\n";  
                 gen->ss << "     syscall"<<"\n";     
             }

                void operator() (const NodeStmtLet& stmt_let) {
                     if(gen->m_vars.contains(stmt_let.ident.value.value())) { 
                     std::cerr << "Identifier already used" << stmt_let.ident.value.value() << std::endl;  
                     exit(EXIT_FAILURE);  
             }
              gen->m_vars.insert({stmt_let.ident.value.value(), Var {.stack_loc = gen->m_stack_size}});
              gen->gen_expr(stmt_let.expr);

          }
        };
            StmtVisitor visitor {.gen = this};
            std::visit(visitor, stmt.var);
    }
    

    [[nodiscard]] std::string gen_prog() {

     ss<<"global _start\n_start:\n";
     
    for (const NodeStmt& stmt : m_prog.stmts) {
        gen_stmt(stmt);
    }



     ss << "     mov rax, 60\n";
     ss << "     mov rdi, 0\n";
     ss << "     syscall"<<"\n";
     
     return ss.str();
     }

    
    private:
    
    void push(const std::string& reg) {
        ss << "     push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const std::string& reg) {
        ss << "   pop " << reg << "\n";
        m_stack_size--;

    }

    struct Var {
        size_t stack_loc;
    };

    const NodeProg m_prog;
    std::stringstream ss;
    size_t m_stack_size;
    std::unordered_map<std::string, Var> m_vars {};
};