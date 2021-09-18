#include <string>
#include <vector>
#include <map>

class PropositionalFormula {

  std::map<std::string, std::string> getSymbol = 
    {{"and", "∧"}, {"or", "∨"}, {"implies", "⇒"}, {"not", "¬"}};

  std::map<std::string, std::string> getOperator =
    {{"∧", "and"}, {"∨", "or"}, {"⇒", "implies"}, {"¬", "not"}};

  struct Leaf {
    std::string value;
  };

  struct Node {
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
          node->op = batch;
          if (getOperator.count(batch))
            node->op = getOperator[batch];
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
    
    if (node->leaves.size() + node->nodes.size() == 1) {
      node->op = "()";
      if (node->nodes.size()) {
        node = node->nodes[0];
      }
    }
    
    return node;
  }

  Node* head;

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
};
