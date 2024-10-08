TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

example : clean compile
	./$(TARGET) -f ./employees.db -n
	./$(TARGET) -f ./employees.db -a "John Doe,123 Main St,40"
	./$(TARGET) -f ./employees.db -a "Timmy,12234 Second St,50"
	./$(TARGET) -f ./employees.db -l -a

test-update : clean compile
	./$(TARGET) -f ./employees.db -n
	./$(TARGET) -f ./employees.db -a "John Doe,123 Main St,40"
	./$(TARGET) -f ./employees.db -l
	./$(TARGET) -f ./employees.db -u "John Doe,60"
	./$(TARGET) -f ./employees.db -l

test-remove : clean compile
	./$(TARGET) -f ./employees.db -n
	./$(TARGET) -f ./employees.db -a "John Doe,123 Main St,40"
	./$(TARGET) -f ./employees.db -a "Timmy,12234 Second St,50"
	./$(TARGET) -f ./employees.db -l
	./$(TARGET) -f ./employees.db -r Timmy
	./$(TARGET) -f ./employees.db -l

test-remove-duplicated : clean compile
	./$(TARGET) -f ./employees.db -n
	./$(TARGET) -f ./employees.db -a "John Doe,123 Main St,40"
	./$(TARGET) -f ./employees.db -a "Timmy,12234 Second St,50"
	./$(TARGET) -f ./employees.db -a "Timmy,12234 Second St,50"
	./$(TARGET) -f ./employees.db -l
	./$(TARGET) -f ./employees.db -r Timmy
	./$(TARGET) -f ./employees.db -l

compile: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?


obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude