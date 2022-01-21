# Essse script executa a heurística GRASP com o algoritmo guloso DEG para todas as instâncias consideradas no 
# experimento deste trabalho (i.e. que não foram utilizadas para o ajuste de parâmetros). Consideramos que as 
# instâncias (já complementadas) estão em arquivos no formato do DIMACS e possuem extensão ".clq" em uma pasta 
# "in" no diretório atual e a saída obtida é direcionada para um arquivo de mesmo nome da instância com extensão 
# ".out" colocada em um diretório "out/GRASP-GD" que é criado no diretório atual (se não existir). O executável 
# do solver deve estar em um diretório "bin" e ser nomeado como "MVCSolver". O tempo de execução também é mensurado
# para cada instância utilizando a função time, conforme definido no experimento.

mkdir -p out/GRASP-GD
alpha=0.5 # Define o valor de alpha
reps=100 # Define o número de repetições
for instance in in/*.clq; do
    instance_file=$(basename ${instance})
    instance_name=$(basename ${instance} .clq)
    output_file="out/GRASP-GD/${instance_name}.out"
    { time ./bin/MVCSolver -i ${instance} -s gd -a ${alpha} -r ${reps}; } 2>&1 | tee ${output_file}
done;