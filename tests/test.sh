#/bin/sh

for graph in ../graphs/*.el
do
	echo "Running gramm on $graph..."
	gramm=$(../gramm*/ecc < ../graphs/$graph | wc -l)
	echo "Running mine on $graph..."
	mine=$(../ecc ../graphs/$graph 2>/dev/null)
	if [ $gramm -eq $mine ]
	then
		echo "Passed."
	else
		echo "Failed!"
	fi
done

