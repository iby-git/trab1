#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

// Function to multiply a portion of two matrices
pair<vector<vector<int>>, long long> multiplyPart(const vector<vector<int>> &A, const vector<vector<int>> &B, int startRow, int endRow)
{
    int n1 = A.size();
    int m1 = A[0].size();
    int m2 = B[0].size();
    vector<vector<int>> result(endRow - startRow, vector<int>(m2, 0));

    auto start = high_resolution_clock::now();

    for (int i = startRow; i < endRow; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            result[i - startRow][j] = 0;
            for (int k = 0; k < m1; k++)
            {
                result[i - startRow][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto elapsed = high_resolution_clock::now() - start;
    long long duration = duration_cast<microseconds>(elapsed).count();

    return {result, duration};
}

// Function to multiply two matrices using threads
vector<string> multiplyMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B, int P)
{
    int n1 = A.size();
    int m2 = B[0].size();

    vector<vector<int>> result(n1, vector<int>(m2, 0));
    vector<thread> threads;
    vector<string> fileNames;

    // Split the work among P threads
    int rowsPerThread = n1 / P;
    int startRow = 0;
    int endRow = 0;

    for (int i = 0; i < P; i++)
    {
        startRow = endRow;
        endRow += rowsPerThread;
        if (i == P - 1)
        {
            endRow = n1;
        }

        // Create a unique filename for this result segment
        string fileName = "result_" + to_string(i) + ".txt";
        fileNames.push_back(fileName);

        threads.emplace_back([=, &A, &B, &result]()
                             {
            auto partResult = multiplyPart(A, B, startRow, endRow);
            writeMatrixToFile(partResult.first, fileName);
            ofstream file(fileName, ios_base::app);
            file << partResult.second; });
    }

    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }

    return fileNames;
}

// Function to read a matrix from a file
vector<vector<int>> readMatrixFromFile(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    int numRows, numCols;
    file >> numRows >> numCols;

    vector<vector<int>> matrix(numRows, vector<int>(numCols));

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            file >> matrix[i][j];
        }
    }

    file.close();
    return matrix;
}

// Function to write a matrix to a file
void writeMatrixToFile(const vector<vector<int>> &matrix, const string &filename)
{
    ofstream file(filename);
    if (!file)
    {
        cerr << "Error opening file for writing: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    int numRows = matrix.size();
    int numCols = matrix[0].size();

    file << numRows << " " << numCols << "\n";

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            file << matrix[i][j] << " ";
        }
        file << "\n";
    }

    file.close();
}

// Function to combine result files into a single output file
void combineResultFiles(const vector<string> &fileNames, const string &outputFile)
{
    ofstream output(outputFile);

    for (const string &fileName : fileNames)
    {
        ifstream file(fileName);
        output << file.rdbuf();
        file.close();
    }

    output.close();
}

int main(int argc, char *argv[])
{
    // Check for the correct number of command-line arguments
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " M1 M2 P" << endl;
        return 1;
    }

    // Read matrices from input files
    vector<vector<int>> M1 = readMatrixFromFile(argv[1]);
    vector<vector<int>> M2 = readMatrixFromFile(argv[2]);

    // Extract the value of P from the command line
    int P = atoi(argv[3]);

    // Multiply the matrices and get the result filenames
    vector<string> resultFiles = multiplyMatrices(M1, M2, P);

    // Combine the result files into a single output file
    combineResultFiles(resultFiles, "M3.txt");

    cout << "Matrix multiplication complete. The result has been stored in M3.txt." << endl;

    return 0;
}
