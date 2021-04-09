#/bin/bash

if [ $# -eq 0 ]
then
    echo 'Usage: ./time_conte_bound.sh <graph_directory>'
    exit 1
fi

graph_dir=$1

echo 'Graph,ConteBound,ECC,TimeUnbounded,TimeBounded,CallsUnbounded,CallsBounded'
for graph in $graph_dir/*.edges
do
    conte_bound=$(java -jar conte/ECC8.jar -f aa -g $graph -o /tmp | grep 'Cliques: ' | awk '{print $2}')
    read -r cliques_without_bound calls_without_bound time_without_bound < <(./ecc < $graph 2>/dev/null)
    read -r cliques_with_bound calls_with_bound time_with_bound < <(./ecc $conte_bound < $graph 2>/dev/null)
    echo "$(basename $graph),$conte_bound,$cliques_without_bound,$time_without_bound,$time_with_bound,$calls_without_bound,$calls_with_bound"
done
