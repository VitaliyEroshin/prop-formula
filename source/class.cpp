#include "class.h"
#include "functions.h"

void form::Formula::push_node(Node* &node, bool is_leaf, std::string batch) {
  /*
    If node is variable, it will check for the syntax, insert in the variables set, 
    and just push to the node as a child. Else, it will recursively parse the 
    subformula, and then push the root node of this subformula.
  */
  if (is_leaf) {
    util::is_variable(batch);
    if (!std::isdigit(batch[0]))
      variables.insert(batch);

    Node* t = new Node();
    t->leaf = true;
    t->value = batch;
    node->nodes.push_back(t);
  } else  {
    node->nodes.push_back(create_node(batch));
  }
}

void form::Formula::process_function_arguments(Node* &node, std::string batch) {
  std::string value = "";
  int balance = 0;
  bool is_leaf = true;
  for (auto x : batch) {
    if (balance != 0) {
      if (x == ')') {
        balance--;
      }
      value.push_back(x);
    } else if (balance == 0 && x != ',' && x != ' ') {
      if (x == '(') {
        balance++;
        is_leaf = false;
      }
      value.push_back(x);
    } else if (balance == 0 && !value.empty()) {
      push_node(node, true, value);
      value = "";
      is_leaf = true;
    }
  }
  push_node(node, is_leaf, value);
}

void form::Formula::push(Node* &node, bool is_leaf, bool &is_function, std::string batch, std::string function_alias) {
  /*
    Pushing node. If operation of the node is "not", it will create additional node. 
    Else, it would just push node as new child.
  */
  if (is_function) {
    Node* t = new Node();
    set_operator(t, function_alias);
    if (special_functions.count(function_alias)) {
      process_function_arguments(t, batch);
      node->nodes.push_back(t);
      return;
    }
    push_node(t, is_leaf, batch);
    node->nodes.push_back(t);
    is_function = false;
  } else {
    push_node(node, is_leaf, batch);
  }
}

void form::Formula::set_operator(Node* &node, std::string s) {
  /*
    Setting operator to the node.
  */
  node->value = s;
  if (unicode_to_function.count(s)) {
    node->value = unicode_to_function[s];
  }
}

form::Formula::Node* form::Formula::create_node(std::string s) {
  /*
    The main process of the parsing.
  */

  Node* node = new Node();
  bool is_leaf = true, is_op = false, is_function = false;

  std::string batch = "";
  std::string function_alias = "";
  int balance = 0;

  for (auto x : s) {
    batch.push_back(x);
      
    if (prefix_functions.count(batch) and balance == 0) {
      function_alias = batch;
      batch = "";
      is_function = true;
    } else if (x == '(') {
      is_leaf = false;
      if (balance == 0)
        batch.pop_back();
      balance++;
    } else if (x == ')') {
      is_leaf = false;
      if (balance == 1)
        batch.pop_back();
      balance--;
    } else if (x == ' ' and balance == 0) {
      batch.pop_back();

      if (is_op) {
        set_operator(node, batch);
        is_op = false;
        is_leaf = true;
      } else {
        push(node, is_leaf, is_function, batch, function_alias);
        is_op = true;
      }
      batch = "";
    }
  }

  
  push(node, is_leaf, is_function, batch, function_alias);
  int node_size = node->nodes.size();

  if (node_size == 1) {
    node = node->nodes[0];
  }
      
  return node;
}

bool form::Formula::process(std::vector<int> values, std::string operation) {
  /*
    Returns value of the node with processed operation.
  */

  return functions[operation](values);
}

int form::Formula::eval(Node* node, util::Evaluation &var_eval) {
  /*
    Getting value of the function on the evaluation.
  */

  if (node->leaf)
    return var_eval.get(node->value);

  std::vector<int> nodes_value;
  
  for (auto x : node->nodes) 
    nodes_value.push_back(eval(x, var_eval));

  return functions[node->value](nodes_value);
}

int form::Formula::eval(Node* node, bool (*f)(std::string)) {
  /*
    Getting value of the function on the set of variables, given by function f.
  */

  if (node->leaf)
    return f(node->value);

  std::vector<int> nodes_value;
    
  for (auto x : node->nodes) 
    nodes_value.push_back(eval(x, f));

  return functions[node->value](nodes_value);
}

bool form::Formula::sat(Node* node) {
  /*
    Exponential (2^N) check for SAT of the formula. Default value for node is root.
  */
  util::Evaluation v;
  std::vector<bool> tmp;
  for (int ev = 0; ev < (1<<variables.size()); ev++) {
    int t = ev;
    for (int i = 0; i < variables.size(); i++) {
      tmp.push_back(t&1);
      t>>=1;
    }
    for (auto x : variables) {
      v.push(x, tmp.back());
      tmp.pop_back();
    }
    if (eval(v))
      return true;
  }
  return false;
}

bool form::Formula::taut(Node* node) {
  /*
    Exponential (2^N) check for tautology of the formula. Default value for node is root.
  */

  util::Evaluation v;
  std::vector<bool> tmp;
  for (int ev = 0; ev < (1<<variables.size()); ev++) {
    int t = ev;
    for (int i = 0; i < variables.size(); i++) {
      tmp.push_back(t&1);
      t>>=1;
    }
    for (auto x : variables) {
      v.push(x, tmp.back());
      tmp.pop_back();
    }
    if (!eval(v))
      return false;
  }
  return true;
}

void form::Formula::build_truth_table(Node* node) {
  
  util::Evaluation v;
  std::vector<bool> tmp;
  std::vector<std::string> variable_names;
  for (auto x : variables) {
    variable_names.push_back(x);
  }
  while (!variable_names.empty()) {
    std::cout << "| " << variable_names.back() << " ";
    variable_names.pop_back();
  }
  std::cout << "|f()|\n";
  for (int ev = 0; ev < (1<<variables.size()); ev++) {
    int t = ev;
    for (int i = 0; i < variables.size(); i++) {
      tmp.push_back(t&1);
      t>>=1;
    }
    for (auto x : tmp) {
      std::cout << "| " << x << " ";
    }
    for (auto x : variables) {
      v.push(x, tmp.back());
      tmp.pop_back();
    }
    std::cout << "| " << eval(v) << " |\n";
  }
}

std::string form::Formula::show(Node* node) {
  /*
    Recursively genereates std::string representation of the node.
  */

  if (node->value == "not") {
    return "¬" + show(node->nodes.back());
  }

  if (special_functions.count(node->value)) {
    std::string res = node->value + "(";
    for (auto x : node->nodes) {
      res += show(x) + ", ";
    }
    res.pop_back();
    res.pop_back();
    res += ")";
    return res;
  }

  if (node->leaf) {
    return node->value;
  }

  std::string res = "";
    
  for (auto x : node->nodes) {
    res += show(x);
    if (x != node->nodes.back())
      res += " " + function_to_unicode[node->value] + " ";   
  }
    
  if (node != root) {
    res = "(" + res + ")";
  }

  return res;
}

void form::Formula::read(std::string s) {
  variables.clear();
  form::Formula::root = create_node(s);
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
  build_truth_table(form::Formula::root);
  if (!calc_time)
    return;
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
}

form::Formula::Node* form::Formula::get_root() {
  return form::Formula::root;
}

form::Formula::Formula() {
  // Setting up some standard functions

  functions["and"] = form::__conjunction;
  functions["or"] = form::__disjunction;
  functions["not"] = form::__negation;
  functions["implies"] = form::__implication;
  functions["xor"] = form::__exclusive_disjunction;

  unicode_to_function = {{"∧", "and"}, {"∨", "or"}, {"→", "implies"}, {"¬", "not"}, {"⊕", "xor"}};
  function_to_unicode = {{"and", "∧"}, {"or", "∨"}, {"implies", "→"}, {"not", "¬"}, {"xor", "⊕"}};
  prefix_functions = {"not", "¬"};
  special_functions = {};
}

void form::Formula::add_function(int (*f) (std::vector<int>), std::string alias) {
  prefix_functions.insert(alias);
  special_functions.insert(alias);
  functions[alias] = f;
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
  node->value = "or";
  negate(node->nodes[0]);
}

void form::Formula::simplify(Node* node) {
  if (node->leaf) {
    return;
  }
  
  if (node->value == "not") {
    if (node->nodes[0]->leaf) {
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
  return;
}

void form::Formula::to_disjunct(Node*& node) {
  if (node->leaf) {
    return;
  }
  if (node->value == "implies") {
    remove_implication(node);
  }
}