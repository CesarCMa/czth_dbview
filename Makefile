TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run : clean default
	./$(TARGET)
	./$(TARGET) -n
	./$(TARGET) -n -f
	./$(TARGET) -f employees.db
	./$(TARGET) -n -f employees.db

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?


obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude