#ifndef __LOADING_DATA__
#define __LOADING_DATA__

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cctype>

namespace fs = std::filesystem;

template <typename T>
const auto load_data = [](const std::string &folder, std::vector<T> &dest)
{
    std::vector<char> buffer;

    for (const auto &entry : fs::directory_iterator(folder))
    {
        if (fs::is_regular_file(entry))
        {

            std::ifstream file(entry.path());
            if (file.is_open())
            {

                auto raw = std::string((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());

                buffer.clear(); // keep buffer clean
                if (buffer.capacity() < raw.length())
                { // enlarge buffer when necessary
                    buffer.reserve(raw.length());
                }

                bool skip = false;
                for (const char c : raw)
                {

                    // keep skiping or start skip if < or {
                    skip = skip || c == '<' || c == '{';

                    if (c == '>' || c == '}')
                    { // stop skipping and push whitespace

                        skip = false;
                        buffer.push_back(' ');
                    }
                    else if (!skip && !ispunct(c))
                    { // if not skip and not puctuation, convert to lowercase and push

                        char c_lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                        buffer.push_back(c_lower);
                    }
                }

                // fill dest with the modified string contents
                if (!buffer.empty())
                {
                    dest.emplace_back(T(buffer.begin(), buffer.end()));
                }

                file.close();
            }
            else
            {
                throw new std::runtime_error("cannot read file " + entry.path().string());
            }
        }
    }
};

/**
 * this function fills the <code>dest</code> vector with the (modified) content of each file in the <code>data_folder_path</code> folder.
 * The file content is modified to lower case and each html tag (anything between &lt; and &gt;) is replaced by a white space.
 * The punctuations are also removed.
 */
template <typename T>
void load_aclImdb_data(const std::string &data_folder_path, std::vector<T> &positive, std::vector<T> &negative)
{

    load_data<std::string>(data_folder_path + "/pos", positive);
    load_data<std::string>(data_folder_path + "/neg", negative);
}

template <typename T>
void load_wiki(const std::string &data_folder_path, std::vector<T> &data)
{

    load_data<std::string>(data_folder_path, data);
}

#endif