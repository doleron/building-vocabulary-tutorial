#include <iostream>

#include "loading_data.hpp"
#include "tokenizer.hpp"

int main(int, char**) {

    std::vector<std::string> positive_data;
    std::vector<std::string> negative_data;

    load_aclImdb_data<std::string>("../data/aclImdb_v1/aclImdb/train", positive_data, negative_data);

    Tokenizer<std::string, 10'000, 250> tokenizer;
    tokenizer.init(positive_data);

    const auto printout = [] (const std::vector<int> & idx, int limit) {
        for (int i = 0; i < limit; ++i) {
            std::cout << idx[i] << " ";
        }
        std::cout << "\n\n";
    };

    std::string sentence = positive_data[1000];

    std::cout << "Sentence: " << sentence.substr(0, 100) << "\n";

    auto vocabulary = tokenizer.build_vocabulary();

    std::vector<int> token_indices = vocabulary(sentence);

    std::cout << "Indices: ";
    printout(token_indices, 10);

    std::cout << "the" << ": ";
    printout(vocabulary("the"), 4);

    std::cout << "and" << ": ";
    printout(vocabulary("and"), 4);

    std::cout << "amazing" << ": ";
    printout(vocabulary("amazing"), 4);

    std::cout << "an unknwon word" << ": ";
    printout(vocabulary("kjs&7as"), 4);

    return 0;
}