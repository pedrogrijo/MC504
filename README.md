Projeto 1 - Sudoku
=====


Laboratório 01 - MC504: Sistemas Operacionais

Grupo: 
	Felipe Rodrigues Novaes  	- 135663
	Matheus Yokoyama Figueiredo	- 137036
	Pedro Rodrigues Grijó	 	- 139715

######## Visão Geral ########
Nós implementamos 3 programas, sendo um para cada tarefa, da seguinte forma:
i)checkSudoku: Esse programa implementa a requisição de verificar se um dado sudoku
		    preenchido está correto ou não.

ii)hintSudoku: Esse programa implementa a requisição de dado um sodoku com campos marcado
		 com "X" vazio, fornecer os possíveis valores para essas posições.

iii)solveSudoku: Com auxilio do hintSudoku, este programa resolve o sudoku, entregando a matriz pronta.



######################### Instruções de Uso ###################################

Para executar os programas, inicialmente, faz-se necessário rodar o arquivo "Makefile" disponibilizado.
Da seguinte forma: ...(GRIJO explica como rodar o makefile)

Então teremos as rotinas: checkSudoku(i), hintSudoku(ii) e solveSudoku(iii). Seguinte instruçoes
para cada um desses:

Siga as instruções da tela do menu.

O checkSudoku aceita uma matriz 9x9 inserindo-a LINHA POR LINHA, representando o sudoku, que deve estar completa. 

Após colocado a matriz completa o programa fará as devidas computações usando nossa estratégia multithread(descrita abaixo) verificando a integridade do sudoku, mostrando, caso exista, todos os erros.

O hintSudoku recebe uma matriz 9x9 dada LINHA POR LINHA, com os numeros faltando representados por um X. 

Após colocar a matriz completa o programa farpa as devidas computações usando uma estratégia multithread(descrita abaixo) e imprimirá uma matriz 9x9 igual a dada só que no lugar dos "X" os possíveis valores que o sudoku aceita.

O solveSudoku executa o hintSudoku e a partir das dicas, começa a completar o sudoku usando uma estratégia multithread(descrita abaixo) e imprimirá a matriz completa.



######################## Estratégia Multithreas adotada ###########################

Cada um dos módulos citados acima (i), (ii) e (iii) tiveram uma abordagem multithread para solucionar
o problema central. Seguindo:

(i) Para verificar se um dado sudoku estava correto criamos tres tipos de threads:
	- Verificar uma dada linha.
	- Verificar uma dada coluna.
	- Verificar um dado bloco.
Então, instaciamos para cada uma dessas 9 com seu respectivo indice. Totalizamos, dessa forma, 27 threads
que juntas solucionavam o problema. A vantagem principal dessa abordagem é a naturalidade da implementação,
visto que pra cada lugar(linha,coluna e bloco) que precisa verificar de um sudoku criamos uma thread separada.

(ii) Nesse problema criamos uma matrix de 3 dimensões onde no piso está guardado o sudoku. E em cada andar" k",
com  1 <=k<= 9 guarda um booleano pra dizer se o grid do piso pode usar o numero "k". Assim, nosso problema se reduz
a preencher essa matrix. Assim, usamos uma abordagem com tres tipos de threads:
        - Preencher para uma dada LINHA todos valores que nao podem ser usados na mesma.
        - Preencher para uma dada COLUNA todos valores que nao podem ser usados na mesma.
        - Preencher para um dado BLOCO todos valores que nao podem ser usado no mesmo.
Instaciamos 9 threads para cada um desses, totalizando 27 threads. Essas juntas preencheriam a matrix para todos
grids. Assim, teriamos como vantagem que não precisamos verificar separadamente para cada grid, o que necessitaria
de uma abordagem com 81 threads. Todavia, usamos mais memória, segundo nossa estrutra de dados de 3 dimensões.
O motivo pela escolha se baseia no fato de um sudoku ter uma matrix apenas de 9x9, o que necessitamos de uma matrix
9x9x10, isso não causa grande impacto de memória.  

(iii)  Neste problema, se aproveitou das dicas fornecidas pelo hintSudoku para minimizar o tempo de preenchimento. Cada thread segue uma das dicas, independentemente, cobrindo assim todos os casos. No caso de completar a matriz, imprimi ela completa e para.
