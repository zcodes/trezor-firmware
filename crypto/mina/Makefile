ifneq ($(shell uname -s),"Darwin")
CFLAGS=-D OSX
endif

CFLAGS += "-I.."

all: reference_signer unit_tests my_test

SRCS = base10.c \
	sha256.c \
	crypto.c \
	pasta_fp.c \
	pasta_fq.c \
	poseidon.c \
	utils.c \
	curve_checks.c \
	../sha2.c \
	../memzero.c \
	../base58.c \
	../blake2b.c \
	../hasher.c \
	../blake256.c \
	../ripemd160.c \
	../sha3.c \
	../groestl.c

OBJS = $(SRCS:.c=.o)


reference_signer: $(OBJS) reference_signer.c
	$(CC) $(CFLAGS) -Wall -Werror $@.c -o $@ $(OBJS) -lm

my_test: $(OBJS) my_test.c
	$(CC) $(CFLAGS) -Wall -Werror $@.c -o $@ $(OBJS) -lm

.PRECIOUS: unit_tests
unit_tests: $(OBJS) *.c *.h
	$(CC) $(CFLAGS) -Wall -Werror $@.c -o $@ $(OBJS) -lm
	@./$@

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $*.o -c $<

clean:
	rm -rf *.o *.log reference_signer unit_tests my_test
