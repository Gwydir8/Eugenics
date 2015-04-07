#include <vector>
#include <queue>
#include <string>
#include <map>
#include <functional>
#include "jvalgorithm.h"
#include "utility.h"

using namespace std;
size_t hash_output(vector<vector<bool>> output);

Ckt_Algo::Ckt_Algo(Circuit circuit,  vector<vector<bool>> e_o) : correct_circuit_output(), expected_output(e_o)  {
  ex_list.push(circuit);
  unique_map.insert(pair<size_t, int>(hash_output(expected_output), 0));
}
size_t hash_output(vector<vector<bool>> output){
  string s = "";
  for (vector<bool> row : output) {
    for (bool bit : row) {
      s += to_string((int)bit);
    }
  }
  hash<string> hash_fn;
  size_t hash = hash_fn(s);
  return hash;
}

void Ckt_Algo::searchPreProcess(){
  Circuit* circ = &ex_list.front();
  if (circ->getOutputCount() < 2){
    //only do this for over 3 outputs
        return;
  }
  for(int i = 0; i<2; ++i){
    for(int j = i+1; j < circ->getInputCount(); ++j){
      circ->addGate(AND, i, j);
      circ->addGate(OR, i, j);
    }
  }

}


bool Ckt_Algo::isUnique(Circuit candidate){
  bool need_more_possibilities = (candidate.getOutputCount() < 2);
  vector<vector<bool>> input_1 = candidate.evaluateAllInputs();


  vector<vector<bool>> input_2;
  input_2.push_back(input_1[1]);
  input_2.push_back(input_1[2]);
  input_2.push_back(input_1[0]);

  unique_map.insert(pair<size_t, int>(hash_output(input_2), candidate.getNotCount()));

  vector<vector<bool>> input_3;
  input_3.push_back(input_1[2]);
  input_3.push_back(input_1[0]);
  input_3.push_back(input_1[1]);

  unique_map.insert(pair<size_t, int>(hash_output(input_3), candidate.getNotCount()));




  size_t hashed_val = hash_output(input_1);
  auto search = unique_map.find(hashed_val);
  if(search == unique_map.end()) { //if(hashed_val is unique)
    unique_map.insert(pair<size_t, int>(hashed_val, candidate.getNotCount()));
    return true;
  }
  else if(search->second > candidate.getNotCount()){
    search->second = candidate.getNotCount();
    return true;
  }
  /* else if(need_more_possibilities){ */
  /*   if(ex_list.size() < 10){ */
  /*       return true; */
  /*   } */
  /* } */
  return false;
}

void Ckt_Algo::checkPermAndMap(Circuit* check, vector<vector<bool>> desired){
  return;
    vector<vector<bool>> actual = check->evaluateAllInputs();
    vector<vector<bool>> transposed_actual(actual.size());
    vector<vector<bool>> transposed_expected(actual.size());
    //transposes our vectors to work easily with columns later
      for (int i = 0; i < actual.size(); ++i) {
        for (int j = 0; j < actual.size(); ++j) {
          transposed_actual[i].push_back(actual[j][i]);
          transposed_expected[i].push_back(desired[j][i]);
        }
      }

      for (int i = 0; i < actual.size(); ++i) {
        for (int j = 0; j < actual.size(); ++j) {
          if(transposed_actual[i] == transposed_expected[j]){
            /* check->mapOutputToOutput(i, j); // */
            return;
          }
        }
      }


      /* bool is_working = true; */
      /* for (int i = 0; i < actual.size(); ++i) { */
      /*   for (int j = 0; j < actual.size(); ++j) { */
      /*     if(transposed_actual[i][j] != actual[j][i] || transposed_expected[i][j] != desired[j][i]){ */
      /*           is_working = false; */
      /*           exit(-1); */
      /*     } */
      /*   } */
      /* } */



      /* if(!is_equal_to_col){ */
      /*     is_equal_to_circuit = false; */
      /* } */
      /* else{ */
      /*   current_circ->mapGateToOutput(current_circ->getGateCount() - i - 1, i); */
      /* } */
      /* is_equal_to_col = false; */
    /* } */
}

//also maps correct gates to outputs
bool Ckt_Algo::circuitMatchesDesired() {
    // check if output of current circuit equals desired
    Circuit * current_circ = &ex_list.front();

    //check permutations of output and map if equal
    /* checkPermAndMap(current_circ, desired); */
    vector<vector<bool>> circ_output = current_circ->evaluateAllInputs();

    if(unique_map.find(pair<size_t, int>(hash_output(circ_output), 0)) == unique_map.end())
    if(circ_output.size() != desired.size()){
      errlog("Getting a weird output size while checking correctness", true);
      return false;
    }
    for (int i = 0; i < circ_output.size(); ++i) {
      if (circ_output[i] != desired[i]) {
        return false;
      }
    }

    //if it got this far, circuit must be correct!
    correct_circuit_output = circ_output;
    return true;
}

void Ckt_Algo::addNot(int counter) {
  // only add a not gate to the end of the circuit
  if(counter < 12 || (counter > 14 && counter < 20)){
    return;
  }

  /* for (int i = 0; i < counter - 1; ++i) { */
    Circuit next = ex_list.front();
    next.addGate(NOT, counter - 1);
    string errmsg = "Ckt_Algo::addNot: " + to_string(next.getGateCount()) +
      " NOT " + to_string(counter);
    errlog(errmsg);
    if(isUnique(next)){
      ex_list.push(next);
    }
  /* } */
}

void Ckt_Algo::addAnd(int counter) {
  int left_bound = (counter > 8)? counter - 8 : 0;
  int right_bound = (counter > left_bound + 5)? left_bound + 5 : counter - 1;
  /* int right_max_connection =  6; */
  for (int i = left_bound; i < right_bound; ++i) {
    for (int j = counter - 5; j < counter - 1; ++j) {
      Circuit next = ex_list.front();
      next.addGate(AND, i, j);
      string errmsg = "Ckt_Algo::addAnd: " + to_string(next.getGateCount()) +
        " AND " + to_string(i) + " " + to_string(j);
      errlog(errmsg);
      if(isUnique(next)){
        ex_list.push(next);
      }
    }
  }

}

void Ckt_Algo::addOr(int counter) {
  int left_bound = (counter > 15)? counter - 15 : 0;
  int right_bound = (counter > left_bound + 5)? left_bound + 5 : counter - 1;
  int higher_left = counter - 8;
  for (int i = left_bound; i < right_bound - 1; ++i) {
    for (int j = higher_left; j < counter - 1; ++j) {
      Circuit next = ex_list.front();
      next.addGate(OR, i, j);
      string errmsg = "Ckt_Algo::addOr: " + to_string(next.getGateCount()) +
        " OR " + to_string(i) + " " + to_string(j);
      errlog(errmsg);
      if(isUnique(next)){
        ex_list.push(next);
      }
    }
  }
}

vector<vector<bool>> Ckt_Algo::search(vector<vector<bool>> desired) {
  searchPreProcess();
  while (circuitMatchesDesired() == false) {
    errlog("Ckt_Algo::search circuit did not match desired, search continuing");
    Circuit temp_c = ex_list.front();
    if(ex_list.empty()){
      errlog("Ckt_Algo::NO MORE QUEUE CIRCUITS");
      exit(-1);
    }
    int gate_count = temp_c.getGateCount();
    // adds NOT/AND/OR gate
    /* errlog("Ckt_Algo::search addNot successful", true); */
    if(temp_c.getNotCount() < 2){
      addNot(gate_count);
    }
    if(temp_c.getGateCount()) {
      addAnd(gate_count);
      /* errlog("Ckt_Algo::search addAnd successful", true); */
      addOr(gate_count);
      /* errlog("Ckt_Algo::search addOr successful", true); */
    }
    errlog("Ckt_Algo::search found :" + to_string(unique_map.size()) + " unique circs ", true);
    errlog("Ckt_Algo::Queue still has :" + to_string(ex_list.size()) + " possibilities", true);
    errlog("Ckt_Algo::On Level Number::" + to_string(gate_count) + " ", true);

    // remove "first" element
    ex_list.pop();
  }
  errlog("Ckt_Algo::search found correct circuit!");
  return correct_circuit_output;
}
