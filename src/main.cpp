#include <iostream>
#include <bitset>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <future>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <utility>

#include "BFS.h"

std::pair<std::vector<int>, std::vector<int>> parse_file(std::string& filename) {
    std::string line;
    std::ifstream file (filename);

    int n = 0;
    int m = 0;

    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (std::getline(file, line)) {
        if (line != "AdjacencyGraph") {
            std::cerr << "Invalid file format." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if (std::getline(file, line)) {
        n = stoi(line);
    }

    if (std::getline(file, line)) {
        m = stoi(line);
    }

    std::vector<int> vertices (n + 1);
    std::vector<int> edges (m);

    int i = 0;

    while (std::getline(file, line)) {
        vertices[i++] = stoi(line);

        if (i == n) break;
    }

    i = 0;

    while (std::getline(file, line)) {
        edges[i++] = stoi(line);
    }

    vertices[n] = i;

    file.close();

    return std::make_pair(vertices, edges);
}

int main(int argc, char **argv)
{
    char c;
    int source;
    std::string filename;

    while ((c = getopt(argc, argv, "f:")) != -1) {
        switch (c) {
            case 'f':
                filename.assign(optarg);
                break;
            case 's':
                source = atoi(optarg);
                break;
            default:
                std::cerr << "Invalid parameter." << std::endl;
                return 1;
        }
    }

    auto graph = parse_file(filename);

    auto [vertices, edges] = graph;

    BFS bfs (vertices, edges);

    return 0;
}
