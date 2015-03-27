#ifndef GATE_H
#define GATE_H
#include <iostream>

class Gate {
 public:
  Gate(Gate* A): input_1(A) {}
  Gate(Gate* A, Gate* B): input_1(A), input_2(B) {}
  Gate(bool B) : b(B){}
  /* virtual bool evaluate() = 0; */
  virtual bool evaluate(){return false;};

 protected:
  bool b;
  Gate* input_1 = nullptr;
  Gate* input_2;
};

class Not : public Gate {
 protected:
 public:
  Not();
  Not(Gate* A) : Gate(A) {}

  bool evaluate() {
    return !(input_1->evaluate());
  }
};

class Wire : public Gate {
 protected:
  bool is_root;

 public:
  Wire();
  Wire(bool B) : Gate(B){}
  Wire(Gate* A) : Gate(A){}

  bool evaluate() {
      if(input_1 == nullptr){
          return b;
      }
      else {
          return input_1->evaluate();
      }
  }
  void setInput(bool new_input){
      if(input_1 == nullptr){
          b = new_input;
      }
      else{
          std::cerr << "Wire isn't an input wire\n";
      }
  }
};

class And : public Gate {
    public:
    And();
    And(Gate* A, Gate* B) : Gate(A, B) {}
    bool evaluate() {
        return input_1->evaluate() && input_2->evaluate();
    }
};

class Or : public Gate {
    protected:
    public:
        Or();
        Or(Gate* A, Gate* B) : Gate(A, B) {}
        bool evaluate() {
            return input_1->evaluate() || input_2->evaluate();
        }
};

#endif /* GATE_H */