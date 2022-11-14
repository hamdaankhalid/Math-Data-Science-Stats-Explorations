#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

struct Record
{
  std::string classification;
  std::vector<bool> categoricalVariables;
};


class ExactBayesClassification {
  public:
  
  ExactBayesClassification(const std::vector<Record>& _records): records(_records)  {}

  std::unordered_map<std::string, double> Classify(const std::vector<bool>& queryRec) {
    // P(queryRec is class A) = Number of all class A records that match queryRec / Total number of class A records
    // a data structure that can store keep a track of key with max value while giving constant time 
    // key (class) - (value) mapping
    int totalMatches = 0;
    std::unordered_map<std::string, int> occurenceClasses;
    for (auto const& rec : records) {
      if (occurenceClasses.find(rec.classification) == occurenceClasses.end()) {
        occurenceClasses[rec.classification] = 0;
      }

      if (isExactMatch(queryRec, rec)) {
        occurenceClasses[rec.classification]++;
        totalMatches++;
      }
    }

    std::unordered_map<std::string, double> res;
    // normalize our records before returning
    for (auto it : occurenceClasses) {
      double prob = (double) it.second / totalMatches;
      res[it.first] = prob;
    }

    return res;
  }

  private:

  std::vector<Record> records;

  bool isExactMatch(const std::vector<bool>& query, const Record& candidate) {
    if (candidate.categoricalVariables.size() != query.size()) {
        throw std::invalid_argument("query should be of same size as categorical variables in records");
    }

    for (int i = 0; i < query.size(); i++) {
        if (query.at(i) != candidate.categoricalVariables.at(i)) {
          return false;
        }
    }
    return true;
  }
};

int main() {
  std::vector<Record> recs = {
    {
      "A", 
      {true, false, true, true}
    },
    {
      "B", 
      {true, false, false, true}
    },
    {
      "C", 
      {true, false, true, true}
    },
    {
      "A", 
      {true, false, true, true}
    }
  };

  ExactBayesClassification ebc(recs);

  std::vector<bool> query = {true, false, true, true};

  auto probs = ebc.Classify(query);

  for (auto res: probs) {
    std::cout << res.first << ": " << res.second << "\n";
  }
}