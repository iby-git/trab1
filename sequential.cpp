#include <iostream>
#include <fstream>
#include <vector>

// Function to multiply two matrices
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n1 = A.size();
    int m1 = A[0].size();
    int n2 = B.size();
    int m2 = B[0].size();

    // Check if the matrices can be multiplied
    if (m1 != n2) {
        std::cerr << "Error: Matrices cannot be multiplied. Number of columns in M1 must be equal to the number of rows in M2." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::vector<int>> result(n1, std::vector<int>(m2, 0));

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            for (int k = 0; k < m1; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

// Function to read a matrix from a file
std::vector<std::vector<int>> readMatrixFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    int numRows, numCols;
    file >> numRows >> numCols;

    std::vector<std::vector<int>> matrix(numRows, std::vector<int>(numCols));

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            file >> matrix[i][j];
        }
    }

    return matrix;
}

// Function to write a matrix to a file
void writeMatrixToFile(const std::vector<std::vector<int>>& matrix, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    int numRows = matrix.size();
    int numCols = matrix[0].size();

    file << numRows << " " << numCols << "\n";

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            file << matrix[i][j] << " ";
        }
        file << "\n";
    }
}

int main() {
    // Read matrices from input files
    std::vector<std::vector<int>> M1 = readMatrixFromFile("M1.txt");
    std::vector<std::vector<int>> M2 = readMatrixFromFile("M2.txt");

    // Multiply the matrices
    std::vector<std::vector<int>> M3 = multiplyMatrices(M1, M2);

    // Write the result to an output file
    writeMatrixToFile(M3, "M3.txt");

    std::cout << "Matrix multiplication complete. The result has been stored in M3.txt." << std::endl;

    return 0;
}