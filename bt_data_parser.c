#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to represent data types
struct DataType {
    unsigned char type;
    char* name;
};

struct Data {
    unsigned char id;
    char* name;
    char* info;
};

// Define data types and their corresponding names
struct DataType data_types[] = {
    {0x01, "Property ID"},
    {0x09, "Device Name"},
    {0xFF, "Vendor Info"},
};


// Function to parse and print data set
void parse_data(const unsigned char *byte_data, size_t length, struct DataType* data_types) {
    // Get the length from the first byte
    unsigned char data_length = byte_data[0];

    // Get the type from the second byte
    unsigned char data_type = byte_data[1];

    // Extract usable data
    unsigned char *usable_data = &byte_data[2];

    // Print length and type
    printf("Length: %hhu\n", data_length);

    // Find the corresponding data type
    char* data_type_name = "Unknown";
    for (int i = 0; data_types[i].type != 0xFF0; i++) {
        if (data_types[i].type == data_type) {
            data_type_name = data_types[i].name;
            break;
        }
    }

    printf("Type: %hhu (%s)\n", data_type, data_type_name);

    // Print usable data bytes
    printf("Usable Data: ");
    for (size_t i = 0; i < data_length - 2; ++i) {
        printf("%02x ", usable_data[i]);
    }
    printf("%02x\n", usable_data[data_length - 2]);
}

int main() {
    
    struct Data data = {0};
    // Example hex string
    const char *hex_string = "02010607094654303030310DFF060070CAEA80FD02AA0B1301";
    size_t hex_length = strlen(hex_string);

    // Check if hex string length is even
    if (hex_length % 2 != 0) {
        printf("Hex string length must be even.\n");
        return 1;
    }

    // Convert hex string to byte array
    size_t byte_count = hex_length / 2;
    unsigned char *byte_data = (unsigned char *)malloc(byte_count);
    for (size_t i = 0; i < byte_count; ++i) {
        sscanf(hex_string + i * 2, "%2hhx", &byte_data[i]);
    }

    // Initialize offset for parsing
    size_t offset = 0;
    while (offset < byte_count) {
        // Calculate length of current data set
        unsigned char data_length = byte_data[offset];
        
        // Parse and process the current data set
        parse_data(byte_data + offset, data_length, data_types);
        
        // Move the offset to the next data set
        offset += data_length;
        
        // Skip the first byte of the next set
        offset += 1;
    }

    // Free allocated memory
    free(byte_data);

    return 0;
}