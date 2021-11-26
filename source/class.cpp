#include "class.h"
#include "functions.h"

void form::Formula::addLeaf(Node*& node, const std::string& batch) {
  Node* leaf = new Node();
  leaf->value = batch;
  setType(leaf);
  if (leaf->type == Node::variable) {
    variables.insert(leaf->value);
  }
  node->nodes.push_back(leaf);
}

void form::Formula::addNode(Node*& node, bool isUnary, const std::string& value) {
  /*
    If node is variable, it will check for the syntax, insert in the variables set, 
    and just push to the node as a child. Else, it will recursively parse the 
    subformula, and then push the root node of this subformula.
  */

  if (isUnary) {
    addLeaf(node, value);
  } else {
    node->nodes.push_back(parseFormula(value));
  }
}

void form::Formula::parseFunctionalArguments(Node*& node, const std::string& arguments) {
  std::string value;
  int balance = 0;
  bool isLeaf = true;
  for (auto c : arguments) {
    if (balance != 0) {
      if (c == ')') {
        balance--;
      }
      value.push_back(c);
    } else if (c != ',' && c != ' ') {
      if (c == '(') {
        balance++;
        isLeaf = false;
      }
      value.push_back(c);
    } else if (!value.empty()) {
      addNode(node, true, value);
      value = "";
      isLeaf = true;
    }
  }
  addNode(node, isLeaf, value);
}

void form::Formula::push(Node* &node, bool isLeaf, bool &isFunctionalSymbol, const std::string& batch,
                         const std::string& functionAlias) {
  /*
    Pushing node. If operation of the node is "not", it will create additional node. 
    Else, it would just push node as new child.
  */

  if (isFunctionalSymbol) {
    Node* t = new Node();
    setOperator(t, functionAlias);
    if (functionalSymbols.count(functionAlias)) {
      parseFunctionalArguments(t, batch);
      node->nodes.push_back(t);
      return;
    }
    addNode(t, isLeaf, batch);
    node->nodes.push_back(t);
    isFunctionalSymbol = false;
  } else {
    addNode(node, isLeaf, batch);
  }
}

void form::Formula::setOperator(Node* &node, const std::string& alias) {
  /*
    Setting operator to the node.
  */
  if (logicalOperationsFromUnicode.count(alias)) {
    node->value = logicalOperationsFromUnicode[alias];
    node->type = Node::logical;
    return;
  }

  if (logicalOperationsToUnicode.count(alias)) {
    node->value = alias;
    node->type = Node::logical;
    return;
  }

  if (functionalSymbols.count(alias)) {
    node->value = alias;
    node->type = Node::functional;
    return;
  }
}

form::Formula::Node* form::Formula::parseFormula(const std::string& formulaStr) {
  /*
    The main process of the parsing.
  */

  Node* node = new Node();
  bool isLeaf = true, isOperation = false, isFunctionalSymbol = false;

  std::string batch = "";
  std::string functionAlias = "";
  int balance = 0;

  for (auto x : formulaStr) {
    batch.push_back(x);
    if ((prefixOperations.count(batch) || predicateOperations.count(batch) || functionalSymbols.count(batch))
          && balance == 0) {
      functionAlias = batch;
      batch = "";
      isFunctionalSymbol = true;
    } else if (x == '(') {
      isLeaf = false;
      if (balance == 0)
        batch.pop_back();
      balance++;
    } else if (x == ')') {
      isLeaf = false;
      if (balance == 1)
        batch.pop_back();
      balance--;
    } else if (x == ' ' and balance == 0) {
      batch.pop_back();

      if (isOperation) {
        setOperator(node, batch);
        isOperation = false;
        isLeaf = true;
      } else {
        push(node, isLeaf, isFunctionalSymbol, batch, functionAlias);
        isOperation = true;
      }
      batch = "";
    }
  }

  
  push(node, isLeaf, isFunctionalSymbol, batch, functionAlias);

  if (node->nodes.size() == 1) {
    /*
      Brackets compression
    */

    node = node->nodes.back();
  }
      
  return node;
}

int form::Formula::eval(Node* node, util::Evaluation &evaluation) {
  /*
    Getting value of the function on the evaluation.
  */

  if (node->type == Node::variable) {
    return evaluation.get(node->value);
  }

  if (node->type == Node::constant) {
    return getConstant(node->value);
  }
  std::vector<int> values;
  
  for (auto& x : node->nodes) {
    values.push_back(eval(x, evaluation));
  }

  return applyOperator(node->value, values, node->type);
}

int form::Formula::eval(Node* node, int (*f)(std::string)) {
  /*
    Getting value of the function on the set of variables, given by function f.
  */

  if (node->type == Node::variable) {
    return f(node->value);
  }

  if (node->type == Node::constant) {
    return getConstant(node->value);
  }

  std::vector<int> values;
    
  for (auto& x : node->nodes) {
    values.push_back(eval(x, f));
  }

  return applyOperator(node->value, values, node->type);
}

bool form::Formula::sat(Node* node) {
  /*
    Exponential (2^N) check for SAT of the formula. Default value for node is root.
  */
  util::Evaluation evaluation;
  std::vector<bool> variableValues;
  for (int variableSet = 0; variableSet < (1 << variables.size()); variableSet++) {
    int variableFromInt = variableSet;
    for (int i = 0; i < variables.size(); i++) {
      variableValues.push_back(variableFromInt & 1);
      variableFromInt>>=1;
    }
    for (auto& x : variables) {
      evaluation.push(x, variableValues.back());
      variableValues.pop_back();
    }
    if (eval(evaluation))
      return true;
  }
  return false;
}

bool form::Formula::taut(Node* node) {
  /*
    Exponential (2^N) check for tautology of the formula. Default value for node is root.
  */

  util::Evaluation evaluation;
  std::vector<bool> variableValues;
  for (int variableSet = 0; variableSet < (1 << variables.size()); variableSet++) {
    int variableFromInt = variableSet;
    for (int i = 0; i < variables.size(); i++) {
      variableValues.push_back(variableFromInt & 1);
      variableFromInt >>= 1;
    }
    for (auto x : variables) {
      evaluation.push(x, variableValues.back());
      variableValues.pop_back();
    }
    if (!eval(evaluation))
      return false;
  }
  return true;
}

void form::Formula::buildTruthTable(Node* node) {
  
  util::Evaluation evaluation;
  std::vector<bool> variableValues;
  std::vector<std::string> variableAliases;
  for (auto x : variables) {
    variableAliases.push_back(x);
  }
  while (!variableAliases.empty()) {
    std::cout << "| " << variableAliases.back() << " ";
    variableAliases.pop_back();
  }
  std::cout << "|f()|\n";
  for (int variableSet = 0; variableSet < (1 << variables.size()); variableSet++) {
    int variableFromInt = variableSet;
    for (int i = 0; i < variables.size(); i++) {
      variableValues.push_back(variableFromInt & 1);
      variableFromInt>>=1;
    }
    for (auto x : variableValues) {
      std::cout << "| " << x << " ";
    }
    for (auto x : variables) {
      evaluation.push(x, variableValues.back());
      variableValues.pop_back();
    }
    std::cout << "| " << eval(evaluation) << " |\n";
  }
}

std::string form::Formula::show(Node* node) {
  /*
    Recursively generates std::string representation of the node.
  */

  if (node->value == "not") {
    return "¬" + show(node->nodes.back());
  }

  if (functionalSymbols.count(node->value) || predicateOperations.count(node->value)) {
    std::string res = node->value + "(";
    for (auto x : node->nodes) {
      res += show(x) + ", ";
    }
    res.pop_back();
    res.back() = ')';
    return res;
  }

  if (node->type == Node::constant || node->type == Node::variable) {
    return node->value;
  }

  std::string res;
    
  for (auto x : node->nodes) {
    res += show(x);
    if (x != node->nodes.back()) {
      res += " " + logicalOperationsToUnicode[node->value] + " ";
    }
  }
    
  if (node != root) {
    res = "(" + res + ")";
  }

  return res;
}

void form::Formula::read(std::string s) {
  variables.clear();
  form::Formula::root = parseFormula(s);
}

std::string form::Formula::show() {
  return show(form::Formula::root);
}

int form::Formula::eval(util::Evaluation &v) {
  return eval(form::Formula::root, v);
}

bool form::Formula::sat(bool calc_time) {
  auto start = std::chrono::steady_clock::now();
  bool res = sat(form::Formula::root);
  if (!calc_time)
    return res;
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
  return res;
}

bool form::Formula::taut(bool calc_time) {
  auto start = std::chrono::steady_clock::now();
  bool res = taut(form::Formula::root);
  if (!calc_time)
    return res;
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
  return res;
}

void form::Formula::build_truth_table(bool calc_time) {
  auto start = std::chrono::steady_clock::now();
  buildTruthTable(form::Formula::root);
  if (!calc_time)
    return;
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
}

form::Formula::Node* form::Formula::get_root() {
  return form::Formula::root;
}

form::Formula::Formula() {
  // Setting up some standard logicalOperations

  logicalOperations["and"] = form::_conjunction;
  logicalOperations["or"] = form::_disjunction;
  logicalOperations["implies"] = form::_implication;
  logicalOperations["xor"] = form::_exclusive_disjunction;
  logicalOperations["not"] = form::_negation;
  logicalOperations["¬"] = form::_negation;
  prefixOperations = {"not", "¬"};
  logicalOperationsFromUnicode = {{"∧", "and"}, {"∨", "or"}, {"→", "implies"},
                                  {"¬", "not"}, {"⊕", "xor"}};
  logicalOperationsToUnicode = {{"and", "∧"}, {"or", "∨"}, {"implies", "→"},
                                {"not", "¬"}, {"xor", "⊕"}};

}

void form::Formula::addFunction(int (*f) (const std::vector<int>&), std::string alias) {
  functionalSymbols[alias] = f;
}

void form::Formula::to_cnf(Node*& node) {
  simplify(node);
  return;
  if (node->value == "and") {
    for (auto& disjunct : node->nodes) {
      to_disjunct(disjunct);
    }
  }
}

void form::Formula::negate(Node*& node) {
  if (node->value == "not") {
    Node* to_delete = node;
    node = node->nodes.back();
    delete to_delete;
  } else {
    Node* negation = new Node();
    negation->value = "not";
    negation->nodes.push_back(node);
    node = negation;
  }
}

void form::Formula::negate() {
  negate(root);
}

void form::Formula::to_cnf() {
  to_cnf(root);
}

void form::Formula::remove_implication(Node*& node) {
  if (node->value != "implies") {
    return;
  }
  node->value = "or";
  negate(node->nodes[0]);
}

void form::Formula::distribute(Node*& node) {

  std::string outerOperator = node->value;
  std::string innerOperator;
  if (outerOperator == "or") {
    innerOperator = "and";
  } else if (outerOperator == "and") {
    innerOperator = "or";
  } else {
    return;
  }

  std::vector<Node*> newNodes;
  std::vector<Node*> set;
  for (auto &x : node->nodes) {
    if (x->value == innerOperator)
      partiteNode(newNodes, set, node, 0);
  }

  //delete node->nodes.
  node->nodes.clear();
  for (auto &x : newNodes) {
    node->nodes.push_back(x);
  }

  // (a or (a and (b or c)) or d = a or (a and b) or (a and c) or d
}

void form::Formula::partiteNode(std::vector<Node*>& newNodes, std::vector<Node*>& set, Node*& node, int i) {
  // change set and push it to newNodes.
  if (i == node->nodes.size()) {
    Node* group = new Node();
    group->type = node->type;
    group->value = node->value;
    for (auto &x : set) {
      group->nodes.push_back(x);
    }

    newNodes.push_back(group);
    return;
  } else {
    // check if nodes[i] value = outer_operation
    for (auto &x : node->nodes[i]->nodes) {
      set.push_back(x);
      partiteNode(newNodes, set, node, i + 1);
      set.pop_back();
    }
  }
}

void form::Formula::deMorgan(Node*& node) {
  /*
    Literally applies de Morgan rule to the node.
   */
  if (node->value == "not") {
    std::string inner_value = node->nodes.back()->value;
    if (inner_value == "or" || inner_value == "and") {
      negate(node);
      node->value = (node->value == "or") ? "and" : "or";
      for (auto& child : node->nodes) {
        negate(child);
      }
    }
  } else if (node->value == "or" || node->value == "and") {
    node->value = (node->value == "or") ? "and" : "or";
    for (auto& child : node->nodes) {
      negate(child);
    }
    negate(node);
  }
}

void form::Formula::simplify(Node* node) {
  if (node->type == Node::variable || node->type == Node::constant) {
    return;
  }
  
  if (node->value == "not") {
    if (node->nodes.back()->type == Node::variable || node->nodes.back()->type == Node::constant) {
      return;
    }
    Node* to_delete = node;
    Node* de_morgan = node->nodes.back();
    if (de_morgan->value == "implies") {
      remove_implication(de_morgan);
    }
    if (de_morgan->value == "or") {
      de_morgan->value = "and";
    } else {
      de_morgan->value = "or";
    }
    for (auto &v : de_morgan->nodes) {
      simplify(v);
      negate(v);
    }
    node = de_morgan;
    delete to_delete;
    return;
  }

  if (node->value == "implies") {
    simplify(node->nodes[0]);
    simplify(node->nodes[1]);
    remove_implication(node);
    simplify(node);
    return;
  }

  std::vector<Node*> nodes;
  for (auto& v : node->nodes) {
    simplify(v);
    if (v->value == node->value) {
      for (auto &vn : v->nodes) {
        nodes.push_back(vn);
      }
      delete v;
    } else {
      nodes.push_back(v);
    }
  }
  node->nodes = nodes;
  for (auto &v : node->nodes) {
    //
  }
  return;
}

void form::Formula::to_disjunct(Node*& node) {
  if (node->type == Node::variable || node->type == Node::constant) {
    return;
  }
  if (node->value == "implies") {
    remove_implication(node);
  }
}

void form::Formula::setType(Node*& node) const {
  if (isConstant(node->value)) {
    node->type = Node::constant;
  } else {
    node->type = Node::variable;
  }

  bool haveDigit = false;
  bool haveLower = false;
  for (auto& sym : node->value) {
    if (std::islower(sym)) {
      assert(!haveDigit);
      haveLower = true;
      continue;
    }
    if (std::isdigit(sym)) {
      haveDigit = true;
      continue;
    }
    throw std::runtime_error("Unknown symbol in variable/constant name.");
  }
  if (!haveLower) {
    node->type = Node::constant;
  }
}

bool form::Formula::isConstant(const std::string& value) const {
  if (constants.count(value)) {
    return true;
  }
  return false;
}

int form::Formula::getConstant(const std::string& value) {
  if (!constants.count(value)) {
    constants[value] = stoi(value);
  }
  return constants[value];
}

int form::Formula::applyOperator(const std::string &operatorAlias, const std::vector<int> &values, Node::typeE type) {
  if (type == Node::logical) {
    return logicalOperations[operatorAlias](values);
  } else if (type == Node::predicate) {
    return predicateOperations[operatorAlias](values);
  } else {
    return functionalSymbols[operatorAlias](values);
  }
}