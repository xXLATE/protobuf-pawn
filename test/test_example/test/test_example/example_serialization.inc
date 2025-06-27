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
stock PackDonkey(const data[eDonkey], buffer[], &offset) {

    // Field 1: hi
    if (strlen(data[eD_Hi]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eD_Hi], buffer, offset);
    }

    // Field 2: is_cool
    if (data[eD_IsCool]) {
        EncodeTag(2, WIRE_TYPE_VARINT, buffer, offset);
        EncodeVarint(data[eD_IsCool] ? 1 : 0, buffer, offset);
    }

    // Field 3: cool_factor
    if (data[eD_CoolFactor] != 0.0) {
        EncodeTag(3, WIRE_TYPE_FIXED32, buffer, offset);
        EncodeFixed32(_:data[eD_CoolFactor], buffer, offset);
    }

}

// ========== Donkey Unpack Function ==========
// Unpack Donkey structure from byte buffer
stock UnpackDonkey(const buffer[], bufferSize, data[eDonkey]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eD_Hi][0] = 0;
    data[eD_IsCool] = false;
    data[eD_CoolFactor] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // hi
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eD_Hi], 256);
                }
            }
            case 2: {
                // is_cool
                if (wireType == WIRE_TYPE_VARINT) {
                    data[eD_IsCool] = bool:(DecodeVarint(buffer, offset, bufferSize) != 0);
                }
            }
            case 3: {
                // cool_factor
                if (wireType == WIRE_TYPE_FIXED32) {
                    data[eD_CoolFactor] = Float:DecodeFixed32(buffer, offset);
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

// ========== Funky_Monkey Pack Function ==========
// Pack Funky_Monkey structure into byte buffer
stock PackFunky_Monkey(const data[eFunky_Monkey], buffer[], &offset) {

    // Field 1: hi
    if (strlen(data[eFM_Hi]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eFM_Hi], buffer, offset);
    }

}

// ========== Funky_Monkey Unpack Function ==========
// Unpack Funky_Monkey structure from byte buffer
stock UnpackFunky_Monkey(const buffer[], bufferSize, data[eFunky_Monkey]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eFM_Hi][0] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // hi
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eFM_Hi], 256);
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
stock PackFunky(const data[eFunky], buffer[], &offset) {

    // Field 1: monkey
    // Pack nested message field monkey using ID
    if (data[eF_Monkey] != 0) {
        new tempFunky_Monkey[eFunky_Monkey];
        if (GetFunky_MonkeyById(data[eF_Monkey], funky_monkeys, maxFunky_Monkeys, tempFunky_Monkey)) {
            // Pack as length-delimited submessage
            new subBuffer[1024];
            new subOffset = 0;
            PackFunky_Monkey(tempFunky_Monkey, subBuffer, subOffset);
            EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
            EncodeVarint(subOffset, buffer, offset);
            for (new i = 0; i < subOffset; i++) {
                buffer[offset++] = subBuffer[i];
            }
        }
    }

    // Field 2: dokey
    // Pack nested message field dokey using ID
    if (data[eF_Dokey] != 0) {
        new tempDonkey[eDonkey];
        if (GetDonkeyById(data[eF_Dokey], donkeys, maxDonkeys, tempDonkey)) {
            // Pack as length-delimited submessage
            new subBuffer[1024];
            new subOffset = 0;
            PackDonkey(tempDonkey, subBuffer, subOffset);
            EncodeTag(2, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
            EncodeVarint(subOffset, buffer, offset);
            for (new i = 0; i < subOffset; i++) {
                buffer[offset++] = subBuffer[i];
            }
        }
    }

}

// ========== Funky Unpack Function ==========
// Unpack Funky structure from byte buffer
stock UnpackFunky(const buffer[], bufferSize, data[eFunky]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eF_Monkey] = 0;
    data[eF_Dokey] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // monkey
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Unpack nested message Funky_Monkey
                    new len = DecodeVarint(buffer, offset, bufferSize);
                    new startOffset = offset;
                    new tempFunky_Monkey[eFunky_Monkey];
                    UnpackFunky_Monkey(buffer[startOffset], len, tempFunky_Monkey);
                    data[eF_Monkey] = tempFunky_Monkey[eFM_Id];
                    offset += len;
                }
            }
            case 2: {
                // dokey
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Unpack nested message Donkey
                    new len = DecodeVarint(buffer, offset, bufferSize);
                    new startOffset = offset;
                    new tempDonkey[eDonkey];
                    UnpackDonkey(buffer[startOffset], len, tempDonkey);
                    data[eF_Dokey] = tempDonkey[eD_Id];
                    offset += len;
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

// ========== SomeRPCRequest Pack Function ==========
// Pack SomeRPCRequest structure into byte buffer
stock PackSomeRPCRequest(const data[eSomeRPCRequest], buffer[], &offset) {

    // Field 1: id
    if (data[eS_Id] != 0) {
        EncodeTag(1, WIRE_TYPE_VARINT, buffer, offset);
        EncodeVarint(data[eS_Id], buffer, offset);
    }

    // Field 2: name
    if (strlen(data[eS_Name]) > 0) {
        EncodeTag(2, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eS_Name], buffer, offset);
    }

}

// ========== SomeRPCRequest Unpack Function ==========
// Unpack SomeRPCRequest structure from byte buffer
stock UnpackSomeRPCRequest(const buffer[], bufferSize, data[eSomeRPCRequest]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eS_Id] = 0;
    data[eS_Name][0] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // id
                if (wireType == WIRE_TYPE_VARINT) {
                    data[eS_Id] = DecodeVarint(buffer, offset, bufferSize);
                }
            }
            case 2: {
                // name
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eS_Name], 256);
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
stock PackSomeRPCResponse(const data[eSomeRPCResponse], buffer[], &offset) {

    // Field 1: result
    if (strlen(data[eS_Result]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eS_Result], buffer, offset);
    }

}

// ========== SomeRPCResponse Unpack Function ==========
// Unpack SomeRPCResponse structure from byte buffer
stock UnpackSomeRPCResponse(const buffer[], bufferSize, data[eSomeRPCResponse]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eS_Result][0] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // result
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eS_Result], 256);
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

