# trab1
 Trabalho 01 de SO
a) Qual o motivo dos resultados obtidos no experimento E1? O que pode ter causado o comportamento observado?
 
b) Qual o motivo dos resultados obtidos no experimento E2? O que pode ter causado o comportamento observado?
 
c) Qual é o valor de P ideal para a multiplicação das matrizes M1 e M2? Justifique sua resposta através dos experimentos realizados.
 

Link da spreadsheet > https://docs.google.com/spreadsheets/d/1w9tuNFJXTYYc9MBeeNyapENB5SAeC8n3-oW2AO7DVu4/edit?usp=sharing
Link do grafico > https://www.desmos.com/calculator/g00doavk0t


compile:

g++ auxiliar.cpp -o auxiliar; g++ sequential.cpp -o sequential; g++ thread.cpp -o thread; g++ process.cpp -o process

run:

./auxiliar 100 100 100 100
./sequential M1.txt M2.txt
./thread M1.txt M2.txt 1250
./process M1.txt M2.txt 1250
 100 