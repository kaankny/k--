#include "../Parser/Parser.hpp"
#include "../Lexer/Lexer.hpp"
#include "../Logger.hpp"
#include "../messages.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Windows x64 uyumlu register listesi
static const char *reglist[] = { "rcx", "rdx", "r8", "r9" };
static int nextReg = 0;

class CodeGenerator {
public:
    std::ofstream outfile;

    CodeGenerator(const std::string &filename) {
        outfile.open(filename);
        if (!outfile.is_open()) {
            Logger::getInstance().log(LogLevel::ERROR, "Output file couldn't be opened.");
            exit(1);
        }
    }

    ~CodeGenerator() {
        outfile.close();
    }

    void generate(t_ast_node *root) {
        preamble();
        int reg = genAST(root);
        printResult(reg);
        postamble();
    }

private:
    void preamble() {
		outfile << "section .data\n";
		outfile << "    fmt db \"%d\", 10, 0\n"; // "%d\n" format string
		outfile << "section .text\n";
		outfile << "    extern printf\n";
		outfile << "    global main\n";
		outfile << "main:\n";
		outfile << "    push rbp\n";
		outfile << "    mov rbp, rsp\n";
		outfile << "    sub rsp, 32\n"; // Align stack to 16 bytes
	}


    void postamble() {
		outfile << "    add rsp, 32\n"; // Restore stack pointer
		outfile << "    mov rax, 0\n"; // Return 0 from main
		outfile << "    pop rbp\n";
		outfile << "    ret\n";
	}


    int allocRegister() {
        if (nextReg >= 4) {
            Logger::getInstance().log(LogLevel::ERROR, "Out of registers!");
            exit(1);
        }
        return nextReg++;
    }

    void freeRegister(int reg) {
        if (nextReg > 0) nextReg--;
    }

    int genAST(t_ast_node *node) {
        if (node == nullptr) return -1;

        int leftReg, rightReg;

        if (node->left) leftReg = genAST(node->left);
        if (node->right) rightReg = genAST(node->right);

        switch (node->op) {
            case AST_NODE_TYPE_ADD:
                return cgAdd(leftReg, rightReg);
            case AST_NODE_TYPE_SUB:
                return cgSub(leftReg, rightReg);
            case AST_NODE_TYPE_MUL:
                return cgMul(leftReg, rightReg);
            case AST_NODE_TYPE_DIV:
                return cgDiv(leftReg, rightReg);
            case AST_NODE_TYPE_INTLIT:
                return cgLoad(node->intValue);
            default:
                Logger::getInstance().log(LogLevel::ERROR, "Unknown AST operator.");
                exit(1);
        }
    }

    int cgLoad(int value) {
        int reg = allocRegister();
        outfile << "\tmov " << reglist[reg] << ", " << value << "\n";
        return reg;
    }

    int cgAdd(int reg1, int reg2) {
        outfile << "\tadd " << reglist[reg2] << ", " << reglist[reg1] << "\n";
        freeRegister(reg1);
        return reg2;
    }

    int cgSub(int reg1, int reg2) {
        outfile << "\tsub " << reglist[reg1] << ", " << reglist[reg2] << "\n";
        freeRegister(reg2);
        return reg1;
    }

    int cgMul(int reg1, int reg2) {
        outfile << "\timul " << reglist[reg2] << ", " << reglist[reg1] << "\n";
        freeRegister(reg1);
        return reg2;
    }

    int cgDiv(int reg1, int reg2) {
		outfile << "    mov rax, " << reglist[reg1] << "\n";
		outfile << "    cqo\n"; // Sign extend rax into rdx
		outfile << "    idiv " << reglist[reg2] << "\n";
		outfile << "    mov " << reglist[reg1] << ", rax\n";
		freeRegister(reg2);
		return reg1;
	}


    void printResult(int reg) {
		outfile << "    lea rcx, [fmt]\n"; // Load address of format string into rcx
		outfile << "    mov rdx, " << reglist[reg] << "\n"; // Move result into rdx
		outfile << "    call printf\n";
		freeRegister(reg);
	}

};
