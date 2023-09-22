#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to multiply two matrices
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n1 = A.size();
    int m1 = A[0].size();
    int n2 = B.size();
    int m2 = B[0].size();

    // Check if the matrices can be multiplied
    if (m1 != n2) {
        cerr << "Error: Matrices cannot be multiplied. Number of columns in M1 must be equal to the number of rows in M2." << endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<int>> result(n1, vector<int>(m2, 0));

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
vector<vector<int>> readMatrixFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    int numRows, numCols;
    file >> numRows >> numCols;

    vector<vector<int>> matrix(numRows, vector<int>(numCols));

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            file >> matrix[i][j];
        }
    }

    file.close();
    return matrix;
}

// Function to write a matrix to a file
void writeMatrixToFile(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    int numRows = matrix.size();
    int numCols = matrix[0].size();

    file << numRows << " " << numCols << "\n";

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            file << "c" << i+1 << j+1 << " " << matrix[i][j] << " ";
            file << "\n";
        }
    }
}

int main(int argc, char* argv[]) {

    // Check for the correct number of command-line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " M1 M2" << std::endl;
        return 1;
    }

    // Read matrices from input files
    vector<vector<int>> M1 = readMatrixFromFile(argv[1]);
    vector<vector<int>> M2 = readMatrixFromFile(argv[2]);

    // Start counting the time
    auto start = high_resolution_clock::now();

    // Multiply the matrices
    vector<vector<int>> M3 = multiplyMatrices(M1, M2);

    // Stop counting the time
    auto stop = high_resolution_clock::now();
    auto elapsed = high_resolution_clock::now() - start;
    long long duration = duration_cast<microseconds>(elapsed).count();

    // Print done
    cout << "Matrix multiplication complete. The result has been stored in M3.txt." << endl;

    // Write the result to an output file
    writeMatrixToFile(M3, "M3.txt");
    ofstream file;
    file.open("M3.txt", ios_base::app);
    file << duration;

    return 0;
}