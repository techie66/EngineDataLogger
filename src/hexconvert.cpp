#include "hexconvert.h"

int char2int(char input)
{
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}

/**
 * \brief Convert ASCII hex to binary.
 *
 * This function assumes src to be a zero terminated string formatted as
 * '0xXX' of variable length, and target to be sufficiently large
 * \param src null-terminatead ASCII HEX
 * \param target byte buffer large enough to hold result
*/
void hex2bin(const char *src, unsigned char *target)
{
  if (src[0] == '0' && src[1] == 'x') {
    char *input = strdup(src);
    char *input_front = input;
    if ( strlen(input) % 2 == 0 ) {
      input += 2;
    } else {
      input++;
      input[0] = '0';
    }
    while (input[0] && input[1]) {
      *(target++) = char2int(input[0]) * 0x10 + char2int(input[1]);
      input += 2;
    }
    free(input_front);
    return;
  }
  throw std::invalid_argument("Invalid input string");
}
