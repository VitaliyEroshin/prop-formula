#include <string>
#include <vector>
#include <map>
#include <set>

namespace prop {

struct varEval {
  std::map<std::string, bool> rule;

  void push(std::string var_name, bool var_value) {
    rule[var_name] = var_value;
  }

  bool get(std::string var_name) {
    if (rule.count(var_name) == 0)
      throw std::invalid_argument("Variable is not defined.");
    return rule[var_name];
  }
};

class Formula {
  std::set<std::string> variables;

  std::map<std::string, std::string> getSymbol = 
    {{"and", "∧"}, {"or", "∨"}, {"implies", "⇒"}, {"implies", "→"}, {"not", "¬"}};

  std::map<std::string, std::string> getOperator =
    {{"∧", "and"}, {"∨", "or"}, {"⇒", "implies"}, {"→", "implies"}, {"¬", "not"}};

  std::set<std::string> operationSymbols = {"∧", "∨", "⇒", "→"}; // not is special case.

  struct Leaf {
    std::string value;
  };

  struct Node {
    bool leaf_first = false;
    std::string op = "";
    std::vector<Node*> nodes;
    std::vector<Leaf*> leaves;
  };

  void correctVariable(std::string var) {
    bool p_digit = false;
    for (auto sym : var) {
      bool digit;
      if (sym >= 'a' and sym <= 'z')
        digit = false;
      else if (sym >= '0' and sym <= '9')
        digit = true;
      else
        throw std::invalid_argument("Incorrect variable: invalid symbol in variable name");

      if (p_digit and !digit) 
        throw std::invalid_argument("Incorrect variable: letter symbol after digits");
      
      p_digit = digit;
    }
  }

  Leaf* createLeaf(std::string x) {
    correctVariable(x);
    Leaf* l = new Leaf();
    variables.insert(x);
    l->value = x;
    return l;
  }

  bool isLeaf(std::string s) {
    for (auto x : s) {
      if (x == ' ' or x == '(' or x == ')')
        return false;
    }
    return true;
  }

  void pushLeafNode(Node* &node, bool is_leaf, std::string batch) {
    if (is_leaf)
      node->leaves.push_back(createLeaf(batch));
    else {
      node->nodes.push_back(createNode(batch));
      if (node->nodes.back()->op == "()") {
        node->leaves.push_back(node->nodes.back()->leaves.back());
        node->nodes.pop_back();
      }
    }
  }

  void push(Node* &node, bool is_leaf, bool &is_not, std::string batch) {
    if (is_not) {
      Node* subnode = new Node();
      subnode->op = "not";
      pushLeafNode(subnode, is_leaf, batch);
      node->nodes.push_back(subnode);
      is_not = false;
    }
    else
      pushLeafNode(node, is_leaf, batch);
  }

  void setOperator(Node* &node, std::string s) {
    node->op = s;
    if (getOperator.count(s))
      node->op = getOperator[s];

    if (node->op == "implies" && !node->leaves.empty()) {
      node->leaf_first = true;
    }
  }

  Node* createNode(std::string s) {
    Node* node = new Node();

    bool is_leaf = true, is_op = false, is_not = false;

    std::string batch = "";
    int balance = 0;
    for (auto x : s) {
      batch.push_back(x);
      
      if ((batch == "not" or batch == "¬") and balance == 0) {
        batch = "";
        is_not = true;
      }

      /*
      else if (operationSymbols.count("0")) {

        std::string t_op = "";
        t_op.push_back(batch.back());
        setOperator(node, t_op);
        batch.pop_back();
        push(node, is_leaf, is_not, batch);
        is_leaf = true;
        is_op = false;
      }
      */
     
      else if (x == '(') {
        is_leaf = false;
        if (balance == 0)
          batch.pop_back();
        balance++;
      }
      else if (x == ')') {
        is_leaf = false;
        if (balance == 1)
          batch.pop_back();
        balance--;
      }
      else if (x == ' ' and balance == 0) {
        batch.pop_back();

        if (is_op) {
          setOperator(node, batch);
          is_op = false;
          is_leaf = true;
        } 
        else {
          push(node, is_leaf, is_not, batch);
          is_op = true;
        }
        
        batch = "";
      }
    }
      
    push(node, is_leaf, is_not, batch);
    int node_size = node->leaves.size() + node->nodes.size();

    if (node_size == 1) {
      node->op = "()";
      if (node->nodes.size()) {
        node = node->nodes[0];
      }
    }
    
    if (node_size > 2)
      throw std::invalid_argument("Too many variables");
      
    return node;
  }

  Node* head;

  bool eval(Node* node, varEval &var_eval) {
    std::vector<bool> nodes_value;
    
    if (node->leaf_first)
      for (auto x : node->leaves) 
        nodes_value.push_back(var_eval.get(x->value));
    
    for (auto x : node->nodes) 
      nodes_value.push_back(eval(x, var_eval));
    
    if (!node->leaf_first)
      for (auto x : node->leaves) 
        nodes_value.push_back(var_eval.get(x->value));
    

    if (node->op == "not") 
      return !nodes_value.back();
    else if (node->op == "()")
      return nodes_value.back();
    else if (node->op == "or")
      return (nodes_value[0] || nodes_value[1]);
    else if (node->op == "and")
      return (nodes_value[0] && nodes_value[1]);
    else if (node->op == "implies") 
      return (!nodes_value[0] || nodes_value[1]);
    
    throw std::invalid_argument("Unknown logic operation");
    return false;
  }

  bool eval(Node* node, bool (*f)(std::string)) {
    std::vector<bool> nodes_value;
    
    if (node->leaf_first)
      for (auto x : node->leaves) 
        nodes_value.push_back(f(x->value));
    
    for (auto x : node->nodes) 
      nodes_value.push_back(eval(x, f));
    
    if (!node->leaf_first)
      for (auto x : node->leaves) 
        nodes_value.push_back(f(x->value));
    

    if (node->op == "not") 
      return !nodes_value.back();
    else if (node->op == "()")
      return nodes_value.back();
    else if (node->op == "or")
      return (nodes_value[0] || nodes_value[1]);
    else if (node->op == "and")
      return (nodes_value[0] && nodes_value[1]);
    else if (node->op == "implies") 
      return (!nodes_value[0] || nodes_value[1]);
    
    throw std::invalid_argument("Unknown logic operation");
    return false;
  }

  bool sat(Node* node) {
    // bruteforce algorithm.

    prop::varEval v;
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

  bool taut(Node* node) {
    // bruteforce algorithm.

    prop::varEval v;
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

  std::string show(Node* x) {
    if (x->op == "not" and x->nodes.size())
      return "¬(" + show(x->nodes.back()) + ")";
    
    if (x->op == "not" and x->leaves.size())
      return "¬" + x->leaves.back()->value;

    if (x->nodes.size() == 2 and x->nodes[0]->op == "not" and x->nodes[1]->op == "not")
      return show(x->nodes[0]) + " " + getSymbol[x->op] + " " + show(x->nodes[1]);

    if (x->nodes.size() == 2 and x->nodes[0]->op == "not")
     return show(x->nodes[0]) + " " + getSymbol[x->op] + " (" + show(x->nodes[1]) + ")";

    if (x->nodes.size() == 2 and x->nodes[1]->op == "not")
    return "(" + show(x->nodes[0]) + ") " + getSymbol[x->op] + " " + show(x->nodes[1]);

    if (x->nodes.size() == 2)
      return "(" + show(x->nodes[0]) + ") " +  getSymbol[x->op] + " (" + show(x->nodes[1]) + ")";

    if (x->leaves.size() == 2)
      return x->leaves[0]->value + " " + getSymbol[x->op] + " " + x->leaves[1]->value;

    if (x->nodes.size() and x->nodes[0]->op == "not") 
      return show(x->nodes[0]) + " " + getSymbol[x->op] + " " + x->leaves[0]->value;
    
    return "(" + show(x->nodes[0]) + ") " + getSymbol[x->op] + " " + x->leaves[0]->value;
  }

  public:
    void read(std::string s) {
      head = createNode(s);
    }
    
    std::string show() {
      return show(head);
    }

    bool eval(varEval &v) {
      
      return eval(head, v);
    }

    bool sat(bool calc_time = false) {
      auto start = std::chrono::steady_clock::now();
      bool res = sat(head);
      if (!calc_time)
        return res;
      auto end = std::chrono::steady_clock::now();
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";
      return res;
    }
    
    bool taut(bool calc_time = false) {
      auto start = std::chrono::steady_clock::now();
      bool res = taut(head);
      if (!calc_time)
        return res;
      auto end = std::chrono::steady_clock::now();
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";
      return res;
    }
};

}