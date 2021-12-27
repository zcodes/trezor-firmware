ifneq ($(shell uname -s),"Darwin")
CFLAGS=-D OSX
endif

CFLAGS += "-I.."

all: reference_signer unit_tests

SRCS = base10.c \
	base58.c \
	blake2b-ref.c \
	sha256.c \
	crypto.c \
	pasta_fp.c \
	pasta_fq.c \
	poseidon.c \
	utils.c \
	curve_checks.c \
	../sha2.c \
	../memzero.c

OBJS = $(SRCS:.c=.o)


reference_signer: $(OBJS) reference_signer.c
	$(CC) $(CFLAGS) -Wall -Werror $@.c -o $@ $(OBJS) -lm

.PRECIOUS: unit_tests
unit_tests: $(OBJS) *.c *.h
	$(CC) $(CFLAGS) -Wall -Werror $@.c -o $@ $(OBJS) -lm
	@./$@

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $*.o -c $<

clean:
	rm -rf *.o *.log reference_signer unit_tests
