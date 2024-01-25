

 * ext2super.c
 *
 * Reads the super-block from a Ext2 floppy disk.
 *
 * Questions?
 * Emanuele Altieri
 * ealtieri@hampshire.edu
 */

#include <stddef.h> /* NULL */
#include "file_system_funcs.h" /* header for ext2 API */

int main(int argc, char *argv[])
{
	char *fs_name =	VIRTUAL_FS;  /* "/dev/ram0" */
	char *path = NULL;

	if (argc < 2)
	{
		return -1;
	}

	path = argv[1];
	PrintSuperBlock(fs_name);

	PrintGroupDesriptor(fs_name);

	PrintFileInfo(fs_name, path);
	return 0;
	;
} /* main() */