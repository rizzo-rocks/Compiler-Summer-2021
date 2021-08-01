if ! g++ -g -std=c++11 Compiler.cpp; then
    echo error building
    exit 1
fi

(cd tests; ./test.sh)

