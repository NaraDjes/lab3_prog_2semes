#include <stdio.h>
#include <inttypes.h>
#include "command.h"
#include "coder.h"
int encode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in;
    FILE *out;

    in = fopen(in_file_name, "r");

    if (!in)
    {
        return -1;
    }

    out = fopen(out_file_name, "wb");

    if (!out)
    {
        return -1;
    }
    while (!feof(in))
    {
        uint32_t code_point;
        CodeUnit code_unit;
        fscanf(in, "%" SCNx32, &code_point);
        if (encode(code_point, &code_unit) < 0)
        {
            printf("Error encode function\n");
            return -1;
        }
        write_code_unit(out, &code_unit);
    }
    fclose(in);
    fclose(out);
    return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in;
    FILE *out;
    in = fopen(in_file_name, "r");
    if (!in)
    {
        return -1;
    }
    out = fopen(out_file_name, "wb");
    if (!out)
    {
        return -1;
    }
    CodeUnit code_unit;
    fseek(in, 0, SEEK_SET);
    fseek(in, 0, SEEK_END);
    long end_symbol = ftell(in);
    fseek(in, 0, SEEK_SET);
    while ((ftell(in)) != (end_symbol))
    {
        if ((read_next_code_unit(in, &code_unit)) == 0)
        {
            uint32_t num = decode(&code_unit);
            if (decode(&code_unit) < 0)
            {
                printf("Error decode function\n");
                return -1;
            }
            fprintf(out, "%" PRIx32 "\n", num);
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}