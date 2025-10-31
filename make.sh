if ${1} == "" ; then
    echo "One argument required. I need one C++ source file to compile."
    echo "Usage: ./make.sh <source_file.cpp>"
    exit 1
fi
g++ ${1} -I/usr/local/include -L/usr/local/lib -lsv128 -o vector.app
