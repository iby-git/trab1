#include <iostream>
#include <fstream>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

// Função para multiplicar uma parte das duas matrizes
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

// Função para multiplicar duas matrizes usando processos
vector<string> multiplyMatrices(const vector<vector<int>> &A, const vector<vector<int>> &B, int P)
{
    int n1 = A.size();
    int m2 = B[0].size();

    vector<vector<int>> result(n1, vector<int>(m2, 0));
    vector<pid_t> childProcesses;
    vector<string> fileNames;

    // Split the work among P child processes
    int rowsPerProcess = n1 / P;
    int startRow = 0;
    int endRow = 0;

    for (int i = 0; i < P; i++)
    {
        startRow = endRow;
        endRow += rowsPerProcess;
        if (i == P - 1)
        {
            endRow = n1;
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            cerr << "Erro ao criar o processo filho." << endl;
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Processo filho
            auto partResult = multiplyPart(A, B, startRow, endRow);
            string fileName = "result_" + to_string(i) + ".txt";
            fileNames.push_back(fileName);
            writeMatrixToFile(partResult.first, fileName);
            ofstream file(fileName, ios_base::app);
            file << partResult.second;
            exit(EXIT_SUCCESS);
        }
        else
        {
            childProcesses.push_back(pid);
        }
    }

    // Esperar que todos os processos filhos terminem
    for (int i = 0; i < P; i++)
    {
        int status;
        waitpid(childProcesses[i], &status, 0);
    }

    return fileNames;
}

// Função para ler uma matriz de um arquivo
vector<vector<int>> readMatrixFromFile(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
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

// Função para escrever uma matriz em um arquivo
void writeMatrixToFile(const vector<vector<int>> &matrix, const string &filename)
{
    ofstream file(filename);
    if (!file)
    {
        cerr << "Erro ao abrir o arquivo para escrita: " << filename << endl;
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

// Função para combinar arquivos de resultado em um único arquivo de saída
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
    // Verificar o número correto de argumentos da linha de comando
    if (argc != 4)
    {
        cerr << "Uso: " << argv[0] << " M1 M2 P" << endl;
        return 1;
    }

    // Ler matrizes dos arquivos de entrada
    vector<vector<int>> M1 = readMatrixFromFile(argv[1]);
    vector<vector<int>> M2 = readMatrixFromFile(argv[2]);

    // Extrair o número de processos da linha de comando
    int P = atoi(argv[3]);

    // Iniciar a contagem do tempo
    auto start = high_resolution_clock::now();

    // Multiplicar as matrizes e obter os nomes dos arquivos de resultado
    vector<string> resultFiles = multiplyMatrices(M1, M2, P);

    // Combinar os arquivos de resultado em um único arquivo de saída
    combineResultFiles(resultFiles, "M3.txt");

    // Parar a contagem do tempo
    auto stop = high_resolution_clock::now();
    auto elapsed = high_resolution_clock::now() - start;
    long long duration = duration_cast<microseconds>(elapsed).count();

    // Adicionar o tempo total ao arquivo de saída
    ofstream output("M3.txt", ios_base::app);
    output << "\nTempo total (microssegundos): " << duration << endl;

    cout << "Multiplicação de matrizes concluída. O resultado foi armazenado em M3.txt." << endl;

    return 0;
}
