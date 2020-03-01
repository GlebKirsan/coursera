#include "Common.h"

using namespace std;

#include <memory>
#include <string>

using ExpressionPtr = std::unique_ptr<Expression>;

class Node : public Expression {
public:
        explicit Node(int value) : value_(value) {}

        int Evaluate() const override {
                return value_;
        }

        std::string ToString() const override {
                return std::to_string(value_);
        }
private:
        int value_;
};

class SumNode : public Expression {
public:
        explicit SumNode(ExpressionPtr left, ExpressionPtr right)
                : left_(move(left)), right_(move(right)) {}

        int Evaluate() const override {
                return left_->Evaluate() + right_->Evaluate();
        }

        std::string ToString() const override {
                std::string left = '(' + left_->ToString() + ')';
                std::string right = '(' + right_->ToString() + ')';
                return left + '+' + right;
        }
private:
        ExpressionPtr left_;
        ExpressionPtr right_;
};

class ProductNode : public Expression {
public:
        explicit ProductNode(ExpressionPtr left, ExpressionPtr right)
                : left_(move(left)), right_(move(right)) {}

        int Evaluate() const override {
                return left_->Evaluate() * right_->Evaluate();
        }

        std::string ToString() const override {
                std::string left = '(' + left_->ToString() + ')';
                std::string right = '(' + right_->ToString() + ')';
                return left + '*' + right;
        }
private:
        ExpressionPtr left_;
        ExpressionPtr right_;
};

// Функции для формирования выражения
ExpressionPtr Value(int value) {
        return make_unique<Node>(value);
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
        return make_unique<SumNode>(move(left), move(right));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
        return make_unique<ProductNode>(move(left), move(right));
}
