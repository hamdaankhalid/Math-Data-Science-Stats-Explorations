#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <set>


struct Record
{
  std::string classification;
  std::vector<bool> categoricalVariables;
};

class Classifier {
  virtual void Train(const std::vector<Record>& _records) = 0;
  virtual  std::unordered_map<std::string, double> Classify(const std::vector<bool>& queryRec) = 0;
};


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


/* *
   * For each classification compute P(Query maps to classification | queryRec vector vals)
   * Expand this to:
   * P(queryRec[0] | classification) * P(queryRec[1] | classification)....*P(queryRec[i] | classification) / P(vector[0])*...*P(vector[i])
   * 
   * Helpful equations:
   * P (A | B) = P ( A & B) / P(B)
   * P (x) = number of times x occurs / total occurences
   * */
class NaiveBayesClassification : public Classifier {
  public:

  /**
   * For each record we can create lookup data structures.
   * We should store the following :
   * - probabilities of each classification
   * - A vector of the training records for in time access
   * - A set of all classifications
   * */
  void Train(const std::vector<Record>& _records) {
    records = _records;
    
    for (auto const& record : records) {
      auto classification = record.classification;
      
      classes.insert(classification);

      if (classificationProbabilites.find(classification) == classificationProbabilites.end()) {
        classificationProbabilites[classification] = 0;
      }
      classificationProbabilites[classification]++;
    }
    for (auto it: classificationProbabilites) {
      classificationProbabilites[it.first] = it.second / (double) records.size();
    }
  }
  
  std::unordered_map<std::string, double> Classify(const std::vector<bool>& queryRec) {
    std::unordered_map<std::string, double> result;

    for (auto classification: classes) {
      // find the probability of this classification and add it to our result map
      result[classification] = ProbabilityOfClassificationGivenQuery(classification, queryRec);
    }

    return result;
  }

  private:
  
  std::set<std::string> classes;
  std::vector<Record> records;
  std::unordered_map<std::string, double> classificationProbabilites;

  /**
   * P(classification) * [P(queryRec[0] | classification) *..* P(queryRec[i] | classification)
   *                      ------------------------------------------------------------------
   *                                  P(queryRec[0]) *..* P(queryRec[i])
   * 
   * => P(classification) * P(queryyRec[0] && classification) / P(classification) *** P(queryyRec[i] && classification) / P(classification)
   *                        ----------------------------------------------------------------------------------------------------------------
   *                        P(queryRec[0]) *..* P(queryRec[i])
   *
   * => P(classification) * (#queryRec[0]&&classification / #classification) ** (#queryRec[i]&&classification / #classification)
   *                        -----------------------------------------------------------------------------------------------------
   *                               (#queryRec[0]/#recs) *..* (#queryRec[i]/#recs) 
   * 
   * Helper Equation Expansion:
   * P(queryRec[i] | classification) = P(classificaton && queryRec[i]) / P (classification)
   * 
   * P(classificaton && queryRec[i]) = ....
   * */
  double ProbabilityOfClassificationGivenQuery(std::string& classification, const std::vector<bool>& queryRec) {
    double denominator = 1;
    double numerator = 1;
    double classificationProb = classificationProbabilites.at(classification);

    for (int i = 0; i < queryRec.size(); i++) {
      double queryRecAndClassificationCount = 1;
      double classificationCount = 0;
      double recQueryCatCount = 1;

      for (auto rec: records) {

        if (rec.categoricalVariables.at(i)) {
          recQueryCatCount++;
        }

        if (rec.classification == classification && rec.categoricalVariables.at(i) == queryRec.at(i)) {
          queryRecAndClassificationCount++;
        }

        if (rec.classification == classification) {
          classificationCount++;
        }
      }
      
      numerator *= queryRecAndClassificationCount/classificationCount;
      denominator *= recQueryCatCount/records.size();
    }

    return classificationProb * (numerator/denominator);
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

  ExactBayesClassification ebc;

  NaiveBayesClassification nbc;

  ebc.Train(recs);
  nbc.Train(recs);

  std::vector<bool> query = {true, false, true, true};

  auto ebcProbs = ebc.Classify(query);

  std::cout << "####### EXACT BAYES CLASSIFICATION #######" <<std::endl;

  for (auto res: ebcProbs) {
    std::cout << res.first << ": " << res.second << "\n";
  }

  std::cout << "####### NAIVE BAYES CLASSIFICATION #######" <<std::endl;


  auto nbcProbs = nbc.Classify(query);

  for (auto res: nbcProbs) {
    std::cout << res.first << ": " << res.second << "\n";
  }
}
