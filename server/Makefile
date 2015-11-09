SRC_DIR := ./src
INC_DIR := ./include
EXE_DIR := ./bin
EXE := $(EXE_DIR)/main
SRC_OBJS := $(wildcard  $(SRC_DIR)/*.c )
INC_OBJS := $(wildcard $(INC_DIR)/*.h)
$(EXE):$(SRC_OBJS) $(INC_OBJS)
	gcc -g  -o $@ $(SRC_OBJS) -lpthread -lcrypt -I$(INC_DIR) 

.PHONY:clean 
clean :
	rm -f $(EXE) 


