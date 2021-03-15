#/bin/sh

if [ $# -eq 0 ]
then
	graph_dir=graphs
else
	graph_dir=$1
fi

for graph in $graph_dir/*.edges
do
	echo "Running gramm on $graph..."
	gramm=$(timeout 30s ./gramm*/ecc -3 -4 < $graph | wc -l)
	echo "Got $gramm"
	echo "Running mine on $graph..."
	mine=$(timeout 30s ./ecc $graph 2>/dev/null)
	echo "Got $mine"
	if [ $gramm -eq $mine ]
	then
		echo "Passed."
	else
		echo "Failed!"
		# exit 1
	fi
done