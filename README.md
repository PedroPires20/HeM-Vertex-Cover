# Heurísticas e Metaheurísticas - Vertex Cover

Este repositório armazena o código que implementa as heurísticas e metaheurísticas a serem comparadas no trabalho de pesquisa da disciplina de Heurísticas e Metaheurísticas, lecionada pelo professor Thiago Ferreira de Noronha durante o segundo semestre de 2021 no DCC/UFMG. 

O trabalho foi realizado pelos alunos Bernardo Teixeira de Amorim Neto e Pedro Dias Pires e tem por objetivo comparar os resultados de diferentes heurísticas para o problema de otimização combinatória *Minimum Vertex Cover (MVC)* ou cobertura mínima de vértices. As heurísticas comparadas são:

- [List Right](https://www.researchgate.net/publication/220114326_A_better_list_heuristic_for_vertex_cover), como controle
- Instanciação da metaheurística GRASP aplicada a heurística gulosa tracional
- Instanciação da metaheurística GRASP aplicada a uma nova heurística gulosa proposta no trabalho

## Instruções de Execução

As instâncias dadas como entradas devem ser arquivos de texto, no formato utilizado pela biblioteca de instâncias DIMACS para o problema da clique, especificado no seguinte [link](http://lcs.ios.ac.cn/~caisw/Resource/about_DIMACS_graph_format.txt#:~:text=DIMACS%20(Center%20for%20Discrete%20Mathematics,for%20problems%20in%20undirected%20graphs.&text=In%20this%20format%2C%20nodes%20are,n%20edges%20in%20the%20graph.). A execução do programa requer que seja informado o caminho para o arquivo de entrada, por meio do modificador `-i` seguido desse caminho e o algoritmo a ser utilizado, que pode variar entre *List Right*, GRASP com heurística gulosa (DEG) ou GRASP com heurística nossa (WEIDEG). O algoritmo deve ser especificado por meio do modificador `-s` seguido das siglas lr, gd ou gw, para cada um dos algoritmos anteriores, nessa ordem. Quando aplicável (i.e. GRASP é utilizado), os valores do parâmetro *alpha* e o número de execuções para amostragem dos resultados devem ser especificados por meio dos modificadores `-a` e `-r`, respectivamente.

Seguindo as especificações anteriores, o comando para a execução do programa possui o seguinte formato:

```bash
./MVCSolver -i (caminho da instância) -s (lr | gd | gw) -a (valor de alpha - opcional) -r (número de repetições - opcional)
```
