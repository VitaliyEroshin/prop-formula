#include "class.h"

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

void form::Formula::push(Node* &node, bool is_leaf, bool &is_not, std::string batch) {
  /*
    Pushing node. If operation of the node is "not", it will create additional node. 
    Else, it would just push node as new child.
  */
  if (is_not) {
    Node* t = new Node();
    t->value = "not";
    push_node(t, is_leaf, batch);
    node->nodes.push_back(t);
    is_not = false;
  } else {
    push_node(node, is_leaf, batch);
  }
}

void form::Formula::set_operator(Node* &node, std::string s) {
  /*
    Setting operator to the node.
  */
  node->value = s;
  if (util::in_get_operator(s)) {
    node->value = util::get_operator(s);
  }
}

form::Formula::Node* form::Formula::create_node(std::string s) {
  /*
    The main process of the parsing.
  */

  Node* node = new Node();
  bool is_leaf = true, is_op = false, is_not = false;

  std::string batch = "";
  int balance = 0;

  for (auto x : s) {
    batch.push_back(x);
      
    if ((batch == "not" or batch == "¬") and balance == 0) {
      batch = "";
      is_not = true;
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
        push(node, is_leaf, is_not, batch);
        is_op = true;
      }
      batch = "";
    }
  }

  
  push(node, is_leaf, is_not, batch);
  int node_size = node->nodes.size();

  if (node_size == 1) {
    node = node->nodes[0];
  }
      
  return node;
}

bool form::Formula::process(std::vector<bool> values, std::string operation) {
  /*
    Returns value of the node with processed operation.
  */

  if (operation == "not")
    return !values.back();

  else if (operation == "()")
    return values.back();

  else if (operation == "implies") 
    return !values[0] || values[1];

  bool r = values.back();
  values.pop_back();
  for (auto x : values) {
    if (operation == "or") {
      r |= x;
    } else if (operation == "and") {
      r &= x;
    } else if (operation == "xor") {
      r ^= x;
    }
  }

  if (operation != "or" and operation != "and" and operation != "xor")
    throw std::invalid_argument("Unknown logic operation");

  return r;
}

bool form::Formula::eval(Node* node, util::Evaluation &var_eval) {
  /*
    Getting value of the function on the evaluation.
  */

  if (node->leaf)
    return var_eval.get(node->value);

  std::vector<bool> nodes_value;
    
  for (auto x : node->nodes) 
    nodes_value.push_back(eval(x, var_eval));

  return form::Formula::process(nodes_value, node->value);
}

bool form::Formula::eval(Node* node, bool (*f)(std::string)) {
  /*
    Getting value of the function on the set of variables, given by function f.
  */

  if (node->leaf)
    return f(node->value);

  std::vector<bool> nodes_value;
    
  for (auto x : node->nodes) 
    nodes_value.push_back(eval(x, f));

  return form::Formula::process(nodes_value, node->value);
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
      res += " " + util::get_symbol(node->value) + " ";   
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

bool form::Formula::eval(util::Evaluation &v) {
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