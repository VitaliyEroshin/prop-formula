#pragma once
#include "utils.h"

namespace form {
  class Formula {
    
    public:
      std::map<std::string, bool (*)(const std::vector<int>&)> logicalOperations;
      std::map<std::string, bool (*)(const std::vector<int>&)> predicateOperations;
      std::map<std::string, int (*)(const std::vector<int>&)> functionalSymbols;
      std::set<std::string> prefixOperations;
      std::map<std::string, int> constants;
      std::set<std::string> variables;

      std::map<std::string, std::string> logicalOperationsFromUnicode;
      std::map<std::string, std::string> logicalOperationsToUnicode;

      struct Node {
        //bool leaf = false;
        std::string value;
        std::vector<Node*> nodes;
        enum typeE {logical, predicate, functional, variable, constant};
        typeE type;
      };

      Node* root{};

      void parseFunctionalArguments(Node* &node, const std::string& arguments);

      void addNode(Node* &node, bool isUnary, const std::string& value);

      void push(Node* &node, bool isLeaf, bool &isFunctionalSymbol, const std::string& batch, const std::string& functionAlias);

      void setOperator(Node* &node, const std::string& alias);

      Node* parseFormula(const std::string& formulaStr);

      bool process(std::vector<int> values, std::string operation);

      int eval(Node* node, util::Evaluation &evaluation);

      int eval(Node* node, int (*f)(std::string));

      bool sat(Node* node);

      bool taut(Node* node);

      void buildTruthTable(Node* node);

      std::string show(Node* node);

      using processFunction = bool (Formula::*)();

      void to_cnf(Node*& node);
      
      void negate(Node*& node);

      void deMorgan(Node*& node);

      void remove_implication(Node*& node);

      void to_disjunct(Node*& node);

      void simplify(Node* node);

      void setType(Node*& node) const;

      bool isConstant(const std::string& value) const;

      void addLeaf(Node*& node, const std::string& batch);

      int getConstant(const std::string& value);

      int applyOperator(const std::string &operatorAlias, const std::vector<int> &values, Node::typeE type);

    public:
      void read(std::string s);

      std::string show();

      int eval(util::Evaluation &v);

      bool sat(bool calc_time = false);

      bool taut(bool calc_time = false);

      void build_truth_table(bool calc_time = false);

      void addFunction(int (*f) (const std::vector<int>&), std::string alias);

      void negate();

      void distribute(Node*& node);

      void partiteNode(std::vector<Node*>& newNodes, std::vector<Node*>& set, Node*& node, int i);

      void distribute() { distribute(root); };
      void de_morgan() { deMorgan(root); };

      void to_cnf();
      Node* get_root();
      
      Formula();
  };
}