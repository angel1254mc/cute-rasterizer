#!/bin/bash

# Run make clean and make in the current directory
echo "Running make clean and make in the current directory..."
make clean
make

# Run make clean and make in the ./test/test-pipeline-1 directory
echo "Running make clean and make in the ./test/test-pipeline-1 directory..."
(cd ./test/test-multi-shapes && make clean && make)

# Run the executable found in ./bin/test_pipeline
echo "Running the executable found in ./bin/test_multi_shapes..."
./bin/test_multi_shapes
