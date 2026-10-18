#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <string>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(int depth = 0) const = 0;
    // NEW: Evaluates the tree using a runtime input 'x'
    virtual double evaluate(double x_val) const = 0;
};

class NumberNode : public ASTNode {
private:
    double value;
public:
    NumberNode(double val) : value(val) {}

    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Number: " << value << "\n";
    }

    double evaluate(double) const override {
        return value; // A fixed number ignores the input
    }
};

// NEW: Node representing a variable input (like 'x' or a track attribute)
class VariableNode : public ASTNode {
public:
    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Variable: [x]\n";
    }

    double evaluate(double x_val) const override {
        return x_val; // Returns whatever input value was passed in!
    }
};

class BinaryOpNode : public ASTNode {
private:
    char op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
public:
    BinaryOpNode(char op, ASTNode* l, ASTNode* r)
        : op(op), left(l), right(r) {}

    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Operator: [" << op << "]\n";
        if (left)  left->print(depth + 4);
        if (right) right->print(depth + 4);
    }

    double evaluate(double x_val) const override {
        double l_val = left ? left->evaluate(x_val) : 0;
        double r_val = right ? right->evaluate(x_val) : 0;

        switch (op) {
            case '+': return l_val + r_val;
            case '-': return l_val - r_val;
            case '*': return l_val * r_val;
            case '/': return (r_val != 0) ? (l_val / r_val) : 0;
            case '<': return l_val < r_val;
            case '>': return l_val > r_val;
            default: return 0;
        }
    }
};

class UnaryOpNode : public ASTNode {
private:
    char op;
    std::unique_ptr<ASTNode> val_node;
public:
    UnaryOpNode(char op, ASTNode* v)
        : op(op), val_node(v) {}

    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Operator: [" << op << "]\n";
        if (val_node)  val_node->print(depth + 4);
        //TODO What is depth? Is +4 correct for Unary?
    }

    double evaluate(double x_val) const override {
        double val = val_node ? val_node->evaluate(x_val) : 0;

        switch (op) {
            case '+': return val;
            case '-': return -val;
            case '!': return !val;
            //TODO perhaps seperate float / int nodes
            //NOTE So bool_expr arith_expr seperation
            //TODO Or type int/float union
            default: return 0;
        }
    }
};

#endif // AST_H
