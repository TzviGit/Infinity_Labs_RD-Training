#ifndef _FS_FILE_FUNCS__
#define _FS_FILE_FUNCS__


#define BASE_OFFSET 1024                   /* locates beginning of the super block (first group) */
#define VIRTUAL_FS "/dev/ram0"              /* the floppy disk device */


void PrintSuperBlock(char *fs);

void PrintGroupDesriptor(char *fs);

long GetInodeOffset(int inode_num);

void PrintFileInfo(char *virtual_disk, char *file_path);

#endif /* _FS_FILE_FUNCS__ */