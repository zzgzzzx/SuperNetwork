BIN = ./bin/SuperVPN 
CROSS = 
CC = $(CROSS)gcc  
CPP = $(CROSS)g++  
LD = $(CROSS)ld

DEFINC = -I../../include -I../../include/cJson -I/usr/include/curl -I../../include/GW
INCS += $(DEFINC)  

CSRCS = $(wildcard ./*.c ./../*.c)  
CPPSRCS = $(wildcard ./*.cpp ./../*.cpp)  
 
COBJS := $(CSRCS:.c=.o)  
CPPOBJS := $(CPPSRCS:.cpp=.o)  

CFLAGS += $(INCS)  
CFLAGS += -w -o2 -Wall -g -fPIC  

CPPFLAGS += $(INCS)  
CPPFLAGS += -w -o2 -Wall -g -fPIC  

LDFLAGS += -lcurl -pthread -lmysqlclient
  
all:$(BIN)  
$(COBJS) : %.o: %.c  
	$(CC) $(CFLAGS) -c $< -o $@   
$(CPPOBJS) : %.o: %.cpp  
	$(CPP) $(CPPFLAGS) -c $< -o $@   
  
$(BIN) : $(COBJS) $(CPPOBJS)  
	$(CPP) -o $(BIN) $(COBJS) $(CPPOBJS) $(LDFLAGS) $(LIBS)   
	rm $(COBJS)  
	rm $(CPPOBJS)  

.PHONY:clean cleanall  
 
clean:              
	rm $(BIN) #$(COBJS) $(CPPOBJS)  
 
cleanall:  
	rm $(BIN) $(COBJS) $(CPPOBJS)  
	-rm $(OBJS) $(TARGET)