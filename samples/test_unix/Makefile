CC=gcc
AR=ar
CFLAGS=-I.
OBJDIR=obj
LIBDIR=lib
OBJS=$(OBJDIR)/http.o $(OBJDIR)/test.o

all: objs libhttp.a test

dirs: $(OBJDIR) $(LIBDIR)
	@printf "Creating directories\n"

$(OBJDIR):
	@mkdir $@

$(LIBDIR):
	@mkdir $@

objs: dirs $(OBJS) 

$(OBJS): $(OBJDIR)/%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@
	@printf "Compiling $@\n"

libhttp.a: objs
	@$(AR) rcs $(LIBDIR)/$@ $(OBJDIR)/http.o
	@printf "Creating library $@\n" 

test: objs libhttp.a
	@$(CC) -o $@ $(OBJDIR)/test.o $(LIBDIR)/libhttp.a $(CFLAGS)
	@printf "Linking final executable\n"

.PHONY: clean

clean:
	@rm -rf lib obj test
