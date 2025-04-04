#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    // Read the input file
    ifstream input_file("D:/Victor/Faculdade/3_Periodo/Estrutura_de_Dados/Lista_3/CovidGenomeBlock/data/covidGenome.txt");
    if (!input_file.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    vector<string> lines;
    string line;
    while (getline(input_file, line)) {
        lines.push_back(line);
    }
    input_file.close();

    // Remove the last line
    if (!lines.empty()) {
        lines.pop_back();
    }

    // Initialize hash table
    const int HASH_TABLE_SIZE = 4096;
    struct Entry {
        string block;
        int count = 0;
    };
    Entry hash_table[HASH_TABLE_SIZE] = {};

    // Lambda function to compute hash
    auto compute_hash = [](const string &block) {
        int hash = 0;
        for (char c : block) {
            int bits;
            switch(c) {
                case 'A': bits = 0; break;
                case 'T': bits = 1; break;
                case 'C': bits = 2; break;
                case 'G': bits = 3; break;
                default:
                    cerr << "Invalid character in block: " << c << endl;
                    exit(1);
            }
            hash = (hash << 2) | bits;
        }
        return hash;
    };

    // Process each line
    for (const string &line : lines) {
        if (line.size() != 60) {
            continue; // Skip lines that don't have exactly 60 characters
        }
        for (int i = 0; i < 10; ++i) {
            int start = i * 6;
            string block = line.substr(start, 6);
            int hash = compute_hash(block);
            assert(hash >= 0 && hash < HASH_TABLE_SIZE);

            if (hash_table[hash].count == 0) {
                hash_table[hash].block = block;
                hash_table[hash].count = 1;
            } else {
                // Ensure the stored block matches the current block (sanity check)
                if (hash_table[hash].block != block) {
                    cerr << "Hash collision detected for blocks: '"
                              << hash_table[hash].block << "' and '" << block << "'" << endl;
                    exit(1);
                }
                hash_table[hash].count++;
            }
        }
    }

    // Collect all entries with count > 0
    vector<Entry> entries;
    for (auto & i : hash_table) {
        if (i.count > 0) {
            entries.push_back(i);
        }
    }

    // Sort entries lexicographically by block
    sort(entries.begin(), entries.end(), [](const Entry &a, const Entry &b) {
        return a.block < b.block;
    });

    // Write the results to the output file
    ofstream output_file("D:/Victor/Faculdade/3_Periodo/Estrutura_de_Dados/Lista_3/CovidGenomeBlock/data/covidOutput.txt");
    if (!output_file.is_open()) {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    for (const Entry &entry : entries) {
        output_file << entry.block << " " << entry.count << endl;
    }
    output_file.close();

    return 0;
}