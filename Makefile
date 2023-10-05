# WHICH COMPILER SHOULD BE USED?
COMPILER = gcc 

# WHERE ARE THE SOURCE FILES?
SIMPATH = src/

# WHAT FILES ARE NEEDED FOR COMPILATION?
FILELIST = $(SIMPATH)elf_reader/elf_reader.c $(SIMPATH)utils/heap.c $(SIMPATH)RegFile.c $(SIMPATH)Syscall.c $(SIMPATH)PROC.c 

# RUN ON 'make'
MEMU: 
	$(COMPILER) $(FILELIST) -o eMIPS

# RUN ON 'make clean'
clean:
	rm -rf eMIPS stdout.txt stderr.txt

