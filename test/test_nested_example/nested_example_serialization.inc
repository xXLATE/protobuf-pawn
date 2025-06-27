// Protobuf serialization functions for Pawn
// Generated from: test/nested_example.proto

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

// ========== Address Pack Function ==========
// Pack Address structure into byte buffer
stock PackAddress(const data[eAddress], buffer[], &offset) {

    // Field 1: street
    if (strlen(data[eA_Street]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eA_Street], buffer, offset);
    }

    // Field 2: city
    if (strlen(data[eA_City]) > 0) {
        EncodeTag(2, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eA_City], buffer, offset);
    }

    // Field 3: zip_code
    if (data[eA_ZipCode] != 0) {
        EncodeTag(3, WIRE_TYPE_VARINT, buffer, offset);
        EncodeVarint(data[eA_ZipCode], buffer, offset);
    }

}

// ========== Address Unpack Function ==========
// Unpack Address structure from byte buffer
stock UnpackAddress(const buffer[], bufferSize, data[eAddress]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eA_Street][0] = 0;
    data[eA_City][0] = 0;
    data[eA_ZipCode] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // street
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eA_Street], 256);
                }
            }
            case 2: {
                // city
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eA_City], 256);
                }
            }
            case 3: {
                // zip_code
                if (wireType == WIRE_TYPE_VARINT) {
                    data[eA_ZipCode] = DecodeVarint(buffer, offset, bufferSize);
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

// ========== Person_PhoneNumber Pack Function ==========
// Pack Person_PhoneNumber structure into byte buffer
stock PackPerson_PhoneNumber(const data[ePerson_PhoneNumber], buffer[], &offset) {

    // Field 1: number
    if (strlen(data[ePP_Number]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[ePP_Number], buffer, offset);
    }

    // Field 2: type
    if (strlen(data[ePP_Type]) > 0) {
        EncodeTag(2, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[ePP_Type], buffer, offset);
    }

}

// ========== Person_PhoneNumber Unpack Function ==========
// Unpack Person_PhoneNumber structure from byte buffer
stock UnpackPerson_PhoneNumber(const buffer[], bufferSize, data[ePerson_PhoneNumber]) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[ePP_Number][0] = 0;
    data[ePP_Type][0] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // number
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[ePP_Number], 256);
                }
            }
            case 2: {
                // type
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[ePP_Type], 256);
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

