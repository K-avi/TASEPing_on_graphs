TARGET:= tog
TEST :=test
DEBUG := togdbg
ALLPROGS= $(TARGET) $(TEST) $(DEBUG)

.PHONY:	all clean

all: $(ALLPROGS)
progs: $(PROGRAMS)

$(TEST): 
	$(MAKE) -C src/ ../test

$(TARGET): 
	$(MAKE) -C src/ ../tog

$(DEBUG): 
	$(MAKE) -C src/ ../togdbg
clean:

	rm -f $(ALLPROGS)

.PHONY: clean
