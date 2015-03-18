OS := $(shell uname)
OPTIONS:= -Wmultichar

ifeq ($(OS),Darwin)
	OPTIONS += -framework OpenCL
else
	OPTIONS += -I/usr/local/browndeer/include -L/usr/local/browndeer/lib -lcoprthr_opencl
endif

main: main.cpp
	g++ -Wall -g main.cpp -o ./bin/main $(OPTIONS)

clean:
	rm -rf main