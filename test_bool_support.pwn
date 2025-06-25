#include <core>
#include <console>

// Include generated files
#include "test/test_example/example_enums.inc"
#include "test/test_example/example_serialization.inc"

main() {
    print("=== Testing Bool Support in Pawn Protobuf ===\n");
    
    test_DonkeyWithBool();
    
    print("=== Bool support test completed ===\n");
}

test_DonkeyWithBool() {
    print("Testing Donkey serialization with bool and float fields...\n");
    
    // Create test data with all new fields
    new data[eDonkey];
    strcpy(data[eHi], "Hello with bool!");
    data[eIsCool] = true;          // Test bool field
    data[eCoolFactor] = Float:3.14159; // Test float field
    
    // Pack data into buffer
    new buffer[256];
    new packedSize = PackDonkey(data, buffer, sizeof(buffer));
    
    printf("Original data:\n");
    printf("  hi: %s\n", data[eHi]);
    printf("  is_cool: %s\n", data[eIsCool] ? "true" : "false");
    printf("  cool_factor: %.5f\n", data[eCoolFactor]);
    printf("Packed size: %d bytes\n", packedSize);
    
    // Print packed bytes in hex format
    printf("Packed bytes: ");
    for (new i = 0; i < packedSize; i++) {
        printf("%02X ", buffer[i] & 0xFF);
    }
    printf("\n");
    
    // Unpack data from buffer
    new unpackedData[eDonkey];
    new success = UnpackDonkey(buffer, packedSize, unpackedData);
    
    if (success) {
        printf("Unpacked data:\n");
        printf("  hi: %s\n", unpackedData[eHi]);
        printf("  is_cool: %s\n", unpackedData[eIsCool] ? "true" : "false");
        printf("  cool_factor: %.5f\n", unpackedData[eCoolFactor]);
        
        // Verify data integrity
        new stringMatch = (strcmp(data[eHi], unpackedData[eHi]) == 0);
        new boolMatch = (data[eIsCool] == unpackedData[eIsCool]);
        new floatMatch = (floatabs(data[eCoolFactor] - unpackedData[eCoolFactor]) < 0.001);
        
        if (stringMatch && boolMatch && floatMatch) {
            print("✓ Donkey with bool/float test PASSED\n");
        } else {
            print("✗ Donkey with bool/float test FAILED - data mismatch\n");
            printf("  String match: %s\n", stringMatch ? "OK" : "FAIL");
            printf("  Bool match: %s\n", boolMatch ? "OK" : "FAIL");
            printf("  Float match: %s\n", floatMatch ? "OK" : "FAIL");
        }
    } else {
        print("✗ Donkey test FAILED - unpacking failed\n");
    }
    print("");
    
    // Test with false bool value
    test_DonkeyWithFalseBool();
}

test_DonkeyWithFalseBool() {
    print("Testing Donkey with false bool value...\n");
    
    // Create test data with false bool
    new data[eDonkey];
    strcpy(data[eHi], "Not cool");
    data[eIsCool] = false;
    data[eCoolFactor] = Float:0.0;
    
    // Pack data into buffer
    new buffer[256];
    new packedSize = PackDonkey(data, buffer, sizeof(buffer));
    
    printf("Original: is_cool=%s, cool_factor=%.1f\n", 
        data[eIsCool] ? "true" : "false", data[eCoolFactor]);
    printf("Packed size: %d bytes\n", packedSize);
    
    // Unpack and verify
    new unpackedData[eDonkey];
    new success = UnpackDonkey(buffer, packedSize, unpackedData);
    
    if (success) {
        printf("Unpacked: is_cool=%s, cool_factor=%.1f\n", 
            unpackedData[eIsCool] ? "true" : "false", unpackedData[eCoolFactor]);
        
        if (!unpackedData[eIsCool] && unpackedData[eCoolFactor] == 0.0) {
            print("✓ False bool test PASSED\n");
        } else {
            print("✗ False bool test FAILED\n");
        }
    } else {
        print("✗ False bool test FAILED - unpacking failed\n");
    }
    print("");
}

// Utility functions
stock strcmp(const str1[], const str2[]) {
    new i = 0;
    while (str1[i] == str2[i] && str1[i] != 0) {
        i++;
    }
    return str1[i] - str2[i];
}

stock strcpy(dest[], const src[]) {
    new i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return i;
}

stock Float:floatabs(Float:value) {
    if (value < 0.0) {
        return -value;
    }
    return value;
} 