#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

bool DirectoryExists(const char *path)
{
    struct stat info;
    if (stat(path, &info ) != 0) return false;
    else if (info.st_mode & S_IFDIR) return true;
    return false;
}

bool FileExists(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

bool FileWriteAllBytes(const char *fileName, const void *bytes, size_t size)
{
    FILE *file = fopen(fileName, "wb");
    if (file)
    {
        size_t dataWritten = fwrite(bytes, sizeof(char), size, file);
        fclose(file);
        if (dataWritten == size) return true;
    }
    return false;
}

unsigned long FileSize(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        unsigned long len = (unsigned long)ftell(file);
        fclose(file);
        return len;
    }
    return 0;
}

bool StringStartsWith(const char *str, const char *value)
{
    size_t lstr = strlen(str), lvalue = strlen(value);
    return lstr < lvalue ? false : strncmp(value, str, lvalue) == 0;
}

char *PathChangeExtension(char *fileName, char *extension)
{
    static char result[512];
    strcpy(result, fileName);
    
    char *pFile = strrchr(result, '\\');
    pFile = pFile == NULL ? result : pFile+1;
    // change extension
    char *pExt = strrchr(pFile, '.');
    if (pExt != NULL) strcpy(pExt, extension);
    else strcat(pFile, extension);

    return result;
}

char *PathGetFileNameWithoutExtension(char *fileName)
{
    static char result[512];
    strcpy(result, fileName);
    
    char *pFile = strrchr(result, '\\');
    pFile = pFile == NULL ? result : pFile+1;
    // remove extension
    char *pExt = strrchr(pFile, '.');
    if (pExt != NULL) strcpy(pExt, "");

    return result;
}
