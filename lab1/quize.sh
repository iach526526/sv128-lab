mkdir -p build


g++ -std=c++17 -O2 -o ./build/lab1_test main.cpp lab1.cpp \
    -I/usr/local/include -L/usr/local/lib -lsv128

./build/lab1_test