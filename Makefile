include makedef

SUBDIRS = src

all: clean depend objects

objects:
	@for subdir in $(SUBDIRS); \
   do \
		echo "making $@ in $$subdir"; \
      ( cd $$subdir && $(MAKE) $@ ) || exit 1; \
   done;

depend:
	@for subdir in $(SUBDIRS); \
	do \
		echo "making $@ in $$subdir"; \
      ( cd $$subdir && $(MAKE) $@ ) || exit 1; \
	done;

clean:
	@for subdir in $(SUBDIRS); \
	do \
		echo "making $@ in $$subdir"; \
		( cd $$subdir && $(MAKE) $@ ) || exit 1; \
	done;
