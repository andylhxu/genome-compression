# makefile for Watson genome reference compressor

CC = g++
CPPFLAGS = -O3
# CPPFLAGS = 
SOURCES = w_demo_v3.cpp ChromNames.cpp DifferenceMap.cpp Compressor.cpp PMF_DPL_Compressor.cpp \
		BoolFiles.cpp ArithmeticCoderC.cpp ModelI.cpp ModelOrder0C.cpp ArithmeticCodingWrapper.cpp
DEP = ChromNames.h DifferenceMap.h Compressor.h PMF_DPL_Compressor.h BoolFiles.h ArithmeticCoderC.h ModelI.h ModelOrder0C.h \
		ArithmeticCodingWrapper.h HuffmanWrapper.h huffman.h Hist.h Hist.cpp tools.h HuffmanWrapper.cpp comp_by_chrom.cpp uncomp_by_chrom.cpp ConvertJWBSubmissionToShorterForm.h
OBJ = $(SOURCES:.cpp=.o)
# LDIR = lib/mtrand/
TARGETNAME = ../comp_uncomp

# .cpp.o:
	# $(CC) -c $< -o $@

# %.o: %.c $(DEPS)
	# gcc -c -o $@ $<

# all: $(OBJ) $(TARGETNAME) $(DEP) $(SOURCES)

$(TARGETNAME): $(OBJ)
	$(CC) $(OBJ) -o $@ $(CPPFLAGS)

# .cpp.o:
	# $(COMPILE.cpp) -o $<

$(OBJ): $(DEP)

.PHONY: all

all: $(OBJ)
	$(CC) comp_by_chrom.cpp $(OBJ) -o comp_by_chrom $(CPPFLAGS)
	$(CC) uncomp_by_chrom.cpp $(OBJ) -o uncomp_by_chrom $(CPPFLAGS)

.PHONY: comp

comp: $(OBJ)
	$(CC) comp_by_chrom.cpp $(OBJ) -o comp_by_chrom $(CPPFLAGS)
	
.PHONY: uncomp

uncomp: $(OBJ)
	$(CC) uncomp_by_chrom.cpp $(OBJ) -o uncomp_by_chrom $(CPPFLAGS)

.PHONY: echo

echo:
	@echo 'src:' $(SOURCES)
	@echo 'obj:' $(OBJ)
	@echo 'dep:' $(DEP)

.PHONY: clean

clean:
	-rm -f $(OBJ) $(TARGETNAME)
	
.PHONY: tar

tar:
	tar -czf code_demo.tar $(SOURCES) $(DEP) makefile
