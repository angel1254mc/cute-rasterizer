#!/bin/bash

# Run make clean and make in the current directory
echo "Running make clean and make in the current directory..."
make clean
make

# Run make clean and make in the ./test/test-pipeline-1 directory
echo "Running make clean and make in the ./test/test-pipeline-1 directory..."
(cd ./test/test-pipeline-1 && make clean && make)

# Run the executable found in ./bin/test_pipeline
echo "Running the executable found in ./bin/test_pipeline..."
./bin/test_pipeline
