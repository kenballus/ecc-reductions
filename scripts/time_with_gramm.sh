#/bin/bash

if [ $# -eq 0 ]
then
	graph_dir=graphs
else
	graph_dir=$1
fi

echo "Graph,Gramm,Me"
for graph in $graph_dir/*.edges
do
	gramm=$(/usr/bin/time -f "%U" bash -c "./gramm-ecc-reductions/ecc -4 < $graph &>/dev/null" 2>&1)
	mine=$(/usr/bin/time -f "%U" bash -c "./ecc < $graph &>/dev/null" 2>&1)
	echo "$(basename $graph),${gramm},${mine}"
done
