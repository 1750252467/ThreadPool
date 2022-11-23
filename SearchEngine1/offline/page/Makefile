SRCS:=$(wildcard src/*.cc)
OBJS:=$(patsubst src/%.cc,objs/%.o, $(SRCS))
OUT:=page

CC = g++

$(OUT): $(OBJS)
	$(CC) $(OBJS) -g -o $@  

objs/%.o: src/%.cc  
	@$(CC) -c $^    -o $@  


.PHONY: clean rebuild print 

clean: 
	rm -f $(OBJS)
	
rebuild: clean $(OUT)

print:
	echo $(OBJS)



