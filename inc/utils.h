bool DirectoryExists(const char *path);

bool FileExists(char *fileName);
bool FileWriteAllBytes(const char *fileName, const void *bytes, size_t size);
unsigned long FileSize(char* fileName);

bool StringStartsWith(const char *str, const char *value);

char *PathChangeExtension(char *fileName, char *extension);
char *PathGetFileNameWithoutExtension(char *fileName);
