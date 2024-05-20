#define HLIMIT(a,b) (((a)<(b))?(a):(b))
#define LLIMIT(a,b) (((a)>(b))?(a):(b))
#define LIMIT(a,b,c) LLIMIT(HLIMIT(a,c),b)

bool DirectoryExists(const char *path);

bool FileExists(char *fileName);
bool FileWriteAllBytes(const char *fileName, const void *bytes, size_t size);
unsigned long FileSize(char* fileName);

bool StringStartsWith(const char *str, const char *value);
unsigned int StringToIntIp(char *ip);

char *PathChangeExtension(char *fileName, char *extension);
char *PathGetFileNameWithoutExtension(char *fileName);

uint32_t GetCRC32(char *fileName);
unsigned long Crc32_ComputeBuf( unsigned long inCrc32, const void *buf, size_t bufLen );

double sqrt(double n);
