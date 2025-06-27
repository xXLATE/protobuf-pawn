# Руководство по тестированию Protobuf-Pawn

## Созданные тестовые файлы

### 1. `test_nested_example_demo.pwn` - Полный функциональный тест
**Назначение**: Комплексное тестирование всех возможностей сгенерированного кода

**Что тестирует**:
- ✅ Создание и заполнение всех типов структур (Address, Person, PhoneNumber, Company)
- ✅ Сериализацию каждой структуры в отдельности
- ✅ Полный цикл сериализация → десериализация с проверкой данных
- ✅ Работу вспомогательных функций поиска по ID
- ✅ ID-based архитектуру с ссылками между структурами
- ✅ Повторяющиеся поля (emails, phone numbers, addresses)
- ✅ Hex dump сериализованных данных для отладки

**Структура теста**:
```
=== ТЕСТ NESTED EXAMPLE PROTOBUF ===

1. Создание и заполнение структур данных...
   - Создание адресов (3 шт.)
   - Создание номеров телефонов (3 шт.)
   - Создание людей (2 шт.) с связями
   - Создание компании с сотрудниками

2. Тестирование сериализации структур...
   - Address → byte array + hex dump
   - PhoneNumber → byte array + hex dump  
   - Person → byte array + hex dump
   - Company → byte array + hex dump

3. Тестирование полного цикла...
   - Address: pack → unpack → verify
   - PhoneNumber: pack → unpack → verify
   - Person: pack → unpack → verify (с emails, lucky numbers)
   - Company: pack → unpack → verify
   - Проверка вспомогательных функций GetXXXById()

=== ТЕСТ ЗАВЕРШЕН УСПЕШНО ===
```

### 2. `test_simple_demo.pwn` - Быстрый тест основных функций
**Назначение**: Быстрая проверка что код компилируется и основные функции работают

**Что тестирует**:
- ✅ Базовую сериализацию/десериализацию Address
- ✅ Базовую сериализацию/десериализацию Person с emails и lucky numbers
- ✅ Доступность всех сгенерированных функций и энумов

## Как запустить тесты

### Предварительные требования
1. Установленный Pawn compiler
2. Сгенерированные файлы из `nested_example.proto`:
   - `test/test_nested_example/test/test_nested_example/nested_example_enums.inc`
   - `test/test_nested_example/test/test_nested_example/nested_example_serialization.inc`

### Команды для тестирования

#### Быстрый тест (рекомендуется начать с него):
```bash
pawncc test_simple_demo.pwn -o test_simple_demo.amx
# Затем запуск test_simple_demo.amx в вашем Pawn runtime
```

#### Полный тест:
```bash
pawncc test_nested_example_demo.pwn -o test_nested_example_demo.amx
# Затем запуск test_nested_example_demo.amx в вашем Pawn runtime
```

## Ожидаемые результаты

### При успешном выполнении вы увидите:
- ✅ Создание структур с корректными данными
- ✅ Успешную сериализацию (размеры в байтах > 0)
- ✅ Корректную десериализацию (данные совпадают с оригиналом)
- ✅ Работающие функции поиска по ID
- ✅ Сообщения "SUCCESS" при проверках
- ✅ Отсутствие ошибок компиляции

### При ошибках вы увидите:
- ❌ Ошибки компиляции (неопределенные символы)
- ❌ Сообщения "FAILED" при проверках данных
- ❌ Нулевые размеры при сериализации
- ❌ Некорректные данные при десериализации

## Отладка

### Если тест не компилируется:
1. Убедитесь что сгенерированы файлы `nested_example_enums.inc` и `nested_example_serialization.inc`
2. Проверьте пути к include файлам
3. Убедитесь что используется последняя версия плагина

### Если данные не совпадают:
1. Проверьте hex dump сериализованных данных
2. Убедитесь что используются правильные имена полей (eP_AddressId, eP_EmailsRSId и т.д.)
3. Проверьте что вспомогательные функции GetXXXById правильно работают

## Демонстрируемые возможности

### ID-based архитектура
```pawn
// Вместо прямого вложения структур:
person[eP_AddressId] = 1;  // Ссылка на адрес с ID=1

// Используются helper функции для доступа:
new address[eAddress];
GetAddressById(person[eP_AddressId], g_addresses, g_addressCount, address);
```

### Повторяющиеся поля
```pawn
// Массивы строк (emails):
person[eP_EmailsRSId][0] = "first@email.com";
person[eP_EmailsRSId][1] = "second@email.com";

// Массивы чисел (lucky numbers):
person[eP_LuckyNumbers][0] = 7;
person[eP_LuckyNumbers][1] = 13;

// Массивы ID ссылок (old addresses):
person[eP_OldAddressesId][0] = 2;  // Ссылка на адрес с ID=2
person[eP_OldAddressesId][1] = 3;  // Ссылка на адрес с ID=3
```

### Protobuf совместимость
```pawn
// Сериализованные данные полностью совместимы с Dart/Java/Python protobuf
new buffer[1024];
new offset = 0;
PackPerson(person, buffer, offset);
// buffer[] теперь содержит protobuf wire format данные
// которые могут быть десериализованы в любом языке
```

Эти тесты помогут убедиться что все исправления работают корректно и плагин готов к использованию в реальных проектах. 