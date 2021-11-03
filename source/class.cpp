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
  for (auto x : batch) {
    if (std::isdigit(x)) {
      value.push_back(x);
    } else if (!value.empty()) {
      Node* argument = new Node();
      argument->value = value;
      argument->leaf = true;
      node->nodes.push_back(argument);
      value = "";
    }
  }
  Node* argument = new Node();
  argument->value = value;
  argument->leaf = true;
  node->nodes.push_back(argument);
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

std::string form::Formula::show(Node* node) {
  /*
    Recursively genereates std::string representation of the node.
  */

  if (node->value == "not") {
    return "¬" + show(node->nodes.back());
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
  prefix_functions = {"not", "¬", "Ramsey", "Color"};
  special_functions = {"Ramsey", "Color"};
}