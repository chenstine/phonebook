CC ?= gcc
CFLAGS_common ?= -O0 -Wall -std=gnu99

EXEC = phonebook_orig phonebook_opt
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

run: $(EXEC)
	watch -d -t ./phonebook_orig

clean:
	$(RM) $(EXEC) *.o perf.*

clean_cache:
	echo "echo 1 > /proc/sys/vm/drop_caches" | sudo sh

stat_orig: clean_cache
	perf stat -e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses ./phonebook_orig

stat_opt: clean_cache
	perf stat -e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses ./phonebook_opt

record_orig_f12500: clean_cache
	sudo perf record -F 12500 -e cache-misses ./phonebook_orig && sudo perf report

record_opt_f12500: clean_cache
	sudo perf record -F 12500 -e cache-misses ./phonebook_opt && sudo perf report

record_orig: clean_cache
	sudo perf record -e cache-misses ./phonebook_orig && sudo perf report

record_opt: clean_cache
	sudo perf record -e cache-misses ./phonebook_opt && sudo perf report
