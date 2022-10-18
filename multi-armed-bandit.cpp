#include <set>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

/**
 * Epsilon Greedy Based Bandit Algorithm
 * Epsilon is between 0 - 1
 * A bandit may have N arms
 * Each arm has a performance score that is tracked everytime it is served
 * Every time we want to serve an arm we need to create a random number between 0 - 1 called r
 * if our r < epsilon, we can serve any arm randomly selected from our pool of arms
 * if our r > epsilon, we need to serve out best performing arm.
 * 
 * Note: If there is a tie in best performing arm then I am going to serve any of the tied best performing arms randomly
 * 
 * Everytime an entry is added we need to keep a track of it's score. The scoring metric
 * for things like click or no click can be taken via percentage, but for things like
 * scoring web page stickiness we should be able to store a score as a summation of average
 * webpage stickiness
 * */
class BanditArm {
  public:
  BanditArm(std::string id): identifier(id) {}

  std::string identifier;
  double raw_score = 0;
  int times_shown = 0;

  double getAveragedScore() const {
    if (times_shown == 0) return 0;
    return raw_score/times_shown;
  }

  // operator overload so that we can use this for sorting
  bool operator > (const BanditArm& obj2) const {
    return (getAveragedScore() > obj2.getAveragedScore());
  }

  std::string to_string() const {
    return "Identifier: " + identifier + " Performance: " + std::to_string(getAveragedScore());
  }
};

class EpsilonGreedyBandit {
  double epsilon;
  std::vector<BanditArm> arm_records;
  public:
    EpsilonGreedyBandit(double e, std::vector<std::string> arm_ids) {
      if (e < 0 || e > 1) {
        throw std::invalid_argument("Epsilong must be in range [0, 1]");
      }
      epsilon = e;
      std::set<std::string> seen_before;
      for (auto arm_id : arm_ids) {
        if (seen_before.find(arm_id) != seen_before.end()) {
          throw std::invalid_argument("Arm identifiers must be unique.");
        }
        BanditArm bd(arm_id);
        arm_records.push_back(bd);
      }
    }

    /**
     * This method uses our explore-exploit ratio called epsilon to give a candidate based on it's previous performance
     * or by random choice. It only performs selection based on multiple criterias and does not mutate any members.
     * */
    std::string getArm() {
      double explore_or_exploit = (double) rand()/RAND_MAX;
      if (explore_or_exploit > epsilon) {
        std::cout << "Bandit is exploring \n";
        int random_arm_index = rand() % arm_records.size();
        return arm_records.at(random_arm_index).identifier;
      } else {
        std::cout << "Bandit is exploiting \n";
        std::sort(arm_records.begin(), arm_records.end(), std::greater<BanditArm>());
        std::vector<BanditArm> best_arm_records;
        double prev_score = -1.00;
        int idx = 0;
        while (idx < arm_records.size() && arm_records.at(idx).getAveragedScore() >= prev_score) {
          best_arm_records.push_back(arm_records.at(idx));
          prev_score = arm_records.at(idx).getAveragedScore();
          idx++;
        }
        
        int random_tied_arm_index = rand() % best_arm_records.size();
        return best_arm_records.at(random_tied_arm_index).identifier;
      }
    }

    /* You may notice that this is the method we actually increment member variables for the specified arm in and
    *  not during the getArm method. The reasoning behind this is that there may be the case that a client asks
    *  for an arm candidate but fails to report back results for, in that case an arm was shown but since no result
    *  was returned it would bring down the averaged score for the arm since the times_shown would increment but
    *  not the raw_score. To avoid this I made the getArm method a const method, and add_arm_observation the one that
    * mutates the arm_record.
    * */
    void add_arm_observation(std::string arm, int value) {
      for (BanditArm& arm_rec : arm_records) {
        if (arm_rec.identifier == arm) {
          arm_rec.raw_score += value;
          arm_rec.times_shown++;
          return;
        }
      }
      std::cout << "Strange things are happening Harry.. \n";
    }

    /**
     * Print a formatted table of arm performance and statistics
     * */
    void report() const {
      std::cout << "Epsilon: " << epsilon << "\n";
      for (auto arm : arm_records) {
        auto repr = arm.to_string();
        for (int i = 0; i<repr.size(); i++) std::cout << "-";
        std::cout << "\n";
        std::cout << repr << "\n";
        for (int i = 0; i<repr.size(); i++) std::cout << "-";
        std::cout << "\n";
      }
    }
};

/**
 * The Driver program will run a simluation where we are testing the web page stickiness
 * based on what images we show our users everytime they open our landing screen.
 * */
int main() {
  // N groups that will be the arms of the bandit
  std::vector<std::string> sample_group_identifiers{ "dog images", "cat images", "horse images", "goat images" };
  // this decides our explore to exploit ratio
  double epsilon = 0.3;
  auto egb = EpsilonGreedyBandit(epsilon, sample_group_identifiers);

  std::cout << "Interactively see how Epsilon greedy multi armed bandits use explore-exploit \n";
  std::cout << "Enter -1 as the time on webpage to exit repl \n \n";
  // loop where a user gets a sample group signifying the image shown on the loading screen
  // and then enters the hypothetical seconds or milliseconds each page was stared at.
  double exit = -1;
  double user_input = 0;
  
  while (user_input != exit) {
    egb.report();
    auto sample_group = egb.getArm();
    std::cout << "Bandit selected " << sample_group << " ! \n";
    std::cout << "How many seconds did the user spend on the page? \n";
    std::cin >> user_input;
    if (user_input == exit) break;
    egb.add_arm_observation(sample_group, user_input);
    
    // clear screen
    std::cout << "\033[2J\033[H";
  }
  return 0;
};
