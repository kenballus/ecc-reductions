#/bin/bash

if [ $# -eq 0 ]
then
    echo 'Usage: ./collect_data.sh <graph_directory>|<graph_file>'
    exit 1
fi

if [ -d $1 ]; then
    graphs=$1/*.edges
else
    graphs=$1
fi


echo 'Graph,ECC,Calls,Time'
for graph in $graphs
do
    echo -n "$(basename $graph),"
    ./ecc < $graph | sed -e 's/^ //g' | sed -e 's/ \+/,/g' # replace all strings of spaces with single commas
    echo
done
