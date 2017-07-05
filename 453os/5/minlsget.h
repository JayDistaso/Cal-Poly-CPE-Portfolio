#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>

#define SUPER_BLOCK_OFFSET 1024
#define LOCATION_OF_PARTITION_TABLE 0x1BE
#define SIGNATURE_BYTE_ONE 510
#define VALID_BYTE_510 0x55
#define SIGNATURE_BYTE_TWO 511
#define VALID_BYTE_511 0xAA
#define PARTITION_TYPE_FOR_MINIX 0x81
#define MINIX_MAGIC_NUMBER 0x4D5A
#define SIZE_OF_INODE_IN_BYTES 64
#define SIZE_OF_DIRECTORY_ENTRY_IN_BYTES 64
#define SECTOR_SIZE 512
#define DIRECT_ZONES 7
#define LENGTH_OF_MINIX_FILENAME 60
#define PATH_DELIMITER "/"
#define DIRECTORY_MASK 0040000
#define REGULAR_FILE 0100000
#define SYM_LINK 01200000
#define OWNER_READ_PERMISSION 0000400
#define OWNER_WRITE_PERMISSION 0000200
#define OWNER_EXECUTE_PERMISSION 0000100
#define GROUP_READ_PERMISSION 0000040
#define GROUP_WRITE_PERMISSION 0000020
#define GROUP_EXECUTE_PERMISSION 0000010
#define OTHER_READ_PERMISSION 0000004
#define OTHER_WRITE_PERMISSION 0000002
#define OTHER_EXECUTE_PERMISSION 0000001
#define MAX_FILENAME 61
#define DEV_ZERO "/dev/zero"

/*Struct definitions*/

typedef struct superblock { /* Minix Version 3 Superblock
* this structure found in fs/super.h
* in minix 3.1.1
*/

/* on disk. These fields and orientation are non-negotiable */
   uint32_t ninodes;       /* number of inodes in this filesystem */
   uint16_t pad1;          /* make things line up properly */
   int16_t i_blocks;       /* # of blocks used by inode bit map */
   int16_t z_blocks;       /* # of blocks used by zone bit map */
   uint16_t firstdata;     /* number of first data zone */
   int16_t log_zone_size;  /* log2 of blocks per zone */
   int16_t pad2;           /* make things line up again */
   uint32_t max_file;      /* maximum file size */
   uint32_t zones;         /* number of zones on disk */
   int16_t magic;          /* magic number */
   int16_t pad3;           /* make things line up again */
   uint16_t blocksize;     /* block size in bytes */
   uint8_t subversion;     /* filesystem sub-version */
   
} Superblock;

typedef struct partition_table {
    uint8_t  bootind;    /* Boot magic number (0x80 if bootable) */
    uint8_t  start_head; /* Start of partition in CHS */
    uint8_t  start_sec;
    uint8_t  start_cyl;
    uint8_t  type;       /* Type of partition (0x81 is MINIX) */
    uint8_t  end_head;   /* End of partition in CHS */
    uint8_t  end_sec;
    uint8_t  end_cyl;
    uint32_t lFirst;     /* First sector (LBA addressing) */
    uint32_t size;       /* Size of partition (in sectors) */
} PartitionEntry;

typedef struct inode {
   uint16_t mode; /* mode */
   uint16_t links; /* number of links */
   uint16_t uid;
   uint16_t gid;
   uint32_t size;
   int32_t atime;
   int32_t mtime;
   int32_t ctime;
   uint32_t zone[DIRECT_ZONES];
   uint32_t indirect;
   uint32_t two_indirect;
   uint32_t unused;
} Inode;

typedef struct directory {
   uint32_t inode; /*inode number*/
   unsigned char name[LENGTH_OF_MINIX_FILENAME]; /*filename string*/
} Directory;

/*Function definitions*/
unsigned long get_partition_offset (int partition, FILE *imageFile,
 int verbose, unsigned long offset);

void parse_partition(PartitionEntry *partitions, FILE *imageFile,
 unsigned long offset, int verbose);

void print_Partition(PartitionEntry *partition);

void get_Superblock(Superblock *superblock, FILE *imageFile,
 unsigned offset, int verbose);

void print_superblock(Superblock *superblock);

void get_Inodes(Inode *inodes, Superblock *superblock, FILE *fp,
 int offset, int verbose);

void print_inodes(Inode *inodes);

void get_Directory(Directory *filesInDirectory, Superblock *superblock,
 int zone, FILE *fp, unsigned long offset);

int get_fileInode(char *parsedPath, Directory *filesInDirectory, int links);

void print_permissions(Inode *inode);

/*Gets offset given partition*/
unsigned long get_partition_offset (int partition, FILE *imageFile,
 int verbose, unsigned long offset) {
   PartitionEntry partitions[4];
   
   /*Make sure partition is valid number*/
   if (partition < 0 || partition > 3) {
      printf("Invalid primary partition number: %d\n", partition);
      exit(EXIT_FAILURE);
   }
   
   /*Make sure partition is valid*/
   parse_partition(partitions, imageFile, verbose, offset);
   
   /*Check for minix partition type*/
   if (partitions[partition].type != PARTITION_TYPE_FOR_MINIX) {
      fprintf(stderr, "Partition type is not minix type\n");
      fprintf(stderr, "Partition type for minix is (%#04x),"
       " this partition is (%#04x)\n", PARTITION_TYPE_FOR_MINIX, 
       partitions[partition].type);
      exit(EXIT_FAILURE);
   }
   
   /*Calculate offset with partition info*/
   offset = partitions[partition].lFirst * SECTOR_SIZE;
      
   if (verbose) {
      print_Partition(&(partitions[partition]));
   } 
   
   return offset;
}

/*Checks if partition is valid*/
void parse_partition(PartitionEntry *partitions, FILE *imageFile,
 unsigned long verbose, int offset) {
   
   int count;
   u_int8_t byte[2];
   
   /*Check signature bytes to see if valid partition table*/
   
   if ((fseek(imageFile, SIGNATURE_BYTE_ONE + offset, SEEK_SET)) == -1) {
      perror("fseek partition");
      exit(EXIT_FAILURE);
   }
   if (!fread(byte, 1, 2, imageFile)) {
      perror("fread");
      exit(EXIT_FAILURE);
   }
   
   /*Checking minix magic numbers for validity*/
   if (byte[0] != VALID_BYTE_510) {
      printf("Bad magic number. (%#04x)\n", byte[0]);
      exit(EXIT_FAILURE);
   }
   if (byte[1] != VALID_BYTE_511) {
      printf("Bad magic number. (%#04x)\n", byte[1]);
      exit(EXIT_FAILURE);
   }
   
   /*If valid we read in the partitions*/
   if ((fseek(imageFile, LOCATION_OF_PARTITION_TABLE +
    offset, SEEK_SET)) == -1) {
      perror("fseek");
      exit(EXIT_FAILURE);
   }
   if (!fread(partitions, sizeof(PartitionEntry), 4, imageFile)) {
      perror("fread");
      exit(EXIT_FAILURE);
   }
    
}

/*Prints partion info for verbose option*/
void print_Partition(PartitionEntry *partition) {
   printf("\tbootind:    %16x\n", partition->bootind);
   printf("\tstart_head: %16u\n", partition->start_head);
   printf("\tstart_sec:  %16u\n", partition->start_sec);
   printf("\tstart_cyl:  %16u\n", partition->start_cyl);
   printf("\ttype:       %16x\n", partition->type);
   printf("\tend_head:   %16u\n", partition->end_head);
   printf("\tend_sec:    %16u\n", partition->end_sec);
   printf("\tend_cyl:    %16u\n", partition->end_cyl);
   printf("\tlFirst:     %16u\n", partition->lFirst);
   printf("\tsize:       %16u\n", partition->size);
}

/*Gets the super block information*/
void get_Superblock(Superblock *superblock, FILE *imageFile,
 unsigned offset, int verbose) {
 
 /*Super block always in same spot*/
   if ((fseek(imageFile, SUPER_BLOCK_OFFSET + offset, SEEK_SET)) == -1) {
      perror("fseek superblock");
      exit(EXIT_FAILURE);
   }
   if (!fread(superblock, sizeof(Superblock), 1, imageFile)) {
      perror("fread superblock");
      exit(EXIT_FAILURE);
   }
   
   /*Check super blocks magic number*/
   if (superblock->magic != MINIX_MAGIC_NUMBER) {
      fprintf(stderr, "superblock magic number (%#04x)" 
       " does not match minix magic number (%#04x)\n", superblock->magic,
       MINIX_MAGIC_NUMBER);
      exit(EXIT_FAILURE);
   }
   
   if (verbose) {
      print_superblock(superblock);
   }   
   
}

/*Prints ther superblock information for verbose option*/
void print_superblock(Superblock *superblock) {
      printf("\nSuperblock Contents:\n");
      printf("Stored Fields:\n");
      printf("  nidodes\t%8u\n", superblock->ninodes);
      printf("  i_blocks\t%8u\n", superblock->i_blocks);
      printf("  z_blocks\t%8u\n", superblock->z_blocks);
      printf("  firstdata\t%8u\n", superblock->firstdata);
      printf("  log_zone_size%8u\n", superblock->log_zone_size);
      printf("  max_file\t%8u\n", superblock->max_file);
      printf("  magic\t%#04x\n", superblock->magic);
      printf("  zones\t%8u\n", superblock->zones);
      printf("  blocksize\t%8u\n", superblock->blocksize);
      printf("  subversion\t%8u\n", superblock->subversion);
}

/*Gtes inode information*/
void get_Inodes(Inode *inodes, Superblock *superblock, FILE *fp,
 int offset, int verbose) {
 
   int inodeOffset = 0;
   
   /*Inode offset*/
   inodeOffset = superblock->blocksize * (superblock->i_blocks +
    superblock->z_blocks + 2); 
   
   //inodes = malloc(sizeof(Inode) * superblock->ninodes);
 
   if (fseek(fp, inodeOffset + offset, SEEK_SET) == -1) {
      perror("fseek inodes");
      exit(EXIT_FAILURE);
   }
   
   /*Read in inodes*/
   if (!fread(inodes, sizeof(Inode) * superblock->ninodes,
    superblock->ninodes, fp)) {
      perror("fread inodes");
      exit(EXIT_FAILURE);
   }
   if (verbose) {
      print_inodes(inodes);
   }
   
}

/*Prints inode information for verbose option*/
void print_inodes(Inode *inodes) {
   printf("File inode:\n");
   /*need to parse for permisisons so we can print them*/
   printf("\tunsigned short mode\t   %#04x\t(", inodes->mode);
   print_permissions(inodes);

   printf(")\n\tunsigned short links\t %8u\n", inodes->links);
   printf("\tunsigned short uid\t %8u\n", inodes->uid);
   printf("\tunsigned short gid\t %8u\n", inodes->gid);
   printf("\tuint32_t size\t %16u\n", inodes->size);
   printf("\tuint32_t atime\t %16u\n", inodes->atime);
   printf("\tuint32_t mtime\t %16u\n", inodes->mtime);
   printf("\tuint32_t ctime\t %16u\n", inodes->ctime);
   
   printf("Direct Zones:\n");
   printf("\t zone[0]:\t %16u\n", inodes->zone[0]);   
   printf("\t zone[1]:\t %16u\n", inodes->zone[1]);   
   printf("\t zone[2]:\t %16u\n", inodes->zone[2]);   
   printf("\t zone[3]:\t %16u\n", inodes->zone[3]);   
   printf("\t zone[4]:\t %16u\n", inodes->zone[4]);   
   printf("\t zone[5]:\t %16u\n", inodes->zone[5]);   
   printf("\t zone[6]:\t %16u\n", inodes->zone[6]);   
   
   printf("\tuint32_t indirect%16u\n", inodes->indirect);
   printf("\tuint32_t double\t %16u\n", inodes->two_indirect);
}

/*Gets a directory ptr*/
void get_Directory(Directory *filesInDirectory, Superblock *superblock,
 int zone, FILE *fp, unsigned long offset) {
 
   int zoneSize = superblock->blocksize << superblock->log_zone_size;
   
   if ((fseek(fp, (zoneSize * zone) + offset, SEEK_SET) == -1)) {
      perror("fseek directory");
      exit(EXIT_FAILURE);
   }
   if (!fread(filesInDirectory, zoneSize, 1, fp)) {
      perror("fread");
      exit(EXIT_FAILURE);
   }
}

/*Gets a files inode number*/
int get_fileInode(char *parsedPath, Directory *filesInDirectory, int links) {
   
   int index, nonZeroInodes = 0;
   for (index = 0; nonZeroInodes < SECTOR_SIZE; index++) {
      
      /*Inodes with zero value are deleted files, not our file*/
      if (!filesInDirectory[index].inode) {
         continue;
      } 
      
      nonZeroInodes++;
      if (strcmp((char *)filesInDirectory[index].name, parsedPath) == 0) {
         return filesInDirectory[index].inode;
      }
   }
   /*signal if we couldnt find it*/
   return -1; 
}

/*Prints the permission string*/
void print_permissions(Inode *inode) {
   char *permissionOutput;
   if (!(permissionOutput = malloc(sizeof(char) * 11))) {
      perror("malloc permissionOutput");
      exit(EXIT_FAILURE);
   }
   permissionOutput[0] = (inode->mode & DIRECTORY_MASK) ? 'd' : '-';
   permissionOutput[1] = (inode->mode & OWNER_READ_PERMISSION) ? 'r' : '-';
   permissionOutput[2] = (inode->mode & OWNER_WRITE_PERMISSION) ? 'w' : '-';
   permissionOutput[3] = (inode->mode & OWNER_EXECUTE_PERMISSION) ? 'x' : '-';
   permissionOutput[4] = (inode->mode & GROUP_READ_PERMISSION) ? 'r' : '-';
   permissionOutput[5] = (inode->mode & GROUP_WRITE_PERMISSION) ? 'w' : '-';
   permissionOutput[6] = (inode->mode & GROUP_EXECUTE_PERMISSION) ? 'x' : '-';
   permissionOutput[7] = (inode->mode & OTHER_READ_PERMISSION) ? 'r' : '-';
   permissionOutput[8] = (inode->mode & OTHER_WRITE_PERMISSION) ? 'w' : '-';
   permissionOutput[9] = (inode->mode & OTHER_EXECUTE_PERMISSION) ? 'x' : '-';
   permissionOutput[10] = '\0';
   printf("%s", permissionOutput);
}



