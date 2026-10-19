#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <string>
#include "../typedefs.h"

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(int depth = 0) const = 0;
    virtual double evaluate(Track* t) const = 0;
};

class NumberNode : public ASTNode {
private:
    double value;
public:
    NumberNode(double val) : value(val) {}

    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Number: " << value << "\n";
    }

    double evaluate(Track* t) const override {
        return value; // A fixed number ignores the input
    }
};

//Node representing a variable input, i.e. a track attribute
class VariableNode : public ASTNode {
private:
    unsigned int track_attr_num;
public:
    VariableNode(unsigned int tan) : track_attr_num(tan) {}

    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Track Attr Num: "
        << track_attr_num << "\n";
    }

    double evaluate(Track* t) const override {
        switch (track_attr_num) {
          case 0: return t->id;

          case 1: return t->start_x;
          case 2: return t->start_y;
          case 3: return t->start_z;

          case 4: return t->end_x;
          case 5: return t->end_y;
          case 6: return t->end_z;

          case 7: return t->b;
          case 8: return t->c;

          case 9: return t->machine_speed;
          case 10: return t->laser_power;
          case 11: return t->laser_status;

          case 12: return t->g_code;

          case 13: return t->vradius;
          case 14: return t->hradius;
          default: return 0;
      }
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

    double evaluate(Track* t) const override {
        double l_val = left ? left->evaluate(t) : 0;
        double r_val = right ? right->evaluate(t) : 0;

        switch (op) {
            case '+': return l_val + r_val;
            case '-': return l_val - r_val;
            case '*': return l_val * r_val;
            case '/': return (r_val != 0) ? (l_val / r_val) : 0;
            case 'O': return l_val || r_val;
            case 'A': return l_val && r_val;
            case 'E': return l_val == r_val;
            case 'N': return l_val != r_val;
            case 'L': return l_val <= r_val;
            case 'G': return l_val >= r_val;
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

    double evaluate(Track* t) const override {
        double val = val_node ? val_node->evaluate(t) : 0;

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

class MappingNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> bool_expr_node;
    Color color;
public:
    // Linked list next pointer
    std::unique_ptr<MappingNode> next = nullptr;

    MappingNode(ASTNode* ben, Color c)
      : bool_expr_node(ben), color(c) {}

    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Mapping Node:\n";
        std::cout << std::string(depth + 4, ' ') << "Bool_Expr:\n";
        if (bool_expr_node)  bool_expr_node->print(depth + 8);
        std::cout << std::string(depth+8, ' ') <<
        "└─Color=(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ")\n";

        if (next) {
            std::cout << std::string(depth, ' ') << "   [Next Mapping in List]:\n";
            next->print(depth);
        }
    }

    double evaluate(Track* t) const override {
        int r_eval = bool_expr_node ? bool_expr_node->evaluate(t) : 0;
        if(r_eval) {t->color=color;} //mutating track color, if bool_expr true
        //TODO finish after successfully mapping the first color
        return r_eval;
    }
};

#endif // AST_H
