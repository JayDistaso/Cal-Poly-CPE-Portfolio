#include "minlsget.h"

void print_usageInformation();

int main(int argc, char *argv[]) {

   FILE *fp = NULL;
   int count, verbose = 0, usingPartition = 0, usingSubpartition = 0,
    offset = 0, primaryPartition = 0, subPartition = 0, 
    /*root directroy (default) inode is 1*/
    currentInode = 1,
    /*This is two because it is used to add links for . and ..,
     root directory (default) is a directory)*/
    pathWasDirectory = 1, zoneSize;
   
   char *path = NULL, *imageFilename = NULL, *parsedPath = NULL,
    *originalPath = NULL;
   Superblock *superblock = NULL;
   Inode *inodes = NULL;
   Directory *filesInDirectory = NULL;
   char filename[MAX_FILENAME];
   
   /*Parse arguments*/
   for (count = 1; count < argc; count++) {
      /*Check if a flag*/

      if (argv[count][0] == '-') {
         /*Not checking validity hard 
         If user uses undefined syntax they get what they deserve*/
         
         if (argv[count][1] == 'v') {
            verbose = 1;
         }
         else if (argv[count][1] == 'p') {
            usingPartition = 1;
            primaryPartition = strtol(argv[++count], NULL, 10);
            if (primaryPartition > 4 || primaryPartition < 0) {
               fprintf(stderr, "Partition %d out of range.  M"
               "ust be 0..3.\n", primaryPartition);
               print_usageInformation();
               exit(EXIT_FAILURE);
            }
            /*If there is a subpartition there should be at
             least 3 arguments from the -p
             ie. -s # imagefile
             We advance the count as we consume the flag values
             */
            if (argc >= count + 3 && argv[count + 1][0] == '-'
             && argv[count + 1][1] == 's') {
               usingSubpartition = 1;
               subPartition = strtol(argv[count + 2], NULL, 10);
               count += 2;
            }
         }
         else if (argv[count][1] == 'h') {
            print_usageInformation();
            exit(EXIT_FAILURE);
         }
         else {
            fprintf(stderr, "Invalid Flag: %s\n", argv[count]);
            exit(EXIT_FAILURE);
         } 
      }
      else {
         /*Not a flag so we are done with flags*/
         break;
      }
   }
   /*One thing left (imagefile)*/
   if (argc - count == 1) {
      path = "/";
      imageFilename = argv[count];
   }
   /*Two Things left (imagefile then path)*/
   else if (argc - count == 2) {
      imageFilename = argv[count];
      path = argv[count + 1];
   }
   /*Wrong number of things left*/
   else {
      fprintf(stderr, "Bad command line argument\n");
      exit(EXIT_FAILURE);
   }
   
   /*People do not want us to sribble over their files, open in READ-ONLY*/

   if (!(fp = fopen(imageFilename, "r"))) {
      perror("fopen");
      exit(EXIT_FAILURE);
   }
   
   /*We need to get an offset if using a partition*/   
   if (usingPartition) {
      offset = get_partition_offset(primaryPartition, fp, verbose, offset);
   }
   if (usingSubpartition) {
      offset = get_partition_offset(subPartition, fp, verbose, offset);
   }
   
   if (!(superblock = malloc(sizeof(Superblock)))) {
      perror("malloc superblock");
      exit(EXIT_FAILURE);
   }   
   
   /*Get the superblock info*/
   get_Superblock(superblock, fp, offset, verbose);
   if (!(inodes = malloc(sizeof(Inode) * superblock->ninodes *
    superblock->ninodes))) {
      perror("malloc inodes");
      exit(EXIT_FAILURE);
   }

   /*Get the inode information*/
   get_Inodes(inodes, superblock, fp, offset, verbose);

   zoneSize = superblock->blocksize << superblock->log_zone_size;
   
   if (!(filesInDirectory = malloc(zoneSize))) {
      perror("malloc filesInDirectory");
      exit(EXIT_FAILURE);
   }
   
   /*Get directory information*/
   get_Directory(filesInDirectory, superblock, inodes[0].zone[0], fp,
    offset); 

   /*Save a copy of path before tokenizing*/
   originalPath = calloc(sizeof(char), strlen(path) + 1);
   strcpy(originalPath, path);
   parsedPath = strtok(path, PATH_DELIMITER);

   /*We need to traverse through path*/
   while (parsedPath) {
      
      currentInode = get_fileInode(parsedPath, filesInDirectory,
       inodes[currentInode - 1].links);
      
      if (currentInode == -1) {
         fprintf(stderr, "File at %s could not be found\n", originalPath);
         exit(1);
      }
      
      /*Check if its a directory*/
      if (inodes[currentInode - 1].mode & DIRECTORY_MASK) {
         
         /*We want to know if the end of path was a directory or a file*/ 
         pathWasDirectory = 1;
         
         get_Directory(filesInDirectory, superblock,
          inodes[currentInode - 1].zone[0], fp, offset);

         
      }
      /*Not a directory*/
      else {
      /*Print the one file out here*/
         print_permissions(&inodes[currentInode - 1]);
         printf("%10u %s\n",
          inodes[currentInode - 1].size, originalPath);
         pathWasDirectory = 0;
      }
      parsedPath = strtok(NULL, PATH_DELIMITER);
   }
   
   if (pathWasDirectory) {
      printf("%s:\n", originalPath);
   }
   
   /*Print all files in directory*/
   for (count = 0; count < inodes[currentInode - 1].size /
    sizeof(Directory) && pathWasDirectory; count++) {
      if (filesInDirectory[count].inode == 0) {
         continue;
      }

      if (!filesInDirectory[count].name) {
         break;
      }

      strncpy(filename, (char *)filesInDirectory[count].name, 60);

      /*Make sure its null terminated*/
      filename[60] = '\0'; 

      print_permissions(&inodes[filesInDirectory[count].inode - 1]);

      printf("%10u %s\n",
       inodes[filesInDirectory[count].inode - 1].size, filename);
       
   } 
   return 0;
}

void print_usageInformation() {
   fprintf(stderr, "usage minls [ -v] [ -p part [ -s subpart ] ] ima"
   "gfile [ path ]\n");
   fprintf(stderr, "Options:\n");
   fprintf(stderr, "\t-p\t part\t --- select partition for filesyst"
   "em (default: none)\n");
   fprintf(stderr, "\t-s\t sub\t --- select subpartition for filesyst"
   "em (default: none)\n");
   fprintf(stderr, "\t-h\t help\t --- print usage information and exit\n");
   fprintf(stderr, "\t-v\t verbose --- increase verbosity level\n");
}
