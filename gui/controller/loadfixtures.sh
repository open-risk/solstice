#!/bin/bash

# List of apps to load fixtures for

NAME[0]="workflow_explorer"
# NAME[1]="results_explorer"
# NAME[2]="visualization_explorer"

# source venv/bin/activate
for i in ${NAME[@]}
do
python3 manage.py loaddata --format=json $i  $i/fixtures/$i.json
done

