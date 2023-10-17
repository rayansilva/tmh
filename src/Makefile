include ../makedef

CPPFLAGS := ${CC_OPTIONS} -I../include -I../include/protocol -I../include/utility -I../include/business -I../include/persistence -I../include/test

PROTOCOLDIR := protocol
UTILITYDIR := utility
BUSINESSDIR := business
PERSISTENCEDIR:= persistence

SOURCES := $(wildcard $(PROTOCOLDIR)/*.cpp $(UTILITYDIR)/*.cpp $(BUSINESSDIR)/*.cpp $(PERSISTENCEDIR)/*.cpp)

OBJS := $(subst .cpp,.o,$(SOURCES))

SUBDIRS := $(PROTOCOLDIR) $(UTILITYDIR) $(BUSINESSDIR) $(PERSISTENCEDIR)

.PHONY: $(SUBDIRS)



objects: directories bin

directories:
	@$(MAKE) -j$(MAKE_J) $(SUBDIRS)

$(SUBDIRS):
	   @$(MAKE) --directory=$@

bin:
	@$(MAKE) -j$(MAKE_J) lib tmhd

lib: libtmh.so.1

libtmh.so.1: $(OBJS)
	$(CXX) -shared -fPIC -Wl,-soname,libtmh.so.1 $(LDFLAGS) $^ -o libtmh.so.1

tmhd: $(OBJS) tmhd.o

depend:
	@for subdir in $(SUBDIRS); \
	do \
		echo "making $@ in $$subdir"; \
		$(MAKE) -C $$subdir $@ || exit 1; \
	done;
	@echo "# DO NOT DELETE" > Makefile.deps
	@gccmakedep -fMakefile.deps -w 102400 $(DEPPATH) *.cpp
	@rm Makefile.deps.bak

clean:
	@for subdir in $(SUBDIRS); \
	do \
		echo "making $@ in $$subdir"; \
		( cd $$subdir && $(MAKE) $@ ) || exit 1; \
	done;
	@find . -perm +111 -type f -exec rm -f {} \;
	@rm -f *.o *.so.* Makefile.deps Makefile~ *.cpp~ tmhd

-include Makefile.deps
