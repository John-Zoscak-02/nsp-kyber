SOURCES = libtomcryptrsa.c 
HEADERS = libtomcryptrsa.h 

CC ?= /usr/bin/gcc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wpointer-arith -O3 -fomit-frame-pointer
NISTFLAGS += -Wno-unused-result -O3 -fomit-frame-pointer
RM = /bin/rm

LIBNAME = libtomcrypt
LIBHEADERS = -I$(LIBNAME)/src/headers
LDFLAGS += -L$(LIBNAME) -l:$(LIBNAME).a 

.PHONY: all speed clean

speed: \
  test_speed1024 \
  test_speed2048 \
  test_speed4096 \

test_speed1024: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) $(LIBHEADERS) -DKEYSIZE=1024 $(SOURCES) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed1024 $(LDFLAGS) 

test_speed2048: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) $(LIBHEADERS) -DKEYSIZE=2048 $(SOURCES) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed2048 $(LDFLAGS) 

test_speed4096: $(SOURCES) $(HEADERS) cpucycles.h cpucycles.c speed_print.h speed_print.c test_speed.c randombytes.c
	$(CC) $(CFLAGS) $(LIBHEADERS) -DKEYSIZE=4096 $(SOURCES) randombytes.c cpucycles.c speed_print.c test_speed.c -o test_speed4096 $(LDFLAGS) 

clean:
	-$(RM) -rf test_speed1024
	-$(RM) -rf test_speed2048
	-$(RM) -rf test_speed4096
