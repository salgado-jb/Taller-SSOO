CC = gcc
CFLAGS = -Wall -Wextra -pedantic
TARGET = ejercicio1

# Comprobamos el sistema operativo
ifeq ($(OS),Windows_NT)
    RM = del /Q #Q: quiet
else
    RM = rm -f #f: force (estos flags son para que no haya error si alguno de los archivos no existe)
endif


all: $(TARGET) 						# Target que se ejecuta si no se especifica ningun target

$(TARGET): $(TARGET).o				# $@ contiene el nombre del target actual, $^ contiene todos los prerequisitos
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET).o: $(TARGET).c					# $< contiene el primer requisito (dependecia)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.obj *.exe $(TARGET)
#Para windows es 'del' en lugar de 'rm', '.obj' en lugar de '.o', y el ejecutable tiene extensión '.exe'

.PHONY: all clean