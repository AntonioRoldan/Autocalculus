objects := $(patsubst %.c,%.o,$(wildcard *.cpp))
derivatives : $(objects)
        g++ -std=c++11 -o derivatives $(objects)