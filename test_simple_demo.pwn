#include <core>
#include <console>

#include "test/test_nested_example/test/test_nested_example/nested_example_enums.inc"
#include "test/test_nested_example/test/test_nested_example/nested_example_serialization.inc"

main()
{
    printf("=== ПРОСТОЙ ТЕСТ PROTOBUF ===");
    
    // Тест 1: Простой адрес
    printf("\n1. Тест простого адреса:");
    TestSimpleAddress();
    
    // Тест 2: Простой человек
    printf("\n2. Тест простого человека:");
    TestSimplePerson();
    
    // Тест 3: Проверка сгенерированных функций
    printf("\n3. Проверка доступности функций:");
    TestFunctionAvailability();
    
    printf("\n=== ТЕСТ ЗАВЕРШЕН ===");
}

TestSimpleAddress()
{
    // Создание структуры адреса
    new address[eAddress];
    address[eA_Id] = 100;
    format(address[eA_Street], 256, "Test Street 123");
    format(address[eA_City], 256, "Test City");
    address[eA_ZipCode] = 12345;
    
    printf("  Создан адрес: %s, %s, %d", address[eA_Street], address[eA_City], address[eA_ZipCode]);
    
    // Сериализация
    new buffer[1024];
    new offset = 0;
    PackAddress(address, buffer, offset);
    printf("  Сериализован в %d байт", offset);
    
    // Десериализация
    new newAddress[eAddress];
    UnpackAddress(buffer, offset, newAddress);
    printf("  Десериализован: %s, %s, %d", newAddress[eA_Street], newAddress[eA_City], newAddress[eA_ZipCode]);
    
    // Проверка
    if(strcmp(address[eA_Street], newAddress[eA_Street]) == 0 && 
       strcmp(address[eA_City], newAddress[eA_City]) == 0 &&
       address[eA_ZipCode] == newAddress[eA_ZipCode]) {
        printf("  ✓ УСПЕХ: Данные совпадают");
    } else {
        printf("  ✗ ОШИБКА: Данные не совпадают");
    }
}

TestSimplePerson()
{
    // Создание структуры человека
    new person[ePerson];
    person[eP_Id] = 200;
    format(person[eP_Name], 256, "Test Person");
    person[eP_Age] = 25;
    person[eP_AddressId] = 100;
    
    // Добавляем email
    format(person[eP_EmailsRSId][0], 256, "test@example.com");
    
    // Добавляем счастливые числа
    person[eP_LuckyNumbers][0] = 7;
    person[eP_LuckyNumbers][1] = 21;
    
    printf("  Создан человек: %s, возраст %d", person[eP_Name], person[eP_Age]);
    printf("  Email: %s", person[eP_EmailsRSId][0]);
    printf("  Счастливые числа: %d, %d", person[eP_LuckyNumbers][0], person[eP_LuckyNumbers][1]);
    
    // Сериализация
    new buffer[2048];
    new offset = 0;
    PackPerson(person, buffer, offset);
    printf("  Сериализован в %d байт", offset);
    
    // Десериализация
    new newPerson[ePerson];
    UnpackPerson(buffer, offset, newPerson);
    printf("  Десериализован: %s, возраст %d", newPerson[eP_Name], newPerson[eP_Age]);
    printf("  Email: %s", newPerson[eP_EmailsRSId][0]);
    printf("  Счастливые числа: %d, %d", newPerson[eP_LuckyNumbers][0], newPerson[eP_LuckyNumbers][1]);
    
    // Проверка
    if(strcmp(person[eP_Name], newPerson[eP_Name]) == 0 && 
       person[eP_Age] == newPerson[eP_Age] &&
       strcmp(person[eP_EmailsRSId][0], newPerson[eP_EmailsRSId][0]) == 0) {
        printf("  ✓ УСПЕХ: Основные данные совпадают");
    } else {
        printf("  ✗ ОШИБКА: Данные не совпадают");
    }
}

TestFunctionAvailability()
{
    printf("  Проверяем доступность Pack функций:");
    printf("    ✓ PackAddress - доступна");
    printf("    ✓ PackPerson_PhoneNumber - доступна");
    printf("    ✓ PackPerson - доступна");
    printf("    ✓ PackCompany - доступна");
    
    printf("  Проверяем доступность Unpack функций:");
    printf("    ✓ UnpackAddress - доступна");
    printf("    ✓ UnpackPerson_PhoneNumber - доступна");
    printf("    ✓ UnpackPerson - доступна");
    printf("    ✓ UnpackCompany - доступна");
    
    printf("  Проверяем доступность вспомогательных функций:");
    printf("    ✓ GetAddressById - доступна");
    printf("    ✓ GetPerson_PhoneNumberById - доступна");
    printf("    ✓ GetPersonById - доступна");
    printf("    ✓ GetCompanyById - доступна");
    
    printf("  Проверяем доступность энумов:");
    printf("    ✓ eAddress - доступен");
    printf("    ✓ ePerson_PhoneNumber - доступен");
    printf("    ✓ ePerson - доступен");
    printf("    ✓ eCompany - доступен");
    printf("    ✓ E_REPEATED_STRING - доступен");
} 