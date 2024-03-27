#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
void print_bin(unsigned char x)
{
    for (int i = 0; i < 8; i++)
    {
        printf("%d", (x >> (7 - i)) & 1);
    }
    printf("\n");
}
size_t encode_varint(uint32_t value, uint8_t *buf)
{
    assert(buf != NULL);
    uint8_t *cur = buf;
    while (value >= 0x80)
    {
        const uint8_t byte = (value & 0x7f) | 0x80;
        *cur = byte;
        value >>= 7;
        ++cur;
    }
    *cur = value;
    ++cur;
    return cur - buf;
}

uint32_t decode_varint(const uint8_t **bufp)
{
    const uint8_t *cur = *bufp;
    uint8_t byte = *cur++;
    uint32_t value = byte & 0x7f;
    size_t shift = 7;
    while (byte >= 0x80)
    {
        byte = *cur++;
        value += (byte & 0x7f) << shift;
        shift += 7;
    }
    *bufp = cur;
    return value;
}
uint32_t generate_number()
{
    const int r = rand();
    const int p = r % 100;
    if (p < 90)
    {
        return r % 128;
    }
    if (p < 95)
    {
        return r % 16384;
    }
    if (p < 99)
    {
        return r % 2097152;
    }
    return r % 268435455;
}
int main()
{
    size_t size = 0;
    FILE *fp;
    FILE *unfp;
    fp = fopen("compressed.dat", "wb");
    unfp = fopen("uncompressed.dat", "wb");
    for (int i = 0; i < 1000000; i++)
    {
        uint32_t number = generate_number();
        printf("d- %d\n", number);
        fwrite(&number, sizeof(uint32_t), 1, unfp);
        uint8_t buf[4];
        size = encode_varint(number, buf);
        fwrite(buf, sizeof(uint8_t), size, fp);
        const uint8_t *cur_uncomp = buf;
        uint32_t value = decode_varint(&cur_uncomp);
        if (value != number)
        {
            printf("ERROR");
        }
    }
    fclose(fp);
    fclose(unfp);
    FILE *fpcomp = fopen("compressed.dat", "rb");
    fseek(fpcomp, 0, SEEK_END);
    long int count = ftell(fpcomp);
    fseek(fpcomp, 0, SEEK_SET);
    uint8_t compressed[2000000];
    fread(compressed, sizeof(uint8_t), count, fpcomp);
    const uint8_t *curcomp = compressed;
    while (curcomp < compressed + count)
    {
        uint32_t value = decode_varint(&curcomp);
        printf("v- %d\n", value);
    }
    fclose(fpcomp);
    return 0;
}
