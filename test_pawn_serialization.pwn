#include <core>
#include <console>

// Include generated files
#include "github.com/example/test/example_enums.inc"
#include "github.com/example/test/example_serialization.inc"

main() {
    print("=== Pawn Protobuf Serialization Test ===\n");
    
    // Test SomeRPCRequest serialization
    test_SomeRPCRequest();
    
    // Test SomeRPCResponse serialization  
    test_SomeRPCResponse();
    
    // Test Donkey serialization
    test_Donkey();
    
    print("=== All tests completed ===\n");
}

test_SomeRPCRequest() {
    print("Testing SomeRPCRequest serialization...\n");
    
    // Create test data
    new data[eSomeRPCRequest];
    data[eId] = 12345;
    strcpy(data[eName], "TestUser");
    
    // Pack data into buffer
    new buffer[256];
    new packedSize = PackSomeRPCRequest(data, buffer, sizeof(buffer));
    
    printf("Original data: id=%d, name=%s\n", data[eId], data[eName]);
    printf("Packed size: %d bytes\n", packedSize);
    
    // Print packed bytes in hex format
    printf("Packed bytes: ");
    for (new i = 0; i < packedSize; i++) {
        printf("%02X ", buffer[i] & 0xFF);
    }
    printf("\n");
    
    // Unpack data from buffer
    new unpackedData[eSomeRPCRequest];
    new success = UnpackSomeRPCRequest(buffer, packedSize, unpackedData);
    
    if (success) {
        printf("Unpacked data: id=%d, name=%s\n", unpackedData[eId], unpackedData[eName]);
        
        // Verify data integrity
        if (data[eId] == unpackedData[eId] && strcmp(data[eName], unpackedData[eName]) == 0) {
            print("✓ SomeRPCRequest test PASSED\n");
        } else {
            print("✗ SomeRPCRequest test FAILED - data mismatch\n");
        }
    } else {
        print("✗ SomeRPCRequest test FAILED - unpacking failed\n");
    }
    print("");
}

test_SomeRPCResponse() {
    print("Testing SomeRPCResponse serialization...\n");
    
    // Create test data
    new data[eSomeRPCResponse];
    strcpy(data[eResult], "Operation completed successfully");
    
    // Pack data into buffer
    new buffer[256];
    new packedSize = PackSomeRPCResponse(data, buffer, sizeof(buffer));
    
    printf("Original data: result=%s\n", data[eResult]);
    printf("Packed size: %d bytes\n", packedSize);
    
    // Print packed bytes in hex format
    printf("Packed bytes: ");
    for (new i = 0; i < packedSize; i++) {
        printf("%02X ", buffer[i] & 0xFF);
    }
    printf("\n");
    
    // Unpack data from buffer
    new unpackedData[eSomeRPCResponse];
    new success = UnpackSomeRPCResponse(buffer, packedSize, unpackedData);
    
    if (success) {
        printf("Unpacked data: result=%s\n", unpackedData[eResult]);
        
        // Verify data integrity
        if (strcmp(data[eResult], unpackedData[eResult]) == 0) {
            print("✓ SomeRPCResponse test PASSED\n");
        } else {
            print("✗ SomeRPCResponse test FAILED - data mismatch\n");
        }
    } else {
        print("✗ SomeRPCResponse test FAILED - unpacking failed\n");
    }
    print("");
}

test_Donkey() {
    print("Testing Donkey serialization...\n");
    
    // Create test data
    new data[eDonkey];
    strcpy(data[eHi], "Hello from Pawn!");
    
    // Pack data into buffer
    new buffer[256];
    new packedSize = PackDonkey(data, buffer, sizeof(buffer));
    
    printf("Original data: hi=%s\n", data[eHi]);
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
        printf("Unpacked data: hi=%s\n", unpackedData[eHi]);
        
        // Verify data integrity
        if (strcmp(data[eHi], unpackedData[eHi]) == 0) {
            print("✓ Donkey test PASSED\n");
        } else {
            print("✗ Donkey test FAILED - data mismatch\n");
        }
    } else {
        print("✗ Donkey test FAILED - unpacking failed\n");
    }
    print("");
}

// Utility function to compare strings (if not available in standard library)
stock strcmp(const str1[], const str2[]) {
    new i = 0;
    while (str1[i] == str2[i] && str1[i] != 0) {
        i++;
    }
    return str1[i] - str2[i];
}

// Utility function to copy strings (if not available in standard library)
stock strcpy(dest[], const src[]) {
    new i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return i;
} 