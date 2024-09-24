TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run : clean default
	./$(TARGET) -f ./employees.db -n
	./$(TARGET) -f ./employees.db -a "John Doe,123 Main St,40"
	./$(TARGET) -f ./employees.db -a "Timmy,12234 Second St,50"
	./$(TARGET) -f ./employees.db -l -a "John Doe,123 Main St,40"

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?


obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude