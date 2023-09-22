#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

// Function to generate a random float between 0 and 1
float randomFloat() {
    return static_cast<float>(rand()) / RAND_MAX;
}

// Function to generate and store a matrix in a file
void generateAndStoreMatrix(int n, int m, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Generate and store the matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            float value = randomFloat();
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

    // Generate and store the first matrix M1
    generateAndStoreMatrix(n1, m1, "M1.txt");

    // Generate and store the second matrix M2
    generateAndStoreMatrix(n2, m2, "M2.txt");

    std::cout << "Matrices M1 and M2 have been generated and stored in M1.txt and M2.txt." << std::endl;

    return 0;
}