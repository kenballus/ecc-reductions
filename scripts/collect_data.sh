#/bin/bash

if [ $# -eq 0 ]
then
    echo 'Usage: ./collect_data.sh <graph_directory>'
    exit 1
fi

graph_dir=$1

echo 'Graph,ECC,Calls,Time'
for graph in $graph_dir/*.edges
do
    echo -n "$(basename $graph),"
    ./ecc < $graph | sed -e 's/^ //g' | sed -e 's/ \+/,/g' # replace all strings of spaces with single commas
    echo
done
