// Template to complete the loadfunc program which locates a function
// in the text section of an ELF file. Sections that start with a
// CAPITAL in their comments require additions and modifications to
// complete the program (unless marked as PROVIDED).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

// macro to add a byte offset to a pointer
#define PTR_PLUS_BYTES(ptr,off) ((void *) (((size_t) (ptr)) + ((size_t) (off))))

// address at which to map the ELF file
#define MAP_ADDRESS  ((void *) 0x0000600000000000)

int main(int argc, char **argv){
  // PROVIDED: command line checks for proper # args
  if(argc < 2){                 
    printf("usage: %s <file>\n",argv[0]);
    return 0;
  }

  // PROVIDED: required command line arguments
  char *objfile_name = argv[1]; // name of file to operate  on
                        
  // PROVIDED: open file to get file descriptor, determine file size
  int fd = open(argv[1], O_RDONLY);                  // open file to get file descriptor
  struct stat stat_buf;         // call fstat() to fill in fields like size of file
  fstat(fd, &stat_buf);                            // ??? 
  int file_size = stat_buf.st_size;            // ???

  // CREATE memory map via mmap() call, ensure that pages are readable
  // AND executable. Map to virtual address MAP_ADDRESS, first arg to mmap().
  char *file_bytes = mmap(MAP_ADDRESS, file_size, PROT_READ | PROT_EXEC, MAP_SHARED, fd, 0);                    // pointer to file contents

  // CHECK for failure in memory map, print message and return 1 if
  // failed; otherwise, print pointer value of memory map
  if(file_bytes == MAP_FAILED){ /// ???
    printf("ERROR: failed to mmap() file %s\n",objfile_name);
    return 1;
  }
  printf("file_bytes at: %p\n",file_bytes);

  // CREATE A POINTER to the intial bytes of the file which are an ELF64_Ehdr struct
  Elf64_Ehdr *ehdr = PTR_PLUS_BYTES(file_bytes, 0); /// ???

  // CHECK e_ident field's bytes 0 to for for the sequence {0x7f,'E','L','F'}.
  // Exit the program with code 1 if the bytes do not match
  int magic_match = ehdr->e_ident[EI_MAG0] == 0x7f && ehdr->e_ident[EI_MAG1] == 'E' && ehdr->e_ident[EI_MAG2] == 'L' &&  ehdr->e_ident[EI_MAG3] == 'F';

  if(!magic_match){
    printf("ERROR: Magic bytes wrong, this is not an ELF file\n");
    return 1;
  }

  // PROVIDED: check for a 64-bit file
  if(ehdr->e_ident[EI_CLASS] != ELFCLASS64){
    printf("ERROR: Not a 64-bit file ELF file\n");
    return 1;
  }

  // PROVIDED: check for x86-64 architecture
  if(ehdr->e_machine != EM_X86_64){
    printf("ERROR: Not an x86-64 file\n");
    return 1;
  }
  // could check hear for ehdr->e_ident[EI_OSABI] for ELFOSABI_LINUX


  // SET UP a pointer to the array of section headers.  Determine the
  // offset of the Section Header Array (e_shoff) and the number of
  // sections (e_shnum). These fields are from the ELF File
  // Header. The print accroding to the format below
  Elf64_Shdr *shdr =  PTR_PLUS_BYTES(file_bytes, ehdr->e_shoff); /// ???
  printf("Section Headers Found:\n");
  printf("- %lu bytes from start of file\n", ehdr->e_shoff); /// ???
  printf("- %hu sections total\n",ehdr->e_shnum); /// ???
  printf("- %p section header virtual address\n", shdr); /// ???

  // SET UP a pointer to the Section Header String Table
  // .shstrtab. Find the its section index in the ELF header with the
  // fiel (e_shstrndx).  The index into the array of section headers
  // to find the position in the file and set up a pointer to it. See
  // the spec diagram for a visual representation.
  uint16_t shstrndx = ehdr->e_shstrndx; /// ???
  char *shstrab = PTR_PLUS_BYTES(file_bytes, shdr[shstrndx].sh_offset); /// ???
  printf("Section Header Names in Section %d\n", shstrndx); /// ???
  printf("- %lu bytes from start of file\n",shdr[shstrndx].sh_offset); /// ???
  printf("- %lu total bytes\n", shdr[shstrndx].sh_size); /// ???
  printf("- %p .shstrtab virtual address\n", shstrab); /// ???

  // SEARCH the Section Header Array for sections with names .symtab
  // (symbol table) and .strtab (string table).  Note their positions
  // in the file (sh_offset field).  Also note the size in bytes
  // (sh_size) and and the size of each entry (sh_entsize) for .symtab
  // so its number of entries can be computed.
  Elf64_Shdr *symtab_sh = NULL;
  Elf64_Shdr *strtab_sh = NULL;

  printf("\n");
  printf("Scanning Section Headers for Relevant Sections\n");
  for(int i=0; i< ehdr->e_shnum; i++){   /// ???
    char *secname = PTR_PLUS_BYTES(shstrab, shdr[i].sh_name);
    printf("[%2d]: %s\n",i,secname);
    if(strcmp(secname, ".symtab") == 0){
      symtab_sh = &shdr[i];
    }
    if(strcmp(secname, ".strtab") == 0){
      strtab_sh = &shdr[i];
    }
  }

  printf("\n");

  // CHECK that the symbol table was found; if not, error out. SET UP
  // a pointer to the .symtab section and print information as shown.
  if(symtab_sh == NULL){
    printf("ERROR: Couldn't find symbol table\n");
    return 1;
  }
  uint64_t symtab_num = symtab_sh->sh_size / symtab_sh->sh_entsize; /// ???
  Elf64_Sym *symtab = PTR_PLUS_BYTES(file_bytes, symtab_sh->sh_offset); /// ???
  printf(".symtab located\n");
  printf("- %lu bytes from start of file\n",symtab_sh->sh_offset); /// ???
  printf("- %lu bytes total size\n", symtab_sh->sh_size); /// ???
  printf("- %lu bytes per entry\n", symtab_sh->sh_entsize); /// ???
  printf("- %lu number of entries\n", symtab_num); /// ???
  printf("- %p .symtab virtual addres\n", symtab); /// ???


  // CHECK that .strtab (string table) section is found. Error out if
  // not. SET UP a pointer to it and print information as shown.
  if(strtab_sh == NULL){
    printf("ERROR: Couldn't find .strtab section\n");
    return 1;
  }
  char *strtab = PTR_PLUS_BYTES(file_bytes, strtab_sh->sh_offset);
  printf(".strtab located\n");
  printf("- %lu bytes from start of file\n", strtab_sh->sh_offset); /// ???
  printf("- %lu total bytes in section\n",  strtab_sh->sh_size); /// ???
  printf("- %p .strtab virtual addres\n", strtab); /// ???

  printf("\n");
  printf("SYMBOL TABLE CONTENTS\n");
  printf("[%3s]  %8s %4s %s\n","idx","TYPE","SIZE","NAME");

  // ITERATE through the symbol table (.symtab section), an array of
  // Elf64_Sym structs. Print the contents of each entry according to
  // the following format:
  // 
  // SYMBOL TABLE CONTENTS
  // [idx]      TYPE SIZE NAME
  // [  0]:   NOTYPE    0 <NONE>
  // [  1]:     FILE    0 x.c
  // [  2]:  SECTION    0 <NONE>
  // [  3]:  SECTION    0 <NONE>
  // [  4]:  SECTION    0 <NONE>
  // [  5]:  SECTION    0 <NONE>
  // [  6]:  SECTION    0 <NONE>
  // [  7]:  SECTION    0 <NONE>
  // [  8]:   OBJECT  512 arr
  // [  9]:     FUNC   17 func
  for(uint64_t i=0; i<symtab_num; i++){
    unsigned char typec = ELF64_ST_TYPE(symtab[i].st_info);
    char* type;
    if(typec == STT_NOTYPE){
      type = "NOTYPE";
    }
    else if(typec == STT_OBJECT){
      type = "OBJECT";
    }
    else if(typec == STT_FUNC){
      type = "FUNC";
    }
    else if(typec == STT_FILE){
      type = "FILE";
    }
    else if(typec == STT_SECTION){
      type = "SECTION";
    }
    if(strlen(&strtab[symtab[i].st_name]) > 0) {
      printf("[%3ld]: %8s %4ld %s\n",i, type, symtab[i].st_size, &strtab[symtab[i].st_name]);
    }
    else{
      printf("[%3ld]: %8s %4ld <NONE>\n",i, type, symtab[i].st_size);
    }

  }

  printf("\n");

  // Close open file and unmap mmap()'d memory
  munmap(file_bytes, file_size);
  close(fd);

  return 0;
}
