#!/bin/bash

# List of apps to create fixtures for
NAME[0]="workflow_explorer"
# NAME[1]="results_explorer"
# NAME[2]="visualization_explorer"


# source venv/bin/activate
for i in ${NAME[@]}
do
mkdir -p $i/fixtures
python3 manage.py dumpdata --format=json $i -o $i/fixtures/$i.json
done

