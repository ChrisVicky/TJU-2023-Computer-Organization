CC = g++
OPT = -O3 -m32
#OPT = -g -m32
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

NUM=3020202184
NAME=刘锦帆

# List all your .cc files here (source files, excluding header files)
SIM_SRC = main.cc

# List corresponding compiled object files here (.o files)
SIM_OBJ = main.o cache.o

TEST_OBJ = test.o para.o
 
#################################

# default rule

all: sim_cache
	@echo "my work is done here..."


# rule for making sim_cache

sim_cache: $(SIM_OBJ)
	$(CC) -o sim_cache $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH SIM_CACHE-----------"


# generic rule for converting any .cc file to any .o file
.cc.o:
	$(CC) $(CFLAGS) -c $*.cc

# type "make clean" to remove all .o files plus the sim_cache binary
clean:
	rm -f *.o sim_cache
	rm -f *.txt *.diff


# type "make clobber" to remove all .o files (leaves sim_cache binary)

clobber:
	rm -f *.o

test: clean sim_cache teststart t6 t7 t8 t9 t10 testdone 
	rm -f *.o sim_cache
	rm -f *.txt *.diff

teststart:
	@echo "--------------------TEST START--------------------"

testdone:
	@echo "--------------------TEST DONE--------------------"

t6: sim_cache
	./sim_cache 32 2048 4096 4 8 0 gcc_trace.txt > 6B.txt && diff -iw ../validation/Validation6_PartB.txt 6B.txt 
	
t7: sim_cache
	./sim_cache 16 1024 8192 8 4 0 go_trace.txt > 7B.txt && diff -iw ../validation/Validation7_PartB.txt 7B.txt 

t8: sim_cache
	./sim_cache 32 1024 0 8 0 256 perl_trace.txt > 8B.txt && diff -iw ../validation/Validation8_PartB.txt 8B.txt

t9: sim_cache
	./sim_cache 128 1024 4096 2 4 1024 gcc_trace.txt > 9B.txt && diff -iw ../validation/Validation9_PartB.txt 9B.txt 

t10: sim_cache
	./sim_cache 64 8192 16384 2 4 1024 perl_trace.txt > 10B.txt && diff -iw ../validation/Validation10_PartB.txt 10B.txt 

zip: clean
	zip -r ${NUM}_${NAME}_project1-2 ./*
