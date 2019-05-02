#include "node.h"

bool EmptyNode::Evaluate(const Date &date, const std::string &event) {
    return true;
}

bool DateComparisonNode::Evaluate(const Date &date, const std::string &event) {
    switch (GetComparison()){
        case Comparison::Less:
            return date < d;
        case Comparison::GreaterOrEqual:
            return date >= d;
        case Comparison::LessOrEqual:
            return date <= d;
        case Comparison::Greater:
            return date > d;
        case Comparison::NotEqual:
            return date != d;
        case Comparison::Equal:
            return date == d;
        default:
            return true;
    }
}

bool EventComparisonNode::Evaluate(const Date &date, const std::string &event) {
    switch (GetComparison()){
        case Comparison::Less:
            return event < ev;
        case Comparison::GreaterOrEqual:
            return event >= ev;
        case Comparison::LessOrEqual:
            return event <= ev;
        case Comparison::Greater:
            return event > ev;
        case Comparison::NotEqual:
            return event != ev;
        case Comparison::Equal:
            return event == ev;
        default:
            return true;
    }
}

bool LogicalOperationNode::Evaluate(const Date &date, const std::string &event) {
    bool left_part = left->Evaluate(date, event);
    bool right_part = rest->Evaluate(date, event);
    switch (logical_operation){
        case LogicalOperation::And:
            return left_part && right_part;
        case LogicalOperation::Or:
            return left_part || right_part;
        default:
            return true;
    }
}