#!/bin/bash

# Start the Solstice server
# Listening at port 9090
/solstice/src/cmake-build-debug/solstice.web &

# Start the Solstice GUI
# Accessible at port 8080
python3 /solstice/controller/manage.py runserver 0.0.0.0:8080 &

# Wait for server or client to exit
wait -n

# Exit with status of process that exited first
exit $?