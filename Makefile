.POSIX:

include config.mk

SRC = bitwise_adj_mat.c
OBJ = ${SRC:.c=.o}

EXTRAFLAGS =

# default to error all: bitwise_adj_mat 
%.o: %.c
	@echo COMPILING CC $< with extra flags \"${EXTRAFLAGS}\"
	@${CC} -g -c ${CFLAGS} $< ${EXTRAFLAGS} -o $@

bitwise_adj_mat: ${OBJ}
	@echo "ERROR: unable to compile bitwise_adj_mat on it's own as it is a library"
	exit 1

cleanobj:
	@echo cleaning objects and temporary files
	@find . -iname '*.o' -delete

clean: cleanobj
	@echo cleaning tests
	@rm -f test_lh
	@echo cleaning gcov guff
	@find . -iname '*.gcda' -delete
	@find . -iname '*.gcov' -delete
	@find . -iname '*.gcno' -delete


test: run_tests

run_tests: compile_tests
	@echo "\n\nrunning test_bam"
	./test_bam
	@echo "\n"

compile_tests: clean ${OBJ}
	@echo "compiling tests"
	@${CC} test_bitwise_adj_mat.c -o test_bam ${LDFLAGS} ${CFLAGS} ${OBJ}
	@make -s cleanobj

example: run_example

run_example: compile_example
	@echo "\n\nrunning example"
	./example
	@echo "\n"

compile_example: clean ${OBJ}
	@echo "compiling example"
	@${CC} example.c -o example ${LDFLAGS} ${CFLAGS} ${OBJ}
	@make -s cleanobj


.PHONY: all clean cleanobj bitwise_adj_mat test example

