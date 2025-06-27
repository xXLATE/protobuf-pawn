# Protobuf-Pawn Plugin

## Описание

Плагин для компилятора Protocol Buffers, который генерирует код на языке Pawn для работы с protobuf сообщениями. Обеспечивает совместимость между приложениями на Pawn и Dart через сетевое взаимодействие с использованием RakNet.

## Последние исправления (Декабрь 2024)

### ✅ Исправлены ошибки компиляции Pawn

**Проблема**: Компилятор Pawn выдавал ошибки о неопределенных символах:
```
error 017: undefined symbol "eP_Address"
error 017: undefined symbol "eP_Emails"
error 080: unknown symbol, or not a constant symbol (symbol "eP_Emails")
```

**Решение**: 
1. **Синхронизированы имена полей** между генераторами энумов и сериализации
2. **Исправлена логика именования полей** для ID-based архитектуры:
   - Одиночные сообщения: `eP_AddressId` (вместо `eP_Address`)
   - Повторяющиеся строки: `eP_EmailsRSId` (вместо `eP_Emails`) 
   - Повторяющиеся сообщения: `eP_OldAddressesId`, `eP_PhoneNumbersId`
3. **Добавлены вспомогательные функции поиска по ID**:
   - `GetAddressById(id, data[][], maxItems, result[])`
   - `GetPersonById(id, data[][], maxItems, result[])`
   - `GetPerson_PhoneNumberById(id, data[][], maxItems, result[])`
   - `GetCompanyById(id, data[][], maxItems, result[])`

### 🔧 Техническая архитектура

**Перенесены общие функции в utils.go**:
- `getMessagePrefix()` - генерация префиксов сообщений
- `convertFieldName()` - конвертация имен полей из snake_case в PascalCase

**Обновлена функция `generateCorrectEnumFieldName()`** для правильного формирования имен полей согласно логике генератора энумов.

## Архитектура плагина

Плагин генерирует **2 файла** на каждый `.proto` файл:

### 1. `*_enums.inc` - Структуры данных
- Энумы для всех сообщений и вложенных сообщений
- ID-based ссылки для сложных типов
- Поддержка типов: `int32`, `string`, `float`, `double`, `bool`, `enum`
- Вспомогательные энумы для повторяющихся полей

### 2. `*_serialization.inc` - Функции сериализации
- Утилитарные функции для protobuf wire format
- `Pack*()` функции для упаковки структур в byte arrays
- `Unpack*()` функции для распаковки byte arrays в структуры
- Вспомогательные функции поиска по ID
- Полная совместимость с Dart protobuf implementation

## Особенности

### ✨ ID-based архитектура
- Избегает двойного вложения в Pawn энумах
- Использует числовые ID для ссылок на сложные объекты
- Поддерживает произвольную глубину вложенности сообщений

### 🎯 Типизация полей
- **Примитивы**: прямое хранение значений
- **Строки**: массивы символов `[256]`
- **Сообщения**: ID ссылки (`*Id` суффикс)
- **Повторяющиеся поля**: массивы с настраиваемым размером
- **Bool поля**: префикс `bool:` для корректной типизации

### 🔗 Совместимость с Dart
- Протокол protobuf wire format
- Поддержка всех основных типов данных
- Корректное кодирование/декодирование varint, fixed32/64, строк
- Тестирование совместимости через `test_dart_interop.dart`

## Использование

### Компиляция плагина
```bash
go build -o protoc-gen-pawn.exe .
```

### Генерация кода
```bash
protoc --plugin=protoc-gen-pawn=./protoc-gen-pawn.exe --pawn_out=output_dir input.proto
```

### Пример использования в Pawn
```pawn
#include "example_enums.inc"
#include "example_serialization.inc"

main() {
    // Создание структуры
    new person[ePerson];
    person[eP_Id] = 1;
    format(person[eP_Name], 256, "John Doe");
    person[eP_Age] = 30;
    
    // Сериализация
    new buffer[1024];
    new offset = 0;
    PackPerson(person, buffer, offset);
    
    // Отправка по сети...
    // SendBuffer(buffer, offset);
    
    // Десериализация
    new receivedPerson[ePerson];
    UnpackPerson(buffer, offset, receivedPerson);
}
```

## Тестирование

### Файлы тестов
- `test_nested_compilation.pwn` - проверка компиляции
- `test_nested_demo.pwn` - демонстрация функциональности
- `test_dart_interop.dart` - тестирование совместимости с Dart
- `test/nested_example.proto` - сложный протобуф с вложенными сообщениями

### Тестовые протобуфы
- `test/example.proto` - базовый пример
- `test/nested_example.proto` - сложные вложенные структуры
- `test/items.proto` - игровые предметы
- `test/items_model.proto` - модели предметов

## Технические детали

### Константы
- `MAX_ARRAY_SIZE = 64` - размер массивов для повторяющихся полей
- Поддержка строк до 256 символов
- Буферы сериализации до 1024 байт

### Wire Type поддержка
- `WIRE_TYPE_VARINT` - int32, int64, bool, enum
- `WIRE_TYPE_FIXED32` - float
- `WIRE_TYPE_FIXED64` - double
- `WIRE_TYPE_LENGTH_DELIMITED` - string, bytes, messages

### Возможности расширения
- Легко добавляемые новые типы данных
- Настраиваемые размеры массивов
- Возможность добавления custom wire types

## Статус проекта

✅ **Готово к использованию**
- Основные функции реализованы
- Протестирована совместимость с Dart
- Исправлены ошибки компиляции Pawn
- Добавлены вспомогательные функции
- Документация обновлена

## Требования

- Go 1.19+
- Protocol Buffers compiler (protoc)
- Pawn compiler для тестирования

## Лицензия

Проект распространяется под лицензией, указанной в файле LICENSE. 