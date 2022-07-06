CXX=clang++
CXX_FLAGS=-std=c++20 -Iincludes -g -fstandalone-debug -O0 -Wall -Wextra -Werror

CXX_FLAGS_=-std=c++20 -Iincludes -g -fstandalone-debug -O0 -Wall -Wextra

exec: bin/exec

bin/exec: ./includes/pixel.h ./includes/stb_image_write.h ./includes/render.h ./src/render.cpp ./includes/main.h ./src/main.cpp
	$(CXX) $(CXX_FLAGS_) $^ 

.DEFAULT_GOAL := exec
.PHONY: exec clean

clean:
	rm -f bin/*