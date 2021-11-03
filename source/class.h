#pragma once
#include "utils.h"

namespace form {
  class Formula {
    
    public:
      std::map<std::string, int (*)(std::vector<int>)> functions;
      std::map<std::string, std::string> unicode_to_function;
      std::map<std::string, std::string> function_to_unicode;

      std::set<std::string> variables;
      std::set<std::string> prefix_functions;
      std::set<std::string> special_functions;

      struct Node {
        bool leaf = false;
        std::string value = "";
        std::vector<Node*> nodes;
      };
      Node* root;

      void process_function_arguments(Node* &node, std::string batch);

      void push_node(Node* &node, bool is_leaf, std::string batch);

      void push(Node* &node, bool is_leaf, bool &is_not, std::string batch, std::string function_alias);

      void set_operator(Node* &node, std::string s);

      Node* create_node(std::string s);

      bool process(std::vector<int> values, std::string operation);

      int eval(Node* node, util::Evaluation &var_eval);

      int eval(Node* node, bool (*f)(std::string));

      bool sat(Node* node);

      bool taut(Node* node);

      void build_truth_table(Node* node);

      std::string show(Node* node);

      using processFunction = bool (Formula::*)();

    public:
      void read(std::string s);

      std::string show();

      int eval(util::Evaluation &v);

      bool sat(bool calc_time = false);

      bool taut(bool calc_time = false);

      void build_truth_table(bool calc_time = false);

      void add_function(int (*f) (std::vector<int>), std::string alias);

      Node* get_root();
      
      Formula();
  };
}