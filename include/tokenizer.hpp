#ifndef __TOKENIZER__
#define __TOKENIZER__

#include <vector>
#include <unordered_map>
#include <queue>
#include <cstring>

template <typename S>
void to_lower_case(S &word)
{
    for (auto &c : word)
    {
        c = tolower(c);
    }
}

template <typename S, int _MAX_TOKENS, int _MAX_SEQUENCE_SIZE>
class Tokenizer
{

    using DATA = std::vector<S>;
    using MAP = std::unordered_map<S, int>;
    using ENTRY = std::pair<S, int>;

public:
    Tokenizer() {}
    virtual ~Tokenizer() {}

    constexpr static int _UNKNOW_WORD_INDEX = 1;

    void init(const DATA &data)
    {

        // populating the freq map of words
        MAP freq;
        for (const S &sentence : data)
        {
            std::stringstream ss(sentence);

            S word;

            while (ss >> word)
            {
                to_lower_case(word);
                freq[word]++;
            }
        }

        auto comparator = [](ENTRY &a, ENTRY &b)
        {
            return a.second > b.second;
        };

        using COMPARATOR = decltype(comparator);

        // populating the min-heap
        std::priority_queue<ENTRY, std::vector<ENTRY>, COMPARATOR> min_heap(comparator);
        const int token_limit = _MAX_TOKENS - 2; // index 0 is reserved for 'no word' and 1 is reseverd for unknown words
        for (auto &it : freq)
        {
            min_heap.push(std::make_pair(it.first, it.second));

            if (min_heap.size() > token_limit)
            {
                min_heap.pop();
            }
        }

        int index = min_heap.size() + 1;
        while (!min_heap.empty())
        {
            const auto &pair = min_heap.top();
            this->dictionary[pair.first] = index;
            min_heap.pop();
            index--;
        }
    }

    void init_from_map(const MAP &freq)
    {

        const auto comparator = [](ENTRY &a, ENTRY &b)
        {
            return a.second > b.second;
        };

        using COMPARATOR = decltype(comparator);

        // populating the min-heap
        std::priority_queue<ENTRY, std::vector<ENTRY>, COMPARATOR> min_heap(comparator);
        const int token_limit = _MAX_TOKENS - 2; // index 0 is reserved for 'no word' and 1 is reseverd for unknown words
        for (auto &it : freq)
        {
            min_heap.push(std::make_pair(it.first, it.second));

            if (min_heap.size() > token_limit)
            {
                min_heap.pop(); // HERE
            }
        }

        int index = min_heap.size() + 1;
        while (!min_heap.empty())
        {
            const auto &pair = min_heap.top();
            this->dictionary[pair.first] = index;
            min_heap.pop();
            index--;
        }
    }

    auto build_vocabulary() const
    {
        const auto vocabulary = [map = this->dictionary](const S &sentence) mutable
        {
            std::vector<int> result(_MAX_SEQUENCE_SIZE);

            std::stringstream ss(sentence);
            S word;
            int i = 0;
            while (i < _MAX_SEQUENCE_SIZE && ss >> word)
            {
                to_lower_case(word);
                int index = map[word];
                if (!index) {
                    index = _UNKNOW_WORD_INDEX;
                }
                result[i] = index;
                i++;
            }

            return result;
        };

        return vocabulary;
    }

private:
    MAP dictionary;
};

#endif