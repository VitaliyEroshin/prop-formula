#pragma once

#include "utils.h"

#include <iostream>
#include <vector>

namespace form {
  class Formula {
    struct Node {
      bool leaf;
      std::string value;
      std::vector<Node*> nodes;
    };
    std::set<std::string> variables;
    Node* root;

    void push_node();
    void push();
    void set_operator(Node* &node, std::string s);
    Node* create_node(std::string s);
    bool process(std::vector<bool> values, std::string operation);
    bool eval(Node* node, util::Evaluation &var_eval);
    bool eval(Node* node, bool (*f)(std::string));
    bool sat(Node* node);
    bool taut(Node* node);
    std::string show(Node* node);

    public:
      void read(std::string s);
      std::string show();
      bool eval(util::Evaluation &v);
      bool sat(bool calc_time = false);
      bool taut(bool calc_time = false);
  };
}