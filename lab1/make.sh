if [ -z "$1" ]
    then
        echo "One argument required. I need one C++ source file to compile."
        echo "Usage: ./make.sh <source_file.cpp>"
        exit 1
fi
mkdir -p build
g++ ${1}  lab1.cpp -I/usr/local/include -L/usr/local/lib -lsv128 -o ./build/run

./build/run