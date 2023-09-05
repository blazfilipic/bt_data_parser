#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to represent data types
struct DataType {
    unsigned char type;
    char* name;
};

// Define a structure for Vendor Info
struct VendorInfo {
    int food_temperature;
    int max_possible_temperature;
    int battery_voltage;
};

struct Data {
    unsigned char id;
    char* name;
    char* info;
    struct VendorInfo* vendor_info; // Add a pointer to VendorInfo
};

// Define data types and their corresponding names
struct DataType data_types[] = {
    {0x01, "Property ID"},
    {0x06, "RandomShit"},
    {0x09, "Device Name"},
    {0xFF, "Vendor Info"},
};

// Function to parse and print data set
void parse_data(unsigned char *byte_data, size_t length, struct DataType* data_types, struct Data* data_array, size_t* data_count) {
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

    // Check if the data type is 0xFF (Vendor Info)
    if (data_type == 0xFF) {
        if (data_length < 8) {
            printf("Invalid 0xFF data format.\n");
            return;
        }
        // Create a VendorInfo structure and populate it
        struct VendorInfo* vendor_info = (struct VendorInfo*)malloc(sizeof(struct VendorInfo));
        vendor_info->food_temperature = (usable_data[1] << 8) | usable_data[0];
        printf("Food Temperature: %04X \n", vendor_info->food_temperature);
        printf("Food Temperature: %d \n", vendor_info->food_temperature);
        vendor_info->max_possible_temperature = (usable_data[3] << 8) | usable_data[2];
        printf("Max Temperature: %04X \n", vendor_info->max_possible_temperature);
        printf("Max Temperature: %d \n", vendor_info->max_possible_temperature);
        vendor_info->battery_voltage = (usable_data[5] << 8) | usable_data[4];
        printf("Battery voltage: %04X \n", vendor_info->battery_voltage);
        printf("Battery voltage: %d \n", vendor_info->battery_voltage);
        /*etc*/

        // Update the Data structure with VendorInfo
        data_array[*data_count].vendor_info = vendor_info;
        (*data_count)++;
    }

    // Print usable data bytes
    /*printf("Usable Data: ");
    for (size_t i = 0; i < data_length - 2; ++i) {
        printf("%02x", usable_data[i]);
    }
    printf("%02x\n", usable_data[data_length - 2]);*/
}

int main() {
    struct Data data_array[100]; // Assuming a maximum of 100 data sets
    size_t data_count = 0;

    // Example hex string
    const char *hex_string = "020106110603CA7994F75310962242118A81FA00000909697670733074725817FFF1036FF22052C0000000000A0577B8411900000000";
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
        parse_data(byte_data + offset, data_length, data_types, data_array, &data_count);

        // Move the offset to the next data set
        offset += data_length;

        // Skip the first byte of the next set
        offset += 1;
    }

    // Free allocated memory for vendor_info
    for (size_t i = 0; i < data_count; i++) {
        if (data_array[i].vendor_info != NULL) {
            free(data_array[i].vendor_info);
        }
    }

    // Free allocated memory
    free(byte_data);

    return 0;
}
