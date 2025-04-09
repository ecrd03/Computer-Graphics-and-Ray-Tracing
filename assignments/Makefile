CC = gcc
LIBS = /usr/lib/x86_64-linux-gnu/libcunit.so.1.0.1 -lm 
CFLAGS = -I. -g -w -I./..
TDIR = ./src/tuple
CDIR = ./src/canvas
RDIR = ./src/ray
SDIR = ./src/surfaces
HDIR = ./src/hits
BINDIR = ./bin
TESTDIR = ./src/tests
PPMDIR = ./ppm

all: tupletest canvastest raytest hitstest rendertest

tupletest: $(TDIR)/tuple.* $(TESTDIR)/tupletest.c
	$(CC) $(CFLAGS) $(TESTDIR)/tupletest.c \
	$(TDIR)/tuple.c $(LIBS) -o $(BINDIR)/tupletest

canvastest: $(TDIR)/tuple.* $(CDIR)/canvas.* \
	        $(TESTDIR)/canvastest.c 
	$(CC) $(CFLAGS) $(TESTDIR)/canvastest.c \
	$(TDIR)/tuple.c $(CDIR)/canvas.c \
	$(LIBS) -o $(BINDIR)/canvastest 

raytest: $(SDIR)/sphere.* $(TDIR)/tuple.* \
		 $(CDIR)/canvas.* $(RDIR)/ray.* $(TESTDIR)/raytest.c 
	$(CC) $(CFLAGS) $(TESTDIR)/raytest.c $(SDIR)/sphere.c \
	$(TDIR)/tuple.c $(CDIR)/canvas.c \
	$(RDIR)/ray.c $(LIBS) -o $(BINDIR)/raytest

hitstest: $(SDIR)/sphere.* $(HDIR)/hits.* $(RDIR)/ray.* \
          $(TESTDIR)/hitstest.c $(TDIR)/tuple.*
	$(CC) $(CFLAGS) $(TESTDIR)/hitstest.c $(SDIR)/sphere.c \
	$(TDIR)/tuple.c $(HDIR)/hits.c \
	$(RDIR)/ray.c $(LIBS) -o $(BINDIR)/hitstest

rendertest: $(SDIR)/sphere.* $(HDIR)/hits.* $(RDIR)/ray.* \
          $(TESTDIR)/rendertest.c $(CDIR)/canvas.* $(TDIR)/tuple.*
	$(CC) $(CFLAGS) $(SDIR)/sphere.c $(CDIR)/canvas.c \
	$(TDIR)/tuple.c $(HDIR)/hits.c $(TESTDIR)/rendertest.c  \
	$(RDIR)/ray.c $(LIBS) -o $(BINDIR)/rendertest

clean:
	rm -f $(BINDIR)/tupletest $(TDIR)/*.orig \
	  $(BINDIR)/canvastest $(CDIR)/*.orig \
	  $(BINDIR)/raytest $(RDIR)/*.orig \
	  $(BINDIR)/hitstest $(HDIR)/*.orig \
	  $(BINDIR)/rendertest \
	  $(PPMDIR)/*.ppm
	