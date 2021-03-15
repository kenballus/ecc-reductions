# Compares Gramm's mysterious lower bound to the lower bound provided by KaMIS.

if [ $# -eq 0 ]
then
    graph_dir=graphs
else
    graph_dir=$1
fi

echo "file,kamis_bound,gramm_bound,gramm_time_total(ms),gramm_time_wasted(ms)" > data.csv
metis_graph="$(uuidgen).metis"
for graph in $graph_dir/*.edges
do
    python scripts/edge_list_to_metis.py $graph > $metis_graph
    echo "Finding KaMIS bound for $graph..."
    kamis_bound="$(./KaMIS/deploy/redumis $metis_graph | tail -n 1 | awk '{print $NF}')"
    echo "Running Gramm..."
    gramm="$(python scripts/.time_gramm.py $graph $kamis_bound)"
    echo -n "$graph," >> data.csv
    echo -n "$kamis_bound," >> data.csv
    echo "$gramm" >> data.csv
    rm $metis_graph
done