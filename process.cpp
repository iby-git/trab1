#include <iostream>
#include <fstream>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to multiply a portion of two matrices
void multiplyPart(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& result, int startRow, int endRow) {
    int n1 = A.size();
    int m1 = A[0].size();
    int m2 = B[0].size();

    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < m2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < m1; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to multiply two matrices using processes
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, int P) {
    int n1 = A.size();
    int m2 = B[0].size();

    vector<vector<int>> result(n1, vector<int>(m2, 0));

    // Split the work among P child processes
    int rowsPerProcess = n1 / P;
    int startRow = 0;
    int endRow = 0;

    for (int i = 0; i < P; i++) {
        startRow = endRow;
        endRow += rowsPerProcess;
        if (i == P - 1) {
            endRow = n1;
        }

        pid_t pid = fork();

        if (pid == -1) {
            cerr << "Error creating child process." << endl;
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            multiplyPart(A, B, result, startRow, endRow);
            exit(EXIT_SUCCESS);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < P; i++) {
        int status;
        wait(&status);
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
            file << matrix[i][j] << " ";
        }
        file << "\n";
    }
}

int main(int argc, char* argv[]) {
    // Check for the correct number of command-line arguments
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " M1 M2 P" << endl;
        return 1;
    }

    // Read matrices from input files
    vector<vector<int>> M1 = readMatrixFromFile(argv[1]);
    vector<vector<int>> M2 = readMatrixFromFile(argv[2]);

    // Extract the number of processes from the command line
    int P = atoi(argv[3]);

    // Start counting the time
    auto start = high_resolution_clock::now();

    // Multiply the matrices
    vector<vector<int>> M3 = multiplyMatrices(M1, M2, P);

    // Stop counting the time
    auto stop = high_resolution_clock::now();
    auto elapsed = high_resolution_clock::now() - start;
    long long duration = duration_cast<microseconds>(elapsed).count();

    // Write the result to an output file
    writeMatrixToFile(M3, "M3.txt");
    ofstream file;
    file.open("M3.txt", ios_base::app);
    file << duration;

    cout << "Matrix multiplication complete. The result has been stored in M3.txt." << endl;

    return 0;
}