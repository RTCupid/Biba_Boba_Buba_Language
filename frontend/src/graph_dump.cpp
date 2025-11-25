#include "node.hpp"
#include <ostream>
namespace language {

void Program::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "digraph G {\n"
       << "    rankdir=TB;\n"
       << "    node [style=filled, fontname=\"Helvetica\", fontcolor=darkblue, "
       << "fillcolor=peachpuff, color=\"#252A34\", penwidth=2.5];\n"
       << "    bgcolor=\"lemonchiffon\";\n\n";

    std::size_t size = stmts_.size();

    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=salmon"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Program"
       << " | addr: " << this << " | parent: " << nullptr << "| { ";
    for (std::size_t i = 0; i < size; ++i) {
        gv << "stmt_" << i << ": " << stmts_[i].get();
        if (i + 1 < size) {
            gv << " | ";
        }
    }
    gv << " } }\"" << "];\n";

    for (const auto &stmt : stmts_) {
        if (stmt) {
            gv << "    node_" << this << " -> node_" << stmt.get() << ";\n";
            stmt->graph_dump(gv, (Node *)this);
        }
    }

    gv << "\n}\n";
}

void Block_stmt::graph_dump(std::ostream &gv, Node *parent) const {
    std::size_t size = stmts_.size();

    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=lightgoldenrod1"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Block"
       << " | addr: " << this << " | parent: " << parent << "| { ";
    for (std::size_t i = 0; i < size; ++i) {
        gv << "stmt_" << i << ": " << stmts_[i].get();
        if (i + 1 < size) {
            gv << " | ";
        }
    }
    gv << " } }\"" << "];\n";

    for (const auto &stmt : stmts_) {
        if (stmt) {
            gv << "    node_" << this << " -> node_" << stmt.get() << ";\n";
            stmt->graph_dump(gv, (Node *)this);
        }
    }
}

void Assignment_stmt::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=plum"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Assignment"
       << " | addr: " << this << " | parent: " << parent << "| { left: " << variable_.get()
       << " | right: " << value_.get() << " } }\"" << "];\n";

    if (variable_) {
        gv << "    node_" << this << " -> node_" << variable_.get() << ";\n";
        variable_->graph_dump(gv, (Node *)this);
    }
    if (value_) {
        gv << "    node_" << this << " -> node_" << value_.get() << ";\n";
        value_->graph_dump(gv, (Node *)this);
    }
}

void Assignment_expr::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=plum"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Assignment expr"
       << " | addr: " << this << " | parent: " << parent << "| { left: " << variable_.get()
       << " | right: " << value_.get() << " } }\"" << "];\n";

    if (variable_) {
        gv << "    node_" << this << " -> node_" << variable_.get() << ";\n";
        variable_->graph_dump(gv, (Node *)this);
    }
    if (value_) {
        gv << "    node_" << this << " -> node_" << value_.get() << ";\n";
        value_->graph_dump(gv, (Node *)this);
    }
}

void While_stmt::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=turquoise"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ While"
       << " | addr: " << this << " | parent: " << parent << "| { left: " << condition_.get()
       << " | right: " << body_.get() << " } }\"" << "];\n";

    if (condition_) {
        gv << "    node_" << this << " -> node_" << condition_.get() << ";\n";
        condition_->graph_dump(gv, (Node *)this);
    }
    if (body_) {
        gv << "    node_" << this << " -> node_" << body_.get() << ";\n";
        body_->graph_dump(gv, (Node *)this);
    }
}

void If_stmt::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=turquoise"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ If"
       << " | addr: " << this << " | parent: " << parent << "| { cond: " << condition_.get()
       << " | then: " << then_branch_.get() << " | else: " << else_branch_.get() << " } }\""
       << "];\n";

    if (condition_) {
        gv << "    node_" << this << " -> node_" << condition_.get() << ";\n";
        condition_->graph_dump(gv, (Node *)this);
    }
    if (then_branch_) {
        gv << "    node_" << this << " -> node_" << then_branch_.get() << ";\n";
        then_branch_->graph_dump(gv, (Node *)this);
    }
    if (else_branch_) {
        gv << "    node_" << this << " -> node_" << else_branch_.get() << ";\n";
        else_branch_->graph_dump(gv, (Node *)this);
    }
}

void Input_stmt::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=lavenderblush1"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Input"
       << " | addr: " << this << " | parent: " << parent
       << "| variable: " << variable_.get()->get_name() << "}\"" << "];\n";

    if (variable_) {
        gv << "    node_" << this << " -> node_" << variable_.get() << ";\n";
        variable_->graph_dump(gv, (Node *)this);
    }
}

void Binary_operator::graph_dump(std::ostream &gv, Node *parent) const {
    const char *op_str = "";
    switch (op_) {
    case Binary_operators::Eq:
        op_str = "==";
        break;
    case Binary_operators::Neq:
        op_str = "!=";
        break;
    case Binary_operators::Less:
        op_str = "\\<";
        break;
    case Binary_operators::LessEq:
        op_str = "\\<=";
        break;
    case Binary_operators::Greater:
        op_str = "\\>";
        break;
    case Binary_operators::GreaterEq:
        op_str = "\\>=";
        break;
    case Binary_operators::Add:
        op_str = "+";
        break;
    case Binary_operators::Sub:
        op_str = "-";
        break;
    case Binary_operators::Mul:
        op_str = "*";
        break;
    case Binary_operators::Div:
        op_str = "/";
        break;
    case Binary_operators::RemDiv:
        op_str = "%";
        break;
    }

    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=lightsteelblue1"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Binary operator"
       << " | addr: " << this << " | parent: " << parent << " | operator: " << op_str
       << " | { left: " << left_.get() << " | right: " << right_.get() << " } }\"" << "];\n";

    if (left_) {
        gv << "    node_" << this << " -> node_" << left_.get() << ";\n";
        left_->graph_dump(gv, (Node *)this);
    }
    if (right_) {
        gv << "    node_" << this << " -> node_" << right_.get() << ";\n";
        right_->graph_dump(gv, (Node *)this);
    }
}

void Unary_operator::graph_dump(std::ostream &gv, Node *parent) const {
    const char *op_str = "";
    switch (op_) {
    case Unary_operators::Neg:
        op_str = "-";
        break;
    case Unary_operators::Plus:
        op_str = "+";
        break;
    case Unary_operators::Not:
        op_str = "!";
        break;
    }

    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=lightsteelblue1"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Unary operator"
       << " | addr: " << this << " | parent: " << parent << " | operator: " << op_str
       << "| operand: " << operand_.get() << " }\"" << "];\n";

    if (operand_) {
        gv << "    node_" << this << " -> node_" << operand_.get() << ";\n";
        operand_->graph_dump(gv, (Node *)this);
    }
}

void Number::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=palegreen"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Number"
       << " | addr: " << this << " | parent: " << parent << " | value: " << number_ << " }\""
       << "];\n";
}

void Variable::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=cornflowerblue"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Variable"
       << " | addr: " << this << " | parent: " << parent << " | name: " << var_name_ << " }\""
       << "];\n";
}

void Print_stmt::graph_dump(std::ostream &gv, Node *parent) const {
    gv << "    node_" << this << "[shape=Mrecord; style=filled; fillcolor=darkorange"
       << "; color=\"#000000\"; " << "fontcolor=\"#000000\"; " << "label=\"{ Print"
       << " | addr: " << this << " | parent: " << parent << " | value: " << value_.get() << "}\""
       << "];\n";

    if (value_) {
        gv << "    node_" << this << " -> node_" << value_.get() << ";\n";
        value_->graph_dump(gv, (Node *)this);
    }
}

} // namespace language
