#!/bin/bash

for i in {1..20}
do
    for j in {1..100}
    do
        ./gramm-ecc-reductions/random-graph $i "$(python -c 'import random; print(round(random.random(), 2))')" > temp.el
        if ! ./ecc temp.el 2>&1 >/dev/null
        then
            exit 0
        fi
    done
done
