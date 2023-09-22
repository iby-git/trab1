#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <random>

// Function to generate a random integer between min and max (inclusive)
int randomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

// Function to generate and store a matrix of random integers in a file
void generateAndStoreMatrix(int n, int m, const std::string& filename, int min, int max) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    // Generate and store the matrix
    file << n << ' ' << m << '\n';
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int value = randomInt(min, max);
            file << value << ' ';
        }
        file << '\n';
    }
}

int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " n1 m1 n2 m2" << std::endl;
        return 1;
    }

    // Extract dimensions from command-line arguments
    int n1 = std::atoi(argv[1]);
    int m1 = std::atoi(argv[2]);
    int n2 = std::atoi(argv[3]);
    int m2 = std::atoi(argv[4]);

    // Set the range for random integer values (adjust as needed)
    int min = 1;
    int max = 100;

    // Generate and store the first matrix M1
    generateAndStoreMatrix(n1, m1, "M1.txt", min, max);

    // Generate and store the second matrix M2
    generateAndStoreMatrix(n2, m2, "M2.txt", min, max);

    std::cout << "Matrices M1 and M2 with different random integers have been generated and stored in M1.txt and M2.txt." << std::endl;

    return 0;
}
