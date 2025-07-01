#include <iostream>
#include <chrono>
#include <algorithm>

#include "loading_data.hpp"
#include "tokenizer.hpp"

using DATA = std::vector<std::string>;

constexpr static size_t MAX_TOKENS = 1'000;

std::unordered_map<std::string, int> load_freqs(const std::string &data_folder_path)
{

    std::vector<char> buffer;
    std::unordered_map<std::string, int> freq;
    std::string contents;
    int count = 0;
    const int file_count = 8854772;
    const int count_step = file_count / 100;
    for (const auto &entry : fs::directory_iterator(data_folder_path))
    {
        buffer.clear();
        contents.clear();
        if (fs::is_regular_file(entry))
        {

            if (count > 0 && (count % count_step == 0)) {
                std::cout << "Parsing ";
                std::cout << (100. * count) / file_count << std::setprecision(1) << std::fixed;
                std::cout << " " << freq.size() << " ";
                std::cout << "%\n";
            }
            count++;

            std::ifstream file(entry.path());
            if (file.is_open())
            {

                auto raw = std::string((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());

                if (buffer.capacity() < raw.length())
                { 
                    buffer.reserve(raw.length());
                }

                bool skip = false;
                for (const char c : raw)
                {

                    skip = skip || c == '<' || c == '{';

                    if (c == '>' || c == '}')
                    { 

                        skip = false;
                        buffer.push_back(' ');
                    }
                    else if (!skip && !ispunct(c))
                    {

                        char c_lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                        buffer.push_back(c_lower);
                    }
                }

                if (!buffer.empty())
                {
                    contents = std::string(buffer.begin(), buffer.end());
                }

                file.close();
            }
            else
            {
                throw new std::runtime_error("cannot read file " + entry.path().string());
            }

            std::stringstream ss(contents);

            std::string word;

            while (ss >> word)
            {
                to_lower_case(word);
                freq[word]++;
            }
        }
    }

    return freq;
}

void
run_sort_loading(const std::unordered_map<std::string, int> &freqs)
{
    std::vector<std::pair<std::string, int>> pairs;

    for (const auto &pair : freqs)
    {
        pairs.push_back(std::make_pair(pair.first, pair.second));
    }
    std::sort(pairs.begin(), pairs.end(), [](auto &a, auto &b)
              { return b.second > a.second; });

    std::unordered_map<std::string, int> vocabulary;

    const size_t limit = std::min(MAX_TOKENS, pairs.size());
    for (int i = 0; i < limit; ++i)
    {
        const auto &pair = pairs[i];
        vocabulary[pair.first] = i + 1;
    }
}

void run_heap_loading(const std::unordered_map<std::string, int> &freqs)
{
    Tokenizer<std::string, MAX_TOKENS, 250> tokenizer;
    tokenizer.init_from_map(freqs);
}

int main(int, char **)
{

    DATA data;
    DATA positive;
    DATA negative;

    /*load_aclImdb_data<std::string>("../data/aclImdb_v1/aclImdb/train", positive, negative);
    data.insert(data.end(), positive.begin(), positive.end());
    data.insert(data.end(), negative.begin(), negative.end());*/

    //load_wiki("../data/wiki-corpus/txt", data);

    const std::unordered_map<std::string, int> freqs = load_freqs("../data/amazon-reviews/txt");

    std::cout << "Word frequences size: " << freqs.size() << "\n";

    for (int i = 0; i < 10; ++i)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < 100; ++j)
        {
            //run_heap_loading(freqs);
            run_sort_loading(freqs);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = end_time - start_time;
        long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << i << "\t" << milliseconds << "\n";
    }

    return 0;
}