#pragma once
#include "utils.h"

namespace form {
  class Formula {
    
    private:
      std::map<std::string, int (*)(std::vector<int>)> functions;
      std::set<std::string> variables;
      struct Node {
        bool leaf = false;
        std::string value = "";
        std::vector<Node*> nodes;
      };
      Node* root;

      void push_node(Node* &node, bool is_leaf, std::string batch);

      void push(Node* &node, bool is_leaf, bool &is_not, std::string batch);

      void set_operator(Node* &node, std::string s);

      Node* create_node(std::string s);

      bool process(std::vector<int> values, std::string operation);

      int eval(Node* node, util::Evaluation &var_eval);

      int eval(Node* node, bool (*f)(std::string));

      bool sat(Node* node);

      bool taut(Node* node);

      std::string show(Node* node);

      using processFunction = bool (Formula::*)();

    public:
      void read(std::string s);

      std::string show();

      int eval(util::Evaluation &v);

      bool sat(bool calc_time = false);

      bool taut(bool calc_time = false);

      Node* get_root();
      
      Formula();
  };
}