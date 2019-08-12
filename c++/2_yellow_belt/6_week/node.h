#ifndef W6_NODE_H
#define W6_NODE_H

#include "date.h"

#include <string>
#include <memory>

enum class Comparison{
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
};

enum class LogicalOperation{
    And,
    Or,
};


class Node {
public:
    Node(const Comparison cmp): cmp(cmp){}

    virtual bool Evaluate(const Date& date, const std::string& event) = 0;

    Comparison GetComparison() const {
        return cmp;
    }
private:
    const Comparison cmp;
};

class EmptyNode: public Node {
public:
    EmptyNode(): Node({}){}

    bool Evaluate(const Date& date, const std::string& event) override;
};

class DateComparisonNode: public Node {
public:
    DateComparisonNode(
            const Comparison &cmp,
            const Date &date):
            Node(cmp),
            d(date){}

    bool Evaluate(const Date& date, const std::string& event) override;
private:
    const Date d;
};

class EventComparisonNode: public Node{
public:
    EventComparisonNode(
            const Comparison& cmp,
            const std::string& value):
            Node(cmp),
            ev(value){}

    bool Evaluate(const Date& date, const std::string& event) override;
private:
    const std::string ev;
};

class LogicalOperationNode: public Node {
public:
    LogicalOperationNode(
            const LogicalOperation& logical_operation,
            const std::shared_ptr<Node>& left,
            const std::shared_ptr<Node>& rest):
            Node({}),
            logical_operation(logical_operation),
            left(left),
            rest(rest){}

    bool Evaluate(const Date& date, const std::string& event) override;
private:
    const LogicalOperation logical_operation;
    const std::shared_ptr<Node> left;
    const std::shared_ptr<Node> rest;
};

#endif //W6_NODE_H
