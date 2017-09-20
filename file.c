/**
 * @file
 * @author Igor Lesik 2017
 *
 */

typedef struct File
{
    const char* path;

} File;

#include <string.h>
#include <sys/xattr.h>

//https://linux.die.net/man/2/setxattr
int File_setXAttrStr(
    const File* file,
    const char* attr_name,
    const char* attr_value_str
)
{
    int result = setxattr(
        file->path,
        attr_name,
        attr_value_str,
        strlen(attr_value_str) + 1,
        XATTR_CREATE | XATTR_REPLACE
    );


    return result;
}
