# pl0c Makefile

CC := gcc
TARGET := pl0c
IDIR := -I include
SDIR := src
ODIR := lbuild
CFILES := $(wildcard $(SDIR)/*.c)
OBJECTS := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES))
CFLAGS := $(IDIR) -fms-extensions -Ofast -DLIN

$(TARGET): $(ODIR) $(OBJECTS)
	$(CC) $(ODIR)/*.o -o $@

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

test:
	cd tests && ./test.sh

clean:
	rm -f *- ${TARGET}
	rm -rf ./$(ODIR)

