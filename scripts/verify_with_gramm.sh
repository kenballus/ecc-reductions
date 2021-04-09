#/bin/sh

if [ $# -eq 0 ]
then
	graph_dir=graphs
else
	graph_dir=$1
fi

for graph in $graph_dir/*.edges
do
    echo -n "Running on $(basename $graph)..."
	gramm=$(./gramm*/ecc -4 < $graph | wc -l)
	mine=$(./ecc < $graph 2>/dev/null | awk '{print $1}')
	if [ $gramm -eq $mine ]
	then
		echo "passed."
	else
		echo "failed!"
        echo "I got $mine"
        echo "Gramm got $gramm"
		exit 1
	fi
done
