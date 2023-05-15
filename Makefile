CC=aarch64-linux-gnu-gcc
#CFLAGS=-I /opt/jerryzheng/yocto_linux/build-condor-adas/tmp/sysroots-components/aarch64/i2c-tools/usr/include/
#CFLAGS+= -Wall -Wextra -Wpedantic
#LDFLAGS=-L /opt/jerryzheng/yocto_linux/build-condor-adas/tmp/sysroots-components/aarch64/i2c-tools/usr/lib/
#LDFLAGS+=-li2c

SRCS := $(shell find . -type f -name "*.c")
OBJS := $(patsubst %.c,%.o,$(SRCS))
EXES := $(patsubst %.c,%,$(SRCS))

all: $(EXES)

$(EXES): $(OBJS)
	$(CC) $(CFLAGS) $@.o $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXES)
