#include <stdio.h>
#include <inttypes.h>
#include "coder.h"
#include "command.h"
int encode(uint32_t code_point, CodeUnit *code_unit)
{
    if (code_point < 0x80)
    {
        code_unit->length = 1;
        code_unit->code[0] = code_point;
    }
    else if (code_point < 0x800)
    {
        code_unit->length = 2;
        code_unit->code[0] = 0xc0 | (code_point >> 6);
        code_unit->code[1] = 0x80 | (code_point & 0x3f);
    }
    else if (code_point < 0x10000)
    {
        code_unit->length = 3;
        code_unit->code[0] = 0xe0 | (code_point >> 12);
        code_unit->code[1] = 0x80 | ((code_point >> 6) & 0x3f);
        code_unit->code[2] = 0x80 | (code_point & 0x3f);
    }
    else if (code_point < 0x200000)
    {
        code_unit->length = 4;
        code_unit->code[0] = 0xf0 | (code_point >> 18);
        code_unit->code[1] = 0x80 | ((code_point >> 12) & 0x3f);
        code_unit->code[2] = 0x80 | ((code_point >> 6) & 0x3f);
        code_unit->code[3] = 0x80 | (code_point & 0x3f);
    }
    else
    {
        return -1;
    }
    return 0;
}

uint32_t decode(const CodeUnit *code_unit)
{
    uint32_t number;
    if ((code_unit->length) == 1)
    {
        number = code_unit->code[0];
        return number;
    }
    else if ((code_unit->length) == 2)
    {
        number = (((code_unit->code[0]) & 0x1f) << 6) | ((code_unit->code[1]) & 0x3f);
        return number;
    }
    else if ((code_unit->length) == 3)
    {
        number = ((((code_unit->code[0]) & 0xf) << 12) | (((code_unit->code[1]) & 0x3f) << 6) | ((code_unit->code[2]) & 0x3f));
        return number;
    }
    else if (code_unit->length == 4)
    {
        number = (((((code_unit->code[0]) & 0x7) << 18) | (((code_unit->code[1]) & 0x3f) << 12) | ((code_unit->code[2]) & 0x3f) << 6) | ((code_unit->code[3]) & 0x3f));
        return number;
    }
    return 0;
}

int read_next_code_unit(FILE *in, CodeUnit *code_unit)
{
    uint8_t buf = 0;
    fread(&buf, sizeof(uint8_t), 1, in);
    if ((buf >> 7) == 0)
    {
        code_unit->code[0] = buf;
        code_unit->length = 1;
    }
    else if ((buf >> 5) == 0x06)
    {
        code_unit->code[0] = buf;
        fread(&buf, sizeof(uint8_t), 1, in);
        code_unit->code[1] = buf;
        code_unit->length = 2;
    }
    else if ((buf >> 4) == 0x0e)
    {
        code_unit->code[0] = buf;
        fread(&buf, sizeof(uint8_t), 1, in);
        code_unit->code[1] = buf;
        fread(&buf, sizeof(uint8_t), 1, in);
        code_unit->code[2] = buf;
        code_unit->length = 3;
    }
    else if ((buf >> 3) == 0x1e)
    {
        code_unit->code[0] = buf;
        fread(&buf, sizeof(uint8_t), 1, in);
        code_unit->code[1] = buf;
        fread(&buf, sizeof(uint8_t), 1, in);
        code_unit->code[2] = buf;
        fread(&buf, sizeof(uint8_t), 1, in);
        code_unit->code[3] = buf;
        code_unit->length = 4;
    }
    return 0;
}

int write_code_unit(FILE *out, const CodeUnit *code_unit)
{
    int result = fwrite(code_unit->code, 1, code_unit->length, out);
    return result;
}