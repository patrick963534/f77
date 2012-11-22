void new_zip(void);
unsigned int add_file(char *filename);
unsigned int add_data(char *source, char *destination, unsigned int size, unsigned int eof);
unsigned int end_file(char *destination);
unsigned int end_zip(unsigned char *destination);

unsigned int deflate(unsigned char *source, unsigned char *destination, unsigned int bitoffset, unsigned int size, unsigned int last);

char* zip_deflate_draft_compress(const char* data, int sz, int* ret_sz);
char* zip_deflate_draft_uncompress(const char* data, int sz, int* ret_sz);