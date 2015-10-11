CC ?= gcc
CFLAGS_common ?= -O0 -Wall -std=gnu99

EXEC = phonebook_orig phonebook_opt phonebook_opt_hash
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) -DORIG -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) -DOPT -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

phonebook_opt_hash: $(SRCS_common) phonebook_opt_hash.c phonebook_opt_hash.h
	$(CC) $(CFLAGS_common) -DOPTHASH -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

run: $(EXEC)
	watch -d -t ./phonebook_orig

clean:
	$(RM) $(EXEC) *.o perf.*

clean_cache:
	echo "echo 1 > /proc/sys/vm/drop_caches" | sudo sh

stat:
	echo "echo 1 > /proc/sys/vm/drop_caches" | sudo sh
	perf stat -e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses ./phonebook_orig
	echo "echo 1 > /proc/sys/vm/drop_caches" | sudo sh
	perf stat -e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses ./phonebook_opt
	echo "echo 1 > /proc/sys/vm/drop_caches" | sudo sh
	perf stat -e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses ./phonebook_opt_hash

record_orig: clean_cache
	sudo perf record -e cache-misses ./phonebook_orig && sudo perf report

record_opt: clean_cache
	sudo perf record -e cache-misses ./phonebook_opt && sudo perf report

record_opt_hash: clean_cache
	sudo perf record -e cache-misses ./phonebook_opt_hash && sudo perf report

astyle:
	astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none *.[ch]
