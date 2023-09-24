# trab1
Trabalho 01 de SO

a) Qual o motivo dos resultados obtidos no experimento E1? O que pode ter causado o comportamento observado?

O programa sequencial tem o aumento de tempo esperado e quadrático baseado na expansão das dimensões das matrizes. O programa de threads foi muito mais rápido devido ao paralelismo, o que também é o esperado para o de processos

b) Qual o motivo dos resultados obtidos no experimento E2? O que pode ter causado o comportamento observado?

Os programas de threads e processos aumentam sua eficiencia relativamente à partição das atividades em threads, quanto mais bem particionado, mais rápida a execução.

c) Qual é o valor de P ideal para a multiplicação das matrizes M1 e M2? Justifique sua resposta através dos experimentos realizados.

Valores baixos tendem a serem mais eficientes, mas muito baixos não. Valores entre 10 e 100 apresentaram os melhores tempos de execução.

Link da spreadsheet > https://docs.google.com/spreadsheets/d/1w9tuNFJXTYYc9MBeeNyapENB5SAeC8n3-oW2AO7DVu4/edit?usp=sharing

Link do grafico > https://www.desmos.com/calculator/g00doavk0t

Link do relatório > https://docs.google.com/document/d/1PZ5zQz4TPmqkJEC_SlQ9pr_MH1Y4EiUMSehzzg1fCb4/edit?usp=sharing


compile:

g++ auxiliar.cpp -o auxiliar; g++ sequential.cpp -o sequential; g++ thread.cpp -o thread; g++ process.cpp -o process

run:

./auxiliar 100 100 100 100
./sequential M1.txt M2.txt
./thread M1.txt M2.txt 1250
./process M1.txt M2.txt 1250
