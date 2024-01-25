

#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "file_sys.h"
#include "file_system_funcs.h"

static void read_inode(int fd, int inode_no, const struct ext2_group_desc *group, struct ext2_inode *inode);
static void FillAndVerifySuperBlock(int fd, struct ext2_super_block *super);
static void read_dir(int fd, char *file_path, struct ext2_inode *inode, const struct ext2_group_desc *group);
static void read_file(int fd, const struct ext2_inode *inode);

/* static void *MapFileSysSToRam(int fd); */

static int g_block_size = 0;
#define BLOCK_OFFSET(block) ((block)* g_block_size)
void PrintSuperBlock(char *fs)
{
    struct ext2_super_block super = {0};
    
    int fd;
    
    if ((fd = open(fs, O_SYNC | O_RDONLY)) < 0)
    {
		perror(fs);
		exit(1);  /* error while opening the floppy device */
	}

    FillAndVerifySuperBlock(fd, &super);

    close(fd);

    

	printf("Reading super-block from device %s :\n"
			"Magic number           : %u\n"
	       "Inodes count            : %u\n"
	       "Blocks count            : %u\n"
	       "Reserved blocks count   : %u\n"
	       "Free blocks count       : %u\n"
	       "Free inodes count       : %u\n"
	       "First data block        : %u\n"
	       "Block size              : %u\n"
	       "Blocks per group        : %u\n"
	       "Inodes per group        : %u\n"
	       "Creator OS              : %u\n"
	       "First non-reserved inode: %u\n"
	       "Size of inode structure : %hu\n"
	       ,
           fs,
		   super.s_magic,
	       super.s_inodes_count,  
	       super.s_blocks_count,
	       super.s_r_blocks_count,     /* reserved blocks count */
	       super.s_free_blocks_count,
	       super.s_free_inodes_count,
	       super.s_first_data_block,
	       g_block_size,
	       super.s_blocks_per_group,
	       super.s_inodes_per_group,
	       super.s_creator_os,
	       super.s_first_ino,          /* first non-reserved inode */
	       super.s_inode_size);
	

}

static void FillAndVerifySuperBlock(int fd, struct ext2_super_block *super)
{
    /* read super-block */

	lseek(fd, BASE_OFFSET, SEEK_SET); 
	read(fd, super, sizeof(*super));
    

    if (super->s_magic != EXT2_SUPER_MAGIC)
    {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        close(fd);
        exit(1);
    }
    
    g_block_size = 1024 << super->s_log_block_size;
}


void PrintGroupDesriptor(char *fs)
{
    struct ext2_super_block super={0};
    struct ext2_group_desc group = {0};
    int fd;

    if ((fd = open(fs, O_SYNC| O_RDONLY)) < 0) {
		perror(fs);
		exit(1);  /* error while opening the floppy device */
	}

    FillAndVerifySuperBlock(fd, &super);

    lseek(fd,  g_block_size , SEEK_SET);
	read(fd, &group, sizeof(group));

	close(fd);
    
    printf("Reading first group-descriptor from device %s :\n"
		   "Blocks bitmap block: %u\n"
		   "Inodes bitmap block: %u\n"
		   "Inodes table block : %u\n"
		   "Free blocks count  : %u\n"
		   "Free inodes count  : %u\n"
		   "Directories count  : %u\n",
           fs,
		   group.bg_block_bitmap,
		   group.bg_inode_bitmap,
		   group.bg_inode_table,
		   group.bg_free_blocks_count,
		   group.bg_free_inodes_count,
		   group.bg_used_dirs_count); /* directories count */

}

void PrintFileInfo(char *virtual_disk, char *file_path) 
{
    struct ext2_super_block super = {0};
	struct ext2_group_desc group = {0};
	struct ext2_inode inode = {0};
	
	int fd = 0;
	char *token = NULL;


	/* open device */

	if ((fd = open(virtual_disk, O_RDONLY)) < 0) {
		perror(virtual_disk);
		exit(1);  /* error while opening the device */
	}

	/* read super-block */

	FillAndVerifySuperBlock(fd, &super);

	
	/* PrintSuperBlock(virtual_disk, &super); */

	/* read group descriptor */

	lseek(fd, g_block_size, SEEK_SET);
	read(fd, &group, sizeof(group));

	/* PrintGroupDescriptor(virtual_disk, &group); */

	/* read root directory */
	read_inode(fd, 2, &group, &inode);
	
	token = strtok(file_path, "/");
	while (S_ISDIR(inode.i_mode))
	{
		read_dir(fd, token, &inode, &group);
		token = strtok(NULL, "/");
	}

	read_file(fd, &inode);

	close(fd);
}

/* static void *MapFileSysSToRam(int fd)
{
    void *map = NULL;
    struct stat file_stats;

	if (-1 == fstat(fd, &file_stats))
	{
        return NULL;
    }

    map = mmap(NULL, file_stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (MAP_FAILED == map)
	{
		close(fd);
		exit(1);
	}

	return map;
} */

static void read_inode(int fd, int inode_no, const struct ext2_group_desc *group, struct ext2_inode *inode)
{
	lseek(fd, BLOCK_OFFSET(group->bg_inode_table)+(inode_no - 1) * sizeof(struct ext2_inode), SEEK_SET);
	read(fd, inode, sizeof(struct ext2_inode));
} /* read_inode() */

/* static size_t BLOCK_OFFSET(size_t block_sequence_number)
{
	return block_sequence_number * g_block_size;
} */

static void read_dir(int fd, char *file_path, struct ext2_inode *inode, const struct ext2_group_desc *group)
{
	void *block = NULL;

	struct ext2_dir_entry_2 *entry = NULL;
	unsigned int size = 0;

	if ((block = malloc(g_block_size)) == NULL) { /* allocate memory for the data block */
		fprintf(stderr, "Memory error\n");
		close(fd);
		exit(1);
	}

	lseek(fd, BLOCK_OFFSET(inode->i_block[0]), SEEK_SET);
	read(fd, block, g_block_size);                /* read block from disk*/

	entry = (struct ext2_dir_entry_2 *) block;  /* first entry in the directory */
			/* Notice that the list may be terminated with a NULL
				entry (entry->inode == NULL)*/
	while((size < inode->i_size) && entry->inode) 
	{
		char file_name[EXT2_NAME_LEN+1];
		memcpy(file_name, entry->name, entry->name_len);
		file_name[entry->name_len] = 0;     /* append null character to the file name */
		
		if (!strcmp(file_path, file_name))
		{
			break;
		}

		entry = (void*) entry + entry->rec_len;
		size += entry->rec_len;
	}

	read_inode(fd, entry->inode, group, inode);

	free(block);

} /* read_dir() */

static void read_file(int fd, const struct ext2_inode *inode)
{
	char *block = NULL;
	int i;

	if ((block = malloc(g_block_size)) == NULL) { /* allocate memory for the data block */
		fprintf(stderr, "Memory error\n");
		close(fd);
		exit(1);
	}

	for (i = 0; i < EXT2_NDIR_BLOCKS && inode->i_block[i]; i++)
	{

		lseek(fd, BLOCK_OFFSET((inode->i_block[i])), SEEK_SET);
		read(fd, block, g_block_size);
		printf("\nReading file content:\n%s", block);
	}

	free(block);
}
