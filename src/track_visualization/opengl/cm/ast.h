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
    const char* track_attr_name;
public:
    VariableNode(const char* tan) : track_attr_name(tan) {}

    void print(int depth) const override {
        std::cout << std::string(depth, ' ') << "└─ Track Attr: "
        << track_attr_name << "\n";
    }

    double evaluate(Track* t) const override {
      const char* key = track_attr_name;
      if (strcmp(key, "t_id") == 0) {return t->id;}
      else if (strcmp(key, "t_start_x") == 0) {return t->start_x;}
      else if (strcmp(key, "t_start_y") == 0) {return t->start_y;}
      else if (strcmp(key, "t_start_z") == 0) {return t->start_z;}
      else if (strcmp(key, "t_end_x") == 0) {return t->end_x;}
      else if (strcmp(key, "t_end_y") == 0) {return t->end_y;}
      else if (strcmp(key, "t_end_z") == 0) {return t->end_z;}
      else if (strcmp(key, "B") == 0) {return t->b;}
      else if (strcmp(key, "C") == 0) {return t->c;}
      else if (strcmp(key, "machine_speed") == 0) {return t->machine_speed;}
      else if (strcmp(key, "laser_power") == 0) {return t->laser_power;}
      else if (strcmp(key, "laser_status") == 0) {return t->laser_status;}
      else if (strcmp(key, "G") == 0) {return t->g_code;}
      else if (strcmp(key, "hradius") == 0) {return t->hradius;}
      else if (strcmp(key, "vradius") == 0) {return t->vradius;}
      fprintf(stderr,"Invalid track_attr_name: %s,"
        "given to VariableNode eval fn!\n",key);
      return -1; //Invalid track_attr_name!
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
