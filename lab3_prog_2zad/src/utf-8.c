#include <stdio.h>
#include <string.h>

#include "coder.h"
#include "command.h"
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage:\ncoder encode <in-file-name> <out-file-name>\ncoder decode <in-file-name> <out-file-name>\n");
        return 0;
    }
    const char *command = argv[1];
    const char *in_file_name = argv[2];
    const char *out_file_name = argv[3];

    if (strcmp(command, "encode") == 0)
    {
        if (encode_file(in_file_name, out_file_name) < 0)
        {
            printf("Error encode file\n");
        }
    }
    else if (strcmp(command, "decode") == 0)
    {
        if (decode_file(in_file_name, out_file_name) < 0)
        {
            printf("Error decode file\n");
        }
    }
    else
    {
        return -1;
    }
}