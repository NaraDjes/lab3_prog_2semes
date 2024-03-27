OBJ = src/$(executable).o src/coder.o src/command.o
executable = utf-8
all: $(executable)
$(executable): $(OBJ)
	gcc -o $@ $^
%.o: src/%.c
	gcc -Wall -o $@ $<
clean:
	rm $(executable) $(OBJ)
	rm -rf src/*.txt src/*.bin src/*.dat