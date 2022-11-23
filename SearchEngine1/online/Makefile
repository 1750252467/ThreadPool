OUT = bin/server
CC = g++
LIBS:= -lpthread -llog4cpp

SRCS  = $(wildcard src/Reactor/*.cc)
SRCS2 = $(wildcard src/SearchEngineServer/*.cc)
SRCS3 = $(wildcard src/KeyRecommander/*.cc)
SRCS4 = $(wildcard src/WebPageSearcher/*.cc)
SRCS5 = $(wildcard src/LRUCache/*.cc)
SRCS6 = $(wildcard src/*.cc)
SRCS7 = $(wildcard src/TimerManager/*.cc)

OBJS  = $(patsubst src/Reactor/%.cc,objs/%.o, $(SRCS))
OBJS2 = $(patsubst src/SearchEngineServer/%.cc,objs/%.o, $(SRCS2))
OBJS3 = $(patsubst src/KeyRecommander/%.cc,objs/%.o, $(SRCS3))
OBJS4 = $(patsubst src/WebPageSearcher/%.cc,objs/%.o, $(SRCS4))
OBJS5 = $(patsubst src/LRUCache/%.cc,objs/%.o, $(SRCS5))
OBJS6 = $(patsubst src/%.cc,objs/%.o, $(SRCS6))
OBJS7 = $(patsubst src/TimerManager/%.cc,objs/%.o, $(SRCS7))

OBJS_ALL = $(OBJS) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7)

$(OUT): $(OBJS_ALL)
	$(CC) $^ -o $@ $(LIBS) -g 

objs/%.o: src/Reactor/%.cc 
	@$(CC) -c $^ -o $@ -g
objs/%.o: src/SearchEngineServer/%.cc
	@$(CC) -c $^ -o $@ -g 
objs/%.o: src/KeyRecommander/%.cc
	@$(CC) -c $^ -o $@ -g 
objs/%.o: src/WebPageSearcher/%.cc
	@$(CC) -c $^ -o $@ -g
objs/%.o: src/LRUCache/%.cc
	@$(CC) -c $^ -o $@ -g 
objs/%.o: src/%.cc
	@$(CC) -c $^ -o $@ -g 
objs/%.o: src/TimerManager/%.cc
	@$(CC) -c $^ -o $@ -g 


.PHONY: clean rebuild print 

clean: 
	rm -rf $(OBJS_ALL) $(OUT)
print:
	echo $(OBJS_ALL)



