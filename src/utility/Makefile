include ../../makedef

SRCS=$(wildcard *.cpp)

OBJS=$(addsuffix .o, $(basename $(SRCS)))

all: objects

objects: $(OBJS)

clean:
	@rm -f *.o Makefile.deps *.cpp~

depend:
	@echo "# DO NOT DELETE" > Makefile.deps
	@gccmakedep -fMakefile.deps -w 102400 $(DEPPATH) $(SRCS)
	@rm -f Makefile.deps.bak

-include Makefile.deps
