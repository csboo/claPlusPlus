#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>

struct Args {
    std::string Output;
    std::string Charset;
    std::string Static;
    int Length;
};

void generateWords(std::queue<char>& charset, const int& len, const std::string& Static);
void true_or_throw(const bool& arg, const char* error);
Args handle_args(const int& argc, const char* argv[]);

static std::string indent(const int& n);

int main(const int argc, const char* argv[]) {
    try {
        Args args = handle_args(argc, argv);

        std::ofstream f(args.Output);

        std::string charset = args.Charset;
        std::queue<char> charset_queue;
        std::for_each(charset.begin(), charset.end(), [&charset_queue](const char& c) { charset_queue.push(c); });

        generateWords(charset_queue,args.Length,args.Static);

        f.close();
    } catch (char const* err) {
        std::cerr << "Error: " << err << "\n";
        exit(1);
    }
}

void generateWords(std::queue<char>& charset, const int& len, const std::string& Static) {
    // Transfer queue elements to a vector (preserving order)
    std::vector<char> chars;
    while (!charset.empty()) {
        chars.push_back(charset.front());
        charset.pop();
    }

    const int n = static_cast<int>(chars.size());
    std::cout << len << " " << n << "\n";
    if (len > n || len <= 0) {
        std::cerr << "Invalid length\n";
        return;
    }

    // Create an indices vector for the first combination [0, 1, ..., len-1]
    std::vector<int> indices(len);
    std::iota(indices.begin(), indices.end(), 0);

    while (true) {
        // Print the current combination
        for (int idx : indices) {
            std::cout << chars[idx];
        }
        std::cout << Static << "\n";

        // Generate the next combination:
        // Find the rightmost index that can be incremented
        int i = len - 1;
        while (i >= 0 && indices[i] == n - len + i)
            --i;
        if (i < 0)
            break; // No more combinations possible

        // Increment this index
        ++indices[i];

        // Reset all subsequent indices
        for (int j = i + 1; j < len; ++j) {
            indices[j] = indices[j - 1] + 1;
        }
    }
}

Args handle_args(const int& argc, const char* argv[]) {
    Args args;
    std::vector<std::string> argvec(argc - 1);
    const std::string APPNAME = argv[0];
    if (argc < 2) {
        argvec.emplace_back("--help");
    } else {
        for (int i = 1; i < argc; i++) {
            argvec.emplace_back(argv[i]);
        }
    }

    for (int i = 0; i < argvec.size(); i++) {
        if (argvec[i] == "--help" || argvec[i] == "-h") {
            std::cout << "c++ app to create custom wordlists\n\n"
                         "Usage: "
                      << APPNAME
                      << " <COMMAND>\n\n"
                         "Commands:\n"
                      << indent(1)
                      << "Nothing here yet!\n\n"
                         "Options:\n"
                      << indent(1) << "-h, --help\n"
                      << indent(1) << "-o, --output\n"
                      << indent(1) << "-c, --charset\n"
                      << indent(1)
                      << "-l, --lenght\n"
                         "\n\nWARNING: use all args, or things will break!\n\n";
            exit(0);
        } else if (argvec.at(i) == "--output" || argvec.at(i) == "-o") {
            true_or_throw(i < argvec.size() - 1, "Missing argument!\n");
            args.Output = argvec.at(i + 1);
            i++;
        } else if (argvec.at(i) == "--charset" || argvec.at(i) == "-c") {
            true_or_throw(i < argvec.size() - 1, "Missing argument!\n");
            args.Charset = argvec.at(i + 1);
            i++;
        } else if (argvec.at(i) == "--lenght" || argvec.at(i) == "-l") {
            true_or_throw(i < argvec.size() - 1, "Missing argument!\n");
            args.Length = stoi(argvec.at(i + 1));
            i++;
        } else if (argvec.at(i) == "--static" || argvec.at(i) == "-s") {
            true_or_throw(i < argvec.size() - 1, "Missing argument!\n");
            args.Static = argvec.at(i + 1);
            i++;
        }
    }

    return args;
}

void true_or_throw(const bool& arg, const char* error) {
    if (!arg) {
        throw error;
    }
}

static std::string indent(const int& n) { return std::string(n, '\t'); }
