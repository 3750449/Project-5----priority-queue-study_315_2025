cmake_minimum_required(VERSION 4.0)
project(priority-queue-study)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src/implementations/BinaryHeapInVector)
add_subdirectory(src/implementations/BinomialQueues utils/comparator.cpp)


add_executable(top_level_main
        main.cpp
        src/harness/RunResults.hpp
)


add_executable(harness
        src/harness/Operation.hpp
        src/harness/main.cpp
        src/implementations/BinaryHeapInVector/BinaryHeapInVector.cpp
        src/implementations/BinaryHeapInVector/BinaryHeapInVector.hpp
        utils/comparator.cpp
        src/implementations/BinomialQueues/BinomialQueue.cpp
        src/implementations/BinomialQueues/BinomialQueue.hpp
        src/implementations/BinomialQueues/BQnode.cpp
        src/harness/RunMetaData.hpp
        src/implementations/LinearBaseLine/LinearBaseLine.cpp
        src/implementations/LinearBaseLine/LinearBaseLine.hpp
        src/implementations/Oracle/QuadraticOracle.cpp
        src/implementations/Oracle/QuadraticOracle.hpp
)

add_executable(batch_then_drain_gen
        src/trace-generators/batch_then_drain_gen/main.cpp
)

add_executable(huffman_trace
        src/trace-generators/huffman_coding/main.cpp
)