// ========== Person Pack Function ==========
// Pack Person structure into byte buffer
stock PackPerson(const data[ePerson], buffer[], &offset, const repeatedStrings[][E_REPEATED_STRING], maxRepeatedStrings, const addresss[][eAddress], maxAddresss, const person_phonenumbers[][ePerson_PhoneNumber], maxPerson_PhoneNumbers) {

    // Field 1: name
    if (strlen(data[eP_Name]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eP_Name], buffer, offset);
    }

    // Field 2: age
    if (data[eP_Age] != 0) {
        EncodeTag(2, WIRE_TYPE_VARINT, buffer, offset);
        EncodeVarint(data[eP_Age], buffer, offset);
    }

    // Field 3: address
    // Pack nested message field address using ID
    if (data[eP_AddressId] != 0) {
        new tempAddress[eAddress];
        if (GetAddressById(data[eP_AddressId], addresss, maxAddresss, tempAddress)) {
            // Pack as length-delimited submessage
            new subBuffer[1024];
            new subOffset = 0;
            PackAddress(tempAddress, subBuffer, subOffset);
            EncodeTag(3, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
            EncodeVarint(subOffset, buffer, offset);
            for (new i = 0; i < subOffset; i++) {
                buffer[offset++] = subBuffer[i];
            }
        }
    }

    // Field 4: emails
    // Pack repeated string array using IDs
    for (new i = 0; i < sizeof(data[eP_EmailsRSId]); i++) {
        if (data[eP_EmailsRSId][i] != 0) {
            new tempString[256];
            if (GetRepeatedStringById(data[eP_EmailsRSId][i], repeatedStrings, maxRepeatedStrings, tempString)) {
                EncodeTag(4, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
                EncodeString(tempString, buffer, offset);
            }
        }
    }

    // Field 5: lucky_numbers
    // Pack repeated int array directly
    for (new i = 0; i < sizeof(data[eP_LuckyNumbers]); i++) {
        if (data[eP_LuckyNumbers][i] != 0) {
            EncodeTag(5, WIRE_TYPE_VARINT, buffer, offset);
            EncodeVarint(data[eP_LuckyNumbers][i], buffer, offset);
        }
    }

    // Field 6: old_addresses
    // Pack repeated message field old_addresses using ID array
    for (new i = 0; i < sizeof(data[eP_OldAddressesId]); i++) {
        if (data[eP_OldAddressesId][i] != 0) {
            new tempAddress[eAddress];
            if (GetAddressById(data[eP_OldAddressesId][i], addresss, maxAddresss, tempAddress)) {
                // Pack as length-delimited submessage
                new subBuffer[1024];
                new subOffset = 0;
                PackAddress(tempAddress, subBuffer, subOffset);
                EncodeTag(6, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
                EncodeVarint(subOffset, buffer, offset);
                for (new j = 0; j < subOffset; j++) {
                    buffer[offset++] = subBuffer[j];
                }
            }
        }
    }

    // Field 7: phone_numbers
    // Pack repeated message field phone_numbers using ID array
    for (new i = 0; i < sizeof(data[eP_PhoneNumbersId]); i++) {
        if (data[eP_PhoneNumbersId][i] != 0) {
            new tempPerson_PhoneNumber[ePerson_PhoneNumber];
            if (GetPerson_PhoneNumberById(data[eP_PhoneNumbersId][i], person_phonenumbers, maxPerson_PhoneNumbers, tempPerson_PhoneNumber)) {
                // Pack as length-delimited submessage
                new subBuffer[1024];
                new subOffset = 0;
                PackPerson_PhoneNumber(tempPerson_PhoneNumber, subBuffer, subOffset);
                EncodeTag(7, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
                EncodeVarint(subOffset, buffer, offset);
                for (new j = 0; j < subOffset; j++) {
                    buffer[offset++] = subBuffer[j];
                }
            }
        }
    }

}

// ========== Person Unpack Function ==========
// Unpack Person structure from byte buffer
stock UnpackPerson(const buffer[], bufferSize, data[ePerson], repeatedStrings[][E_REPEATED_STRING], &maxRepeatedStrings, addresss[][eAddress], &maxAddresss, person_phonenumbers[][ePerson_PhoneNumber], &maxPerson_PhoneNumbers) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eP_Name][0] = 0;
    data[eP_Age] = 0;
    data[eP_AddressId] = 0;
    // Clear array for Emails
    for (new i = 0; i < sizeof(data[eP_EmailsRSId]); i++) {
        data[eP_EmailsRSId][i][0] = 0;
    }
    // Clear array for LuckyNumbers
    for (new i = 0; i < sizeof(data[eP_LuckyNumbers]); i++) {
        data[eP_LuckyNumbers][i] = 0;
    }
    // Clear array for OldAddresses
    for (new i = 0; i < sizeof(data[eP_OldAddressesId]); i++) {
        data[eP_OldAddressesId][i] = 0;
    }
    // Clear array for PhoneNumbers
    for (new i = 0; i < sizeof(data[eP_PhoneNumbersId]); i++) {
        data[eP_PhoneNumbersId][i] = 0;
    }

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // name
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eP_Name], 256);
                }
            }
            case 2: {
                // age
                if (wireType == WIRE_TYPE_VARINT) {
                    data[eP_Age] = DecodeVarint(buffer, offset, bufferSize);
                }
            }
            case 3: {
                // address
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Unpack nested message Address
                    new len = DecodeVarint(buffer, offset, bufferSize);
                    new startOffset = offset;
                    new tempAddress[eAddress];
                    UnpackAddress(buffer[startOffset], len, tempAddress);
                    // Add to global addresss array
                    if (maxAddresss < sizeof(addresss)) {
                        tempAddress[eA_Id] = maxAddresss + 1;
                        for (new j = 0; j < eAddress; j++) {
                            addresss[maxAddresss][j] = tempAddress[j];
                        }
                        data[eP_AddressId] = tempAddress[eA_Id];
                        maxAddresss++;
                    }
                    offset += len;
                }
            }
            case 4: {
                // emails
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Add repeated string to global repeatedStrings array
                    new tempString[256];
                    DecodeString(buffer, offset, bufferSize, tempString, 256);
                    // Add string to repeatedStrings array
                    if (maxRepeatedStrings < sizeof(repeatedStrings)) {
                        strcopy(repeatedStrings[maxRepeatedStrings][E_REPEATED_STRING_Text], tempString, 256);
                        repeatedStrings[maxRepeatedStrings][E_REPEATED_STRING_Id] = maxRepeatedStrings + 1;
                        // Find empty slot in ID array and assign string ID
                        for (new i = 0; i < sizeof(data[eP_EmailsRSId]); i++) {
                            if (data[eP_EmailsRSId][i] == 0) {
                                data[eP_EmailsRSId][i] = repeatedStrings[maxRepeatedStrings][E_REPEATED_STRING_Id];
                                break;
                            }
                        }
                        maxRepeatedStrings++;
                    }
                }
            }
            case 5: {
                // lucky_numbers
                if (wireType == WIRE_TYPE_VARINT) {
                    // Find empty slot in array
                    for (new i = 0; i < sizeof(data[eP_LuckyNumbers]); i++) {
                        if (data[eP_LuckyNumbers][i] == 0) {
                            data[eP_LuckyNumbers][i] = DecodeVarint(buffer, offset, bufferSize);
                            break;
                        }
                    }
                }
            }
            case 6: {
                // old_addresses
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Unpack repeated nested message Address
                    new len = DecodeVarint(buffer, offset, bufferSize);
                    new startOffset = offset;
                    new tempAddress[eAddress];
                    UnpackAddress(buffer[startOffset], len, tempAddress);
                    // Add to global addresss array
                    if (maxAddresss < sizeof(addresss)) {
                        tempAddress[eA_Id] = maxAddresss + 1;
                        for (new j = 0; j < eAddress; j++) {
                            addresss[maxAddresss][j] = tempAddress[j];
                        }
                        // Find empty slot in ID array
                        for (new i = 0; i < sizeof(data[eP_OldAddressesId]); i++) {
                            if (data[eP_OldAddressesId][i] == 0) {
                                data[eP_OldAddressesId][i] = tempAddress[eA_Id];
                                break;
                            }
                        }
                        maxAddresss++;
                    }
                    offset += len;
                }
            }
            case 7: {
                // phone_numbers
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Unpack repeated nested message Person_PhoneNumber
                    new len = DecodeVarint(buffer, offset, bufferSize);
                    new startOffset = offset;
                    new tempPerson_PhoneNumber[ePerson_PhoneNumber];
                    UnpackPerson_PhoneNumber(buffer[startOffset], len, tempPerson_PhoneNumber);
                    // Add to global person_phonenumbers array
                    if (maxPerson_PhoneNumbers < sizeof(person_phonenumbers)) {
                        tempPerson_PhoneNumber[ePP_Id] = maxPerson_PhoneNumbers + 1;
                        for (new j = 0; j < ePerson_PhoneNumber; j++) {
                            person_phonenumbers[maxPerson_PhoneNumbers][j] = tempPerson_PhoneNumber[j];
                        }
                        // Find empty slot in ID array
                        for (new i = 0; i < sizeof(data[eP_PhoneNumbersId]); i++) {
                            if (data[eP_PhoneNumbersId][i] == 0) {
                                data[eP_PhoneNumbersId][i] = tempPerson_PhoneNumber[ePP_Id];
                                break;
                            }
                        }
                        maxPerson_PhoneNumbers++;
                    }
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

// ========== Company Pack Function ==========
// Pack Company structure into byte buffer
stock PackCompany(const data[eCompany], buffer[], &offset, const repeatedStrings[][E_REPEATED_STRING], maxRepeatedStrings, const persons[][ePerson], maxPersons, const addresss[][eAddress], maxAddresss, const person_phonenumbers[][ePerson_PhoneNumber], maxPerson_PhoneNumbers) {

    // Field 1: name
    if (strlen(data[eC_Name]) > 0) {
        EncodeTag(1, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
        EncodeString(data[eC_Name], buffer, offset);
    }

    // Field 2: employees
    // Pack repeated message field employees using ID array
    for (new i = 0; i < sizeof(data[eC_EmployeesId]); i++) {
        if (data[eC_EmployeesId][i] != 0) {
            new tempPerson[ePerson];
            if (GetPersonById(data[eC_EmployeesId][i], persons, maxPersons, tempPerson)) {
                // Pack as length-delimited submessage
                new subBuffer[1024];
                new subOffset = 0;
                PackPerson(tempPerson, subBuffer, subOffset);
                EncodeTag(2, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
                EncodeVarint(subOffset, buffer, offset);
                for (new j = 0; j < subOffset; j++) {
                    buffer[offset++] = subBuffer[j];
                }
            }
        }
    }

    // Field 3: headquarters
    // Pack nested message field headquarters using ID
    if (data[eC_HeadquartersId] != 0) {
        new tempAddress[eAddress];
        if (GetAddressById(data[eC_HeadquartersId], addresss, maxAddresss, tempAddress)) {
            // Pack as length-delimited submessage
            new subBuffer[1024];
            new subOffset = 0;
            PackAddress(tempAddress, subBuffer, subOffset);
            EncodeTag(3, WIRE_TYPE_LENGTH_DELIMITED, buffer, offset);
            EncodeVarint(subOffset, buffer, offset);
            for (new i = 0; i < subOffset; i++) {
                buffer[offset++] = subBuffer[i];
            }
        }
    }

}

// ========== Company Unpack Function ==========
// Unpack Company structure from byte buffer
stock UnpackCompany(const buffer[], bufferSize, data[eCompany], repeatedStrings[][E_REPEATED_STRING], &maxRepeatedStrings, persons[][ePerson], &maxPersons, addresss[][eAddress], &maxAddresss, person_phonenumbers[][ePerson_PhoneNumber], &maxPerson_PhoneNumbers) {
    new offset = 0;
    new fieldNumber, wireType;

    // Clear the data structure
    data[eC_Name][0] = 0;
    // Clear array for Employees
    for (new i = 0; i < sizeof(data[eC_EmployeesId]); i++) {
        data[eC_EmployeesId][i] = 0;
    }
    data[eC_HeadquartersId] = 0;

    while (offset < bufferSize) {
        DecodeTag(buffer, offset, bufferSize, fieldNumber, wireType);
        
        switch (fieldNumber) {
            case 1: {
                // name
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    DecodeString(buffer, offset, bufferSize, data[eC_Name], 256);
                }
            }
            case 2: {
                // employees
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Unpack repeated nested message Person
                    new len = DecodeVarint(buffer, offset, bufferSize);
                    new startOffset = offset;
                    new tempPerson[ePerson];
                    UnpackPerson(buffer[startOffset], len, tempPerson, repeatedStrings, maxRepeatedStrings, addresss, maxAddresss, person_phonenumbers, maxPerson_PhoneNumbers);
                    // Add to global persons array
                    if (maxPersons < sizeof(persons)) {
                        tempPerson[eP_Id] = maxPersons + 1;
                        for (new j = 0; j < ePerson; j++) {
                            persons[maxPersons][j] = tempPerson[j];
                        }
                        // Find empty slot in ID array
                        for (new i = 0; i < sizeof(data[eC_EmployeesId]); i++) {
                            if (data[eC_EmployeesId][i] == 0) {
                                data[eC_EmployeesId][i] = tempPerson[eP_Id];
                                break;
                            }
                        }
                        maxPersons++;
                    }
                    offset += len;
                }
            }
            case 3: {
                // headquarters
                if (wireType == WIRE_TYPE_LENGTH_DELIMITED) {
                    // Unpack nested message Address
                    new len = DecodeVarint(buffer, offset, bufferSize);
                    new startOffset = offset;
                    new tempAddress[eAddress];
                    UnpackAddress(buffer[startOffset], len, tempAddress);
                    // Add to global addresss array
                    if (maxAddresss < sizeof(addresss)) {
                        tempAddress[eA_Id] = maxAddresss + 1;
                        for (new j = 0; j < eAddress; j++) {
                            addresss[maxAddresss][j] = tempAddress[j];
                        }
                        data[eC_HeadquartersId] = tempAddress[eA_Id];
                        maxAddresss++;
                    }
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

// ========== Helper Functions for ID Lookup ==========

// Get repeated string by ID
stock GetRepeatedStringById(id, const data[][E_REPEATED_STRING], maxItems, result[256]) {
    for (new i = 0; i < maxItems; i++) {
        if (data[i][E_REPEATED_STRING_Id] == id) {
            strcopy(result, data[i][E_REPEATED_STRING_Text], 256);
            return 1;
        }
    }
    result[0] = 0; // Not found
    return 0;
}

// Get Address by ID from array
stock GetAddressById(id, const data[][eAddress], maxItems, result[eAddress]) {
    for (new i = 0; i < maxItems; i++) {
        if (data[i][eA_Id] == id) {
            for (new j = 0; j < eAddress; j++) {
                result[j] = data[i][j];
            }
            return 1; // Found
        }
    }
    return 0; // Not found
}

// Get Person_PhoneNumber by ID from array
stock GetPerson_PhoneNumberById(id, const data[][ePerson_PhoneNumber], maxItems, result[ePerson_PhoneNumber]) {
    for (new i = 0; i < maxItems; i++) {
        if (data[i][ePP_Id] == id) {
            for (new j = 0; j < ePerson_PhoneNumber; j++) {
                result[j] = data[i][j];
            }
            return 1; // Found
        }
    }
    return 0; // Not found
}

// Get Person by ID from array
stock GetPersonById(id, const data[][ePerson], maxItems, result[ePerson]) {
    for (new i = 0; i < maxItems; i++) {
        if (data[i][eP_Id] == id) {
            for (new j = 0; j < ePerson; j++) {
                result[j] = data[i][j];
            }
            return 1; // Found
        }
    }
    return 0; // Not found
}

// Get Company by ID from array
stock GetCompanyById(id, const data[][eCompany], maxItems, result[eCompany]) {
    for (new i = 0; i < maxItems; i++) {
        if (data[i][eC_Id] == id) {
            for (new j = 0; j < eCompany; j++) {
                result[j] = data[i][j];
            }
            return 1; // Found
        }
    }
    return 0; // Not found
}

