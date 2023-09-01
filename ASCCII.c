#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Function to convert a single hex character to its decimal value
int hexCharToDecimal(char hex) {
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    } else if (hex >= 'A' && hex <= 'F') {
        return 10 + (hex - 'A');
    } else if (hex >= 'a' && hex <= 'f') {
        return 10 + (hex - 'a');
    } else {
        return -1; // Invalid hex character
    }
}

// Function to convert a hex string to an ASCII text string
void hexToAscii(const char *hex, char *ascii) {
    int len = strlen(hex);
    int asciiIndex = 0;

    for (int i = 0; i < len; i += 2) {
        int highNibble = hexCharToDecimal(hex[i]);
        int lowNibble = hexCharToDecimal(hex[i + 1]);

        if (highNibble == -1 || lowNibble == -1) {
            printf("Invalid hex string\n");
            return;
        }

        char asciiChar = (char)((highNibble << 4) | lowNibble);
        ascii[asciiIndex++] = asciiChar;
    }

    ascii[asciiIndex] = '\0';
}

int main() {
    char hexString[] = "476562657269742041432050524f"; // Example hex string
    char asciiString[100]; // Assuming the ASCII string won't exceed 100 characters

    hexToAscii(hexString, asciiString);
    printf("Hex: %s\nASCII: %s\n", hexString, asciiString);

    return 0;
}
