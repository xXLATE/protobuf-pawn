#include <core>
#include <console>

#include "test/test_nested_example/test/test_nested_example/nested_example_enums.inc"
#include "test/test_nested_example/test/test_nested_example/nested_example_serialization.inc"

// Глобальные массивы для хранения данных (имитируем базу данных)
new g_addresses[10][eAddress];           // Массив адресов
new g_persons[10][ePerson];              // Массив людей
new g_phoneNumbers[20][ePerson_PhoneNumber]; // Массив номеров телефонов
new g_companies[5][eCompany];            // Массив компаний

new g_addressCount = 0;
new g_personCount = 0;
new g_phoneNumberCount = 0;
new g_companyCount = 0;

main()
{
    printf("=== ТЕСТ NESTED EXAMPLE PROTOBUF ===");
    printf("");
    
    // ========== ЭТАП 1: СОЗДАНИЕ И ЗАПОЛНЕНИЕ СТРУКТУР ==========
    printf("1. Создание и заполнение структур данных...");
    CreateTestData();
    
    // ========== ЭТАП 2: ТЕСТИРОВАНИЕ СЕРИАЛИЗАЦИИ ==========
    printf("");
    printf("2. Тестирование сериализации структур...");
    TestSerialization();
    
    // ========== ЭТАП 3: ТЕСТИРОВАНИЕ ПОЛНОГО ЦИКЛА ==========
    printf("");
    printf("3. Тестирование полного цикла сериализация -> десериализация...");
    TestFullCycle();
    
    printf("");
    printf("=== ТЕСТ ЗАВЕРШЕН УСПЕШНО ===");
}

// Создание тестовых данных
CreateTestData()
{
    printf("  Создание адресов...");
    
    // Адрес 1
    g_addresses[0][eA_Id] = 1;
    format(g_addresses[0][eA_Street], 256, "123 Main Street");
    format(g_addresses[0][eA_City], 256, "New York");
    g_addresses[0][eA_ZipCode] = 10001;
    g_addressCount++;
    printf("    Address 1: %s, %s, %d", g_addresses[0][eA_Street], g_addresses[0][eA_City], g_addresses[0][eA_ZipCode]);
    
    // Адрес 2  
    g_addresses[1][eA_Id] = 2;
    format(g_addresses[1][eA_Street], 256, "456 Oak Avenue");
    format(g_addresses[1][eA_City], 256, "Los Angeles");
    g_addresses[1][eA_ZipCode] = 90210;
    g_addressCount++;
    printf("    Address 2: %s, %s, %d", g_addresses[1][eA_Street], g_addresses[1][eA_City], g_addresses[1][eA_ZipCode]);
    
    // Адрес 3
    g_addresses[2][eA_Id] = 3;
    format(g_addresses[2][eA_Street], 256, "789 Pine Road");
    format(g_addresses[2][eA_City], 256, "Chicago");
    g_addresses[2][eA_ZipCode] = 60601;
    g_addressCount++;
    printf("    Address 3: %s, %s, %d", g_addresses[2][eA_Street], g_addresses[2][eA_City], g_addresses[2][eA_ZipCode]);
    
    printf("  Создание номеров телефонов...");
    
    // Телефон 1
    g_phoneNumbers[0][ePP_Id] = 1;
    format(g_phoneNumbers[0][ePP_Number], 256, "+1-555-0123");
    format(g_phoneNumbers[0][ePP_Type], 256, "mobile");
    g_phoneNumberCount++;
    printf("    Phone 1: %s (%s)", g_phoneNumbers[0][ePP_Number], g_phoneNumbers[0][ePP_Type]);
    
    // Телефон 2
    g_phoneNumbers[1][ePP_Id] = 2;
    format(g_phoneNumbers[1][ePP_Number], 256, "+1-555-0456");
    format(g_phoneNumbers[1][ePP_Type], 256, "work");
    g_phoneNumberCount++;
    printf("    Phone 2: %s (%s)", g_phoneNumbers[1][ePP_Number], g_phoneNumbers[1][ePP_Type]);
    
    // Телефон 3
    g_phoneNumbers[2][ePP_Id] = 3;
    format(g_phoneNumbers[2][ePP_Number], 256, "+1-555-0789");
    format(g_phoneNumbers[2][ePP_Type], 256, "home");
    g_phoneNumberCount++;
    printf("    Phone 3: %s (%s)", g_phoneNumbers[2][ePP_Number], g_phoneNumbers[2][ePP_Type]);
    
    printf("  Создание людей...");
    
    // Человек 1
    g_persons[0][eP_Id] = 1;
    format(g_persons[0][eP_Name], 256, "John Doe");
    g_persons[0][eP_Age] = 30;
    g_persons[0][eP_AddressId] = 1; // Ссылка на адрес 1
    
    // Emails
    format(g_persons[0][eP_EmailsRSId][0], 256, "john@example.com");
    format(g_persons[0][eP_EmailsRSId][1], 256, "john.doe@work.com");
    
    // Lucky numbers
    g_persons[0][eP_LuckyNumbers][0] = 7;
    g_persons[0][eP_LuckyNumbers][1] = 13;
    g_persons[0][eP_LuckyNumbers][2] = 42;
    
    // Old addresses
    g_persons[0][eP_OldAddressesId][0] = 2; // Ссылка на адрес 2
    g_persons[0][eP_OldAddressesId][1] = 3; // Ссылка на адрес 3
    
    // Phone numbers
    g_persons[0][eP_PhoneNumbersId][0] = 1; // Ссылка на телефон 1
    g_persons[0][eP_PhoneNumbersId][1] = 2; // Ссылка на телефон 2
    
    g_personCount++;
    printf("    Person 1: %s, age %d, address_id %d", g_persons[0][eP_Name], g_persons[0][eP_Age], g_persons[0][eP_AddressId]);
    printf("      Emails: %s, %s", g_persons[0][eP_EmailsRSId][0], g_persons[0][eP_EmailsRSId][1]);
    printf("      Lucky numbers: %d, %d, %d", g_persons[0][eP_LuckyNumbers][0], g_persons[0][eP_LuckyNumbers][1], g_persons[0][eP_LuckyNumbers][2]);
    printf("      Old addresses: %d, %d", g_persons[0][eP_OldAddressesId][0], g_persons[0][eP_OldAddressesId][1]);
    printf("      Phone numbers: %d, %d", g_persons[0][eP_PhoneNumbersId][0], g_persons[0][eP_PhoneNumbersId][1]);
    
    // Человек 2
    g_persons[1][eP_Id] = 2;
    format(g_persons[1][eP_Name], 256, "Jane Smith");
    g_persons[1][eP_Age] = 25;
    g_persons[1][eP_AddressId] = 2; // Ссылка на адрес 2
    
    // Emails
    format(g_persons[1][eP_EmailsRSId][0], 256, "jane@example.com");
    
    // Lucky numbers
    g_persons[1][eP_LuckyNumbers][0] = 3;
    g_persons[1][eP_LuckyNumbers][1] = 21;
    
    // Phone numbers
    g_persons[1][eP_PhoneNumbersId][0] = 3; // Ссылка на телефон 3
    
    g_personCount++;
    printf("    Person 2: %s, age %d, address_id %d", g_persons[1][eP_Name], g_persons[1][eP_Age], g_persons[1][eP_AddressId]);
    
    printf("  Создание компании...");
    
    // Компания 1
    g_companies[0][eC_Id] = 1;
    format(g_companies[0][eC_Name], 256, "Tech Solutions Inc");
    g_companies[0][eC_EmployeesId][0] = 1; // Ссылка на человека 1
    g_companies[0][eC_EmployeesId][1] = 2; // Ссылка на человека 2
    g_companies[0][eC_HeadquartersId] = 1; // Ссылка на адрес 1
    g_companyCount++;
    printf("    Company: %s, headquarters_id %d", g_companies[0][eC_Name], g_companies[0][eC_HeadquartersId]);
    printf("    Employees: %d, %d", g_companies[0][eC_EmployeesId][0], g_companies[0][eC_EmployeesId][1]);
}

// Тестирование сериализации отдельных структур
TestSerialization()
{
    new buffer[2048];
    new offset;
    
    printf("  Сериализация Address...");
    offset = 0;
    PackAddress(g_addresses[0], buffer, offset);
    printf("    Address serialized: %d bytes", offset);
    PrintBufferHex(buffer, offset, "Address");
    
    printf("  Сериализация Person_PhoneNumber...");
    offset = 0;
    PackPerson_PhoneNumber(g_phoneNumbers[0], buffer, offset);
    printf("    PhoneNumber serialized: %d bytes", offset);
    PrintBufferHex(buffer, offset, "PhoneNumber");
    
    printf("  Сериализация Person...");
    offset = 0;
    PackPerson(g_persons[0], buffer, offset);
    printf("    Person serialized: %d bytes", offset);
    PrintBufferHex(buffer, offset, "Person");
    
    printf("  Сериализация Company...");
    offset = 0;
    PackCompany(g_companies[0], buffer, offset);
    printf("    Company serialized: %d bytes", offset);
    PrintBufferHex(buffer, offset, "Company");
}

// Тестирование полного цикла сериализация -> десериализация
TestFullCycle()
{
    new buffer[2048];
    new offset;
    
    printf("  === ТЕСТ АДРЕСА ===");
    // Сериализация адреса
    offset = 0;
    PackAddress(g_addresses[0], buffer, offset);
    printf("    Оригинальный адрес: %s, %s, %d", g_addresses[0][eA_Street], g_addresses[0][eA_City], g_addresses[0][eA_ZipCode]);
    printf("    Сериализован в %d байт", offset);
    
    // Десериализация адреса
    new unpackedAddress[eAddress];
    UnpackAddress(buffer, offset, unpackedAddress);
    printf("    Десериализованный адрес: %s, %s, %d", unpackedAddress[eA_Street], unpackedAddress[eA_City], unpackedAddress[eA_ZipCode]);
    printf("    Проверка: %s", (strcmp(g_addresses[0][eA_Street], unpackedAddress[eA_Street]) == 0) ? "SUCCESS" : "FAILED");
    
    printf("");
    printf("  === ТЕСТ ТЕЛЕФОНА ===");
    // Сериализация телефона
    offset = 0;
    PackPerson_PhoneNumber(g_phoneNumbers[0], buffer, offset);
    printf("    Оригинальный телефон: %s (%s)", g_phoneNumbers[0][ePP_Number], g_phoneNumbers[0][ePP_Type]);
    printf("    Сериализован в %d байт", offset);
    
    // Десериализация телефона
    new unpackedPhone[ePerson_PhoneNumber];
    UnpackPerson_PhoneNumber(buffer, offset, unpackedPhone);
    printf("    Десериализованный телефон: %s (%s)", unpackedPhone[ePP_Number], unpackedPhone[ePP_Type]);
    printf("    Проверка: %s", (strcmp(g_phoneNumbers[0][ePP_Number], unpackedPhone[ePP_Number]) == 0) ? "SUCCESS" : "FAILED");
    
    printf("");
    printf("  === ТЕСТ ЧЕЛОВЕКА ===");
    // Сериализация человека  
    offset = 0;
    PackPerson(g_persons[0], buffer, offset);
    printf("    Оригинальный человек: %s, возраст %d", g_persons[0][eP_Name], g_persons[0][eP_Age]);
    printf("    Сериализован в %d байт", offset);
    
    // Десериализация человека
    new unpackedPerson[ePerson];
    UnpackPerson(buffer, offset, unpackedPerson);
    printf("    Десериализованный человек: %s, возраст %d", unpackedPerson[eP_Name], unpackedPerson[eP_Age]);
    printf("    Email 1: %s", unpackedPerson[eP_EmailsRSId][0]);
    printf("    Email 2: %s", unpackedPerson[eP_EmailsRSId][1]);
    printf("    Lucky numbers: %d, %d, %d", unpackedPerson[eP_LuckyNumbers][0], unpackedPerson[eP_LuckyNumbers][1], unpackedPerson[eP_LuckyNumbers][2]);
    printf("    Address ID: %d", unpackedPerson[eP_AddressId]);
    printf("    Проверка: %s", (strcmp(g_persons[0][eP_Name], unpackedPerson[eP_Name]) == 0) ? "SUCCESS" : "FAILED");
    
    printf("");
    printf("  === ТЕСТ КОМПАНИИ ===");
    // Сериализация компании
    offset = 0;
    PackCompany(g_companies[0], buffer, offset);
    printf("    Оригинальная компания: %s", g_companies[0][eC_Name]);
    printf("    Сериализована в %d байт", offset);
    
    // Десериализация компании
    new unpackedCompany[eCompany];
    UnpackCompany(buffer, offset, unpackedCompany);
    printf("    Десериализованная компания: %s", unpackedCompany[eC_Name]);
    printf("    Headquarters ID: %d", unpackedCompany[eC_HeadquartersId]);
    printf("    Employee IDs: %d, %d", unpackedCompany[eC_EmployeesId][0], unpackedCompany[eC_EmployeesId][1]);
    printf("    Проверка: %s", (strcmp(g_companies[0][eC_Name], unpackedCompany[eC_Name]) == 0) ? "SUCCESS" : "FAILED");
    
    printf("");
    printf("  === ТЕСТ ВСПОМОГАТЕЛЬНЫХ ФУНКЦИЙ ===");
    TestHelperFunctions();
}

// Тестирование вспомогательных функций поиска по ID
TestHelperFunctions()
{
    new foundAddress[eAddress];
    new foundPerson[ePerson];
    new foundPhone[ePerson_PhoneNumber];
    new foundCompany[eCompany];
    
    printf("    Поиск Address по ID 1...");
    if(GetAddressById(1, g_addresses, g_addressCount, foundAddress)) {
        printf("      Найден: %s, %s, %d", foundAddress[eA_Street], foundAddress[eA_City], foundAddress[eA_ZipCode]);
    } else {
        printf("      НЕ НАЙДЕН!");
    }
    
    printf("    Поиск Person по ID 1...");
    if(GetPersonById(1, g_persons, g_personCount, foundPerson)) {
        printf("      Найден: %s, возраст %d", foundPerson[eP_Name], foundPerson[eP_Age]);
    } else {
        printf("      НЕ НАЙДЕН!");
    }
    
    printf("    Поиск PhoneNumber по ID 1...");
    if(GetPerson_PhoneNumberById(1, g_phoneNumbers, g_phoneNumberCount, foundPhone)) {
        printf("      Найден: %s (%s)", foundPhone[ePP_Number], foundPhone[ePP_Type]);
    } else {
        printf("      НЕ НАЙДЕН!");
    }
    
    printf("    Поиск Company по ID 1...");
    if(GetCompanyById(1, g_companies, g_companyCount, foundCompany)) {
        printf("      Найдена: %s", foundCompany[eC_Name]);
    } else {
        printf("      НЕ НАЙДЕНА!");
    }
    
    printf("    Поиск несуществующего Address по ID 999...");
    if(GetAddressById(999, g_addresses, g_addressCount, foundAddress)) {
        printf("      ОШИБКА: Найден несуществующий адрес!");
    } else {
        printf("      Корректно: не найден");
    }
}

// Вспомогательная функция для вывода буфера в hex формате
PrintBufferHex(const buffer[], size, const name[])
{
    printf("    %s hex dump (%d bytes):", name, size);
    
    new line[256];
    new pos = 0;
    
    for(new i = 0; i < size; i++) {
        if(i % 16 == 0) {
            if(i > 0) {
                printf("      %s", line);
                pos = 0;
            }
            pos += format(line[pos], sizeof(line) - pos, "%04X: ", i);
        }
        
        pos += format(line[pos], sizeof(line) - pos, "%02X ", buffer[i] & 0xFF);
        
        if(i == size - 1) {
            // Дополняем последнюю строку пробелами
            while((i % 16) != 15) {
                pos += format(line[pos], sizeof(line) - pos, "   ");
                i++;
            }
            printf("      %s", line);
        }
    }
} 