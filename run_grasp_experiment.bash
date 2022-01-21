# Essse script executa o experimento de ajuste de parâmetros para as heurísticas baseadas em GRASP para todas as 
# consforme especificado no projeto deste trabalho. Consideramos que as instâncias a serem usadas para o ajuste
# (já complementadas) estão em arquivos no formato do DIMACS e possuem extensão ".clq" estão em uma pasta "in" no 
# diretório atual e a saída obtida é direcionada para um arquivo de mesmo nome da instância, seguido pelo valor de 
# alpha sendo considerado (separado por um "-") e a extensão ".out". Os arquivos de saída são colocados em um 
# diretório "out/GRASPEXP-(nome do algoritmo utilizado)" que é criado no diretório atual (se não existir). O 
# executável do solver deve estar em um diretório "bin" e ser nomeado como "MVCSolver". O tempo de execução também
# é mensurado para cada instância utilizando a função time, conforme definido no experimento.

alphas=(0.1 0.5) # Lista com os valores de alpha a serem considerados
reps=100 # Número de repetições a ser utilizado
greedy_algo="gd" # Nome da estratégia grasp a ser utilizada (gw p/ WEIDEG ou gd p/ DEG)
mkdir -p "out/GRASPEXP-${greedy_algo}"
for alpha in ${alphas[@]}; do
    for instance in in/*.clq; do
        instance_file=$(basename ${instance})
        instance_name=$(basename ${instance} .clq)
        output_file="out/GRASPEXP-${greedy_algo}/${instance_name}-${alpha}.out"
        { time ./bin/MVCSolver -i ${instance} -s ${greedy_algo} -a ${alpha} -r ${reps}; } 2>&1 | tee ${output_file}
    done;
done;