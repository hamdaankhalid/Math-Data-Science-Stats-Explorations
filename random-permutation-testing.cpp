#include<vector>
#include<functional>
#include<numeric>
#include<string>
#include<iostream>

std::ostream& operator<<(std::ostream& os, const std::vector<double>& vector) {
  for (auto element : vector) {
    os << std::to_string(element) << " ";
  }
  return os;
}

/**
 * Multiple samples are combines and shuffled and a statisitic is compared across
 * original group and across randomized shuffled group, random group picking is done with replacement.
 * */
void random_permutation_test(std::vector<std::vector<double> >& original_sample, double stat(std::vector<double>&), int iterations) {
  std::vector<double> statistics_for_input;
  std::transform(original_sample.begin(), original_sample.end(), std::back_inserter(statistics_for_input), stat);
  std::cout << "Original Sample Group Statistic: " << statistics_for_input << "\n";

  std::vector<double> mergedpool;
  for (auto const &v: original_sample) {
    mergedpool.insert(mergedpool.end(), v.begin(), v.end());
  }
  std::cout << "Merged pool to pick random samples from: " << mergedpool << "\n";

  for (int i = 0; i < iterations; i++) {
    auto mergedpoolmutable = mergedpool;
    std::cout << "Random Pool " << i << " statistic: ";
    std::vector<double> stats_for_pool;
    
    for (auto const &vec: original_sample) {
      std::vector<double> group(vec.size(), 0);
      for (int j = 0; j < vec.size(); j++) {
        int randidx = rand() % mergedpoolmutable.size();
        group[j] = mergedpoolmutable.at(randidx);
        mergedpoolmutable.erase(mergedpoolmutable.begin()+randidx);
      }
      stats_for_pool.push_back(stat(group));
    }

    std::cout << stats_for_pool << "\n";
  }
}

int main() {
  // driver program running random permutation tests for 3 groups with statistic set to mean
  std::vector<std::vector<double> > foo{{5,6,3},{1,2,3},{4,5,6}};
  random_permutation_test(foo, [](std::vector<double>& input_data)->double {
    return (double)std::accumulate(input_data.begin(), input_data.end(), 0)/input_data.size();
  }, 100);
}