#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <set>

bool operator==(const std::vector<bool>& lhs, const std::vector<bool>& rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (int i = 0; i < lhs.size(); i++) {
        if (lhs.at(i) != rhs.at(i)) {
          return false;
        }
    }
    return true;
}


struct Record
{
  std::string classification;
  std::vector<bool> categoricalVariables;
};

class Classifier {
  virtual void Train(const std::vector<Record>& _records) = 0;
  virtual  std::unordered_map<std::string, double> Classify(const std::vector<bool>& queryRec) = 0;
};

// class NaiveBayesClassification : public Classifier {
//   public:

//   NaiveBayesClassification(const std::vector<Record>& _records): records(_records) {}

//   /**
//    * Conditional probability based classification
//    * */
//   std::unordered_map<std::string, double> Classify(const std::vector<bool>& queryRec) {

//   }

//   private:

//   std::vector<Record> records;
// };

class ExactBayesClassification : public Classifier {
  public:

  void Train(const std::vector<Record>& _records) {
    for (auto const& record: _records) {
      classes.insert(record.classification);
      if (matchLookup.find(record.categoricalVariables) == matchLookup.end()) {
        matchLookup[record.categoricalVariables] = {};
      }
      matchLookup[record.categoricalVariables].push_back(record.classification);
    }
  }

  std::unordered_map<std::string, double> Classify(const std::vector<bool>& queryRec) {
    if (matchLookup.find(queryRec) == matchLookup.end()) {
      return {};
    }

    std::unordered_map<std::string, double> intermediate;
    auto queryExactMatches = matchLookup.at(queryRec);
    for (auto const& match : queryExactMatches) {
      if (intermediate.find(match) == intermediate.end()) {
        intermediate[match] = 0;
      }
      intermediate[match]++;
    }

    std::unordered_map<std::string, double> result;

    for (auto classification : classes) {
      result[classification] = 0;
    }

    double numMatches = (double) queryExactMatches.size();
    for (auto it : intermediate) {
      result[it.first] = it.second / numMatches;
    }

    return result;
  }

  private:
  std::set<std::string> classes;
  std::unordered_map< std::vector<bool>, std::vector<std::string>> matchLookup;
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

  ExactBayesClassification ebc;

  ebc.Train(recs);

  std::vector<bool> query = {true, false, true, true};

  auto probs = ebc.Classify(query);

  for (auto res: probs) {
    std::cout << res.first << ": " << res.second << "\n";
  }
}