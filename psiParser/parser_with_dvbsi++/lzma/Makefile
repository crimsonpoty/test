INCDIRS = -I./C
DISTDIR = ../lib
MAKEDEPEND = $(CXX) -M
CFLAGS = -c -g -Wall -D_7ZIP_ST

ARFLAGS = ru
LZMALIB = lib7zlzma.a
OBJS = Alloc.o LzmaDec.o

#exe: sample.cpp
#	$(CXX) -o LzmaDec4Telefonica $(INCDIRS) sample.cpp -L. -l7zlzma 
#	cp LzmaDec4Telefonica $(MY_WORK_DIR)

all: $(LZMALIB)

Alloc.o: ./C/Alloc.c
	$(CXX) $(CFLAGS) ./C/Alloc.c

LzmaDec.o: ./C/LzmaDec.c
	$(CXX) $(CFLAGS) ./C/LzmaDec.c

clean:
	@echo cleaning workspace...
	@rm -f $(OBJS) $(LZMALIB) *~
	@rm -f Makefile.dep

depend : Makefile.dep
Makefile.dep :
	@echo "updating dependencies..."
	$(MAKEDEPEND) $(INCDIRS) $(OBJS:%.o=%.c) > Makefile.dep

new : clean depend all

dist: all
	@echo "distributing $(LZMALIB) to $(DISTDIR)..."
	@cp $(LZMALIB) $(DISTDIR)
	@cp $(INCLUDES) $(DISTINCDIR)
	@$(RANLIB) $(DISTDIR)/$(LZMALIB)

$(LZMALIB) : $(OBJS)
	@echo updating library...
	@$(AR) $(ARFLAGS) $(LZMALIB) $(OBJS)
	@$(RANLIB) $(LZMALIB)

.c.o : 
	@echo compiling $<...
	$(CXX) $(DEFINES) $(CFLAGS) $(INCDIRS) -c $<

-include Makefile.dep