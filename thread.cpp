#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

void multiply_part(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B,
                   std::vector<std::vector<int>>& result, int start_row, int end_row, int thread_num) {
    int n1 = A.size();
    int m1 = A[0].size();
    int m2 = B[0].size();

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < m2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < m1; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Calculate the computation time for this thread
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // Save the result to the corresponding file
    write_matrix_to_file(result, "part_result_" + std::to_string(thread_num) + ".txt", duration);
}

void multiply_matrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B,
                      std::vector<std::vector<int>>& result, int P) {
    int n1 = A.size();
    int m2 = B[0].size();

    std::vector<std::thread> threads;

    int rows_per_thread = n1 / P;
    int start_row = 0;
    int end_row = 0;

    for (int i = 0; i < P; ++i) {
        auto start_time = std::chrono::high_resolution_clock::now(); // Start the timer for this thread
        start_row = end_row;
        end_row += rows_per_thread;
        if (i == P - 1) {
            end_row = n1;
        }
        threads.emplace_back(multiply_part, std::ref(A), std::ref(B), std::ref(result), start_row, end_row, i);
    }

    // Start all threads
    for (auto& thread : threads) {
        thread.join();
    }
}

std::vector<std::vector<int>> read_matrix_from_file(const std::string& filename) {
    std::ifstream file(filename);
    int numRows, numCols;
    file >> numRows >> numCols;
    std::vector<std::vector<int>> matrix(numRows, std::vector<int>(numCols));

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            std::string cell;
            file >> cell >> matrix[i][j];
        }
    }
    return matrix;
}

void write_matrix_to_file(const std::vector<std::vector<int>>& matrix, const std::string& filename, long long duration) {
    int numRows = matrix.size();
    int numCols = matrix[0].size();

    std::ofstream file(filename);
    file << numRows << " " << numCols << "\n";
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            file << "c" << i + 1 << j + 1 << " " << matrix[i][j] << " ";
        }
        file << "\n";
    }
    file << "Time taken: " << duration << " microseconds\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " M1 M2 P\n";
        return 1;
    }

    std::vector<std::vector<int>> M1 = read_matrix_from_file(argv[1]);
    std::vector<std::vector<int>> M2 = read_matrix_from_file(argv[2]);
    int P = std::stoi(argv[3]);

    std::vector<std::vector<int>> M3(M1.size(), std::vector<int>(M2[0].size()));

    auto start_time = std::chrono::high_resolution_clock::now();
    multiply_matrices(M1, M2, M3, P);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    write_matrix_to_file(M3, "M3.txt", duration);
    std::cout << "Multiplication of matrices complete. The result has been stored in M3.txt and includes the time taken for each thread.\n";

    return 0;
}
