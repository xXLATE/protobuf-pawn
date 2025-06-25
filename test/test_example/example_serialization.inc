// Protobuf serialization functions for Pawn
// Generated from: test/example.proto

// ========== Protobuf Utility Functions ==========

// Encode varint value to buffer
stock EncodeVarint(value, buffer[], &offset) {
    new temp = value;
    while (temp >= 0x80) {
        buffer[offset++] = (temp & 0x7F) | 0x80;
        temp >>>= 7;
    }
    buffer[offset++] = temp & 0x7F;
}

// Decode varint value from buffer
stock DecodeVarint(const buffer[], &offset, maxOffset) {
    new result = 0;
    new shift = 0;
    new byte;
    while (offset < maxOffset) {
        byte = buffer[offset++];
        result |= (byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7;
        if (shift >= 32) break; // Prevent overflow
    }
    return result;
}

// Encode 32-bit fixed value to buffer
stock EncodeFixed32(value, buffer[], &offset) {
    buffer[offset++] = value & 0xFF;
    buffer[offset++] = (value >>> 8) & 0xFF;
    buffer[offset++] = (value >>> 16) & 0xFF;
    buffer[offset++] = (value >>> 24) & 0xFF;
}

// Decode 32-bit fixed value from buffer
stock DecodeFixed32(const buffer[], &offset) {
    new result = buffer[offset] |
        (buffer[offset + 1] << 8) |
        (buffer[offset + 2] << 16) |
        (buffer[offset + 3] << 24);
    offset += 4;
    return result;
}

// Encode 64-bit fixed value to buffer
stock EncodeFixed64(value, buffer[], &offset) {
    // Note: Pawn doesn't have native 64-bit support, treating as float
    new low = _:value;
    new high = 0; // Upper 32 bits set to 0 for simplicity
    buffer[offset++] = low & 0xFF;
    buffer[offset++] = (low >>> 8) & 0xFF;
    buffer[offset++] = (low >>> 16) & 0xFF;
    buffer[offset++] = (low >>> 24) & 0xFF;
    buffer[offset++] = high & 0xFF;
    buffer[offset++] = (high >>> 8) & 0xFF;
    buffer[offset++] = (high >>> 16) & 0xFF;
    buffer[offset++] = (high >>> 24) & 0xFF;
}

// Decode 64-bit fixed value from buffer
stock Float:DecodeFixed64(const buffer[], &offset) {
    // Note: Pawn doesn't have native 64-bit support, reading only lower 32 bits
    new result = buffer[offset] |
        (buffer[offset + 1] << 8) |
        (buffer[offset + 2] << 16) |
        (buffer[offset + 3] << 24);
    offset += 8; // Skip all 8 bytes
    return Float:result;
}

// Encode string to buffer
stock EncodeString(const str[], buffer[], &offset) {
    new len = strlen(str);
    EncodeVarint(len, buffer, offset);
    for (new i = 0; i < len; i++) {
        buffer[offset++] = str[i];
    }
}

// Decode string from buffer
stock DecodeString(const buffer[], &offset, maxOffset, dest[], maxlen) {
    new len = DecodeVarint(buffer, offset, maxOffset);
    if (len >= maxlen) len = maxlen - 1;
    for (new i = 0; i < len && offset < maxOffset; i++) {
        dest[i] = buffer[offset++];
    }
    dest[len] = 0;
    return len;
}

// Protobuf wire types
#define WIRE_TYPE_VARINT    0
#define WIRE_TYPE_FIXED64   1
#define WIRE_TYPE_LENGTH_DELIMITED 2
#define WIRE_TYPE_FIXED32   5

// Encode field tag (field number + wire type)
stock EncodeTag(fieldNumber, wireType, buffer[], &offset) {
    EncodeVarint((fieldNumber << 3) | wireType, buffer, offset);
}

// Decode field tag
stock DecodeTag(const buffer[], &offset, maxOffset, &fieldNumber, &wireType) {
    new tag = DecodeVarint(buffer, offset, maxOffset);
    fieldNumber = tag >>> 3;
    wireType = tag & 0x7;
    return tag;
}

// ========== Donkey Pack Function ==========
// Pack Donkey structure into byte buffer
stock PackDonkey(const data[eDonkey], buffer[], maxSize) {
    new offset = 0;

    // Field 1: hi
    if (strlen(data[eHi]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eHi], buffer, offset);
    }

    // Field 2: is_cool
    if (data[eIsCool]) {
        EncodeTag(2, WIRE_TYPE_VARINT, buffer, offset);
        EncodeVarint(data[eIsCool] ? 1 : 0, buffer, offset);
    }

    // Field 3: cool_factor
    if (data[eCoolFactor] != 0.0) {
        EncodeTag(3, WIRE_TYPE_FIXED32, buffer, offset);
        EncodeFixed32(_:data[eCoolFactor], buffer, offset);
    }

    return offset; // Return total bytes written
}

// ========== Donkey Unpack Function ==========
// Unpack Donkey structure from byte buffer
stock UnpackDonkey(const buffer[], bufferSize, data[eDonkey]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eHi][0] = 0;
    data[eIsCool] = false;
    data[eCoolFactor] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // hi
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eHi], 256);
                }
            }
            case 2: {
                // is_cool
                if (wireType == WIRE_TYPE_VARINT) {
                    data[eIsCool] = bool:(DecodeVarint(buffer, offset, bufferSize) != 0);
                }
            }
            case 3: {
                // cool_factor
                if (wireType == WIRE_TYPE_FIXED32) {
                    data[eCoolFactor] = Float:DecodeFixed32(buffer, offset);
                }
            }
            default: {
                // Unknown field, skip it
                switch (wireType) {
                    case WIRE_TYPE_VARINT: DecodeVarint(buffer, offset, bufferSize);
                    case WIRE_TYPE_FIXED32: offset += 4;
                    case WIRE_TYPE_FIXED64: offset += 8;
                    case WIRE_TYPE_LENGTH_DELIMITED: {
                        new len = DecodeVarint(buffer, offset, bufferSize);
                        offset += len;
                    }
                }
            }
        }
    }
    return 1; // Success
}

// ========== Funky Pack Function ==========
// Pack Funky structure into byte buffer
stock PackFunky(const data[eFunky], buffer[], maxSize) {
    new offset = 0;

    // Field 1: monkey
    // TODO: Nested message packing for Funky_Monkey

    // Field 2: dokey
    // TODO: Nested message packing for Donkey

    return offset; // Return total bytes written
}

// ========== Funky Unpack Function ==========
// Unpack Funky structure from byte buffer
stock UnpackFunky(const buffer[], bufferSize, data[eFunky]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eMonkey] = 0;
    data[eDokey] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // monkey
                // TODO: Nested message unpacking for Funky_Monkey
            }
            case 2: {
                // dokey
                // TODO: Nested message unpacking for Donkey
            }
            default: {
                // Unknown field, skip it
                switch (wireType) {
                    case WIRE_TYPE_VARINT: DecodeVarint(buffer, offset, bufferSize);
                    case WIRE_TYPE_FIXED32: offset += 4;
                    case WIRE_TYPE_FIXED64: offset += 8;
                    case WIRE_TYPE_LENGTH_DELIMITED: {
                        new len = DecodeVarint(buffer, offset, bufferSize);
                        offset += len;
                    }
                }
            }
        }
    }
    return 1; // Success
}

// ========== SomeRPCRequest Pack Function ==========
// Pack SomeRPCRequest structure into byte buffer
stock PackSomeRPCRequest(const data[eSomeRPCRequest], buffer[], maxSize) {
    new offset = 0;

    // Field 1: id
    if (data[eId] != 0) {
        EncodeTag(1, WIRE_TYPE_VARINT, buffer, offset);
        EncodeVarint(data[eId], buffer, offset);
    }

    // Field 2: name
    if (strlen(data[eName]) > 0) {
        EncodeTag(2, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eName], buffer, offset);
    }

    return offset; // Return total bytes written
}

// ========== SomeRPCRequest Unpack Function ==========
// Unpack SomeRPCRequest structure from byte buffer
stock UnpackSomeRPCRequest(const buffer[], bufferSize, data[eSomeRPCRequest]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eId] = 0;
    data[eName][0] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // id
                if (wireType == WIRE_TYPE_VARINT) {
                    data[eId] = DecodeVarint(buffer, offset, bufferSize);
                }
            }
            case 2: {
                // name
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eName], 256);
                }
            }
            default: {
                // Unknown field, skip it
                switch (wireType) {
                    case WIRE_TYPE_VARINT: DecodeVarint(buffer, offset, bufferSize);
                    case WIRE_TYPE_FIXED32: offset += 4;
                    case WIRE_TYPE_FIXED64: offset += 8;
                    case WIRE_TYPE_LENGTH_DELIMITED: {
                        new len = DecodeVarint(buffer, offset, bufferSize);
                        offset += len;
                    }
                }
            }
        }
    }
    return 1; // Success
}

// ========== SomeRPCResponse Pack Function ==========
// Pack SomeRPCResponse structure into byte buffer
stock PackSomeRPCResponse(const data[eSomeRPCResponse], buffer[], maxSize) {
    new offset = 0;

    // Field 1: result
    if (strlen(data[eResult]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eResult], buffer, offset);
    }

    return offset; // Return total bytes written
}

// ========== SomeRPCResponse Unpack Function ==========
// Unpack SomeRPCResponse structure from byte buffer
stock UnpackSomeRPCResponse(const buffer[], bufferSize, data[eSomeRPCResponse]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eResult][0] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // result
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eResult], 256);
                }
            }
            default: {
                // Unknown field, skip it
                switch (wireType) {
                    case WIRE_TYPE_VARINT: DecodeVarint(buffer, offset, bufferSize);
                    case WIRE_TYPE_FIXED32: offset += 4;
                    case WIRE_TYPE_FIXED64: offset += 8;
                    case WIRE_TYPE_LENGTH_DELIMITED: {
                        new len = DecodeVarint(buffer, offset, bufferSize);
                        offset += len;
                    }
                }
            }
        }
    }
    return 1; // Success
}

