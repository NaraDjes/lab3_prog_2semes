#include <inttypes.h>
enum
{
    MaxCodeLength = 4
};
typedef struct
{
    uint8_t code[MaxCodeLength];
    size_t length;
} CodeUnit;
int encode(uint32_t code_point, CodeUnit *code_unit);
uint32_t decode(const CodeUnit *code_unit);
int read_next_code_unit(FILE *in, CodeUnit *code_unit);
int write_code_unit(FILE *out, const CodeUnit *code_unit);