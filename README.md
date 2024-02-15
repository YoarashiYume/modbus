# modbus

Простая обертка над библиотекой [modbuslib](https://github.com/stephane/libmodbus) для работы по RTU.

Содержит реализацию для ведущего и ведомого устройства.

## Заполнение структуры устройства

Симулированная структура устройства:
 
| Регистр | Тип | Порядок байт | Moddbus-тип   |
| :---:  |  :---:   |  :---:  |   :---:  |
|0|UInt16|  NONE| Input Registers  |
|1|UInt32|  BADC| Input Registers  |
|2|------|------| ------ |
|3| bool |  NONE|  Coils |
|4|double|HGFEDCBA|Holding Registers |
|5|------|------|------ |
|6|------|------|------ |
|7|------|------| ------|

### Использование в качестве ведомого устройства
```cpp
ModbusSlave device{1/* id устройства*/};

/*
Настройки ModbusConfig
*/

device.addData({
    SlaveData
    {
        Data{3, 4, {1, 0, 1, 0}}, //bool
        Data{},//no discrete inputs
        Data{0, 2, 
            {0, 2/*UInt16*/, 0, 1, 13, 24 /*UInt32*/}/*Представление 2х значений в std::uint8_t[]*/,
            {2, 4}, {BYTE_ORDER::NONE, BYTE_ORDER::BADC}},//UInt16 and UInt32
        Data{4, 1, {0, 25, 3, 1}, {8} ,{BYTE_ORDER::HGFEDCBA}}//double
    }
});

if (device.connect())
{
    auto listen = device.listen(); //Начало прослушивания порта
    while (listen != LISTEN_RESULT::FAILED)
    {
        if (listen != LISTEN_RESULT::IGNORED_DATA)
            device.sendAnswer();//Обработка ответа callback`ом и отправка ответа
        listen = slave.listen();
    }
}
slave.close();//Удаление контекста

```


### Использование в качестве ведущего устройства

```cpp

ModbusMaster device

/*
Настройки ModbusConfig
*/
device.addDevice(1 /*id добавляемого ведомого устройства*/,{
    DeviceInfo{DEVICE_DATA_TYPE::INPUT_REGISTERS,
        BYTE_ORDER::NONE, 0, sizeof(std::uint16_t), 1
    },
    DeviceInfo{DEVICE_DATA_TYPE::INPUT_REGISTERS,
        BYTE_ORDER::BADC, 1, sizeof(std::uint32_t), 1
    },
    DeviceInfo{DEVICE_DATA_TYPE::COILS
        BYTE_ORDER::NONE, 3, sizeof(std::uint8_t), 1
    },
    DeviceInfo{DEVICE_DATA_TYPE::HOLDING_REGISTERS
        BYTE_ORDER::HGFEDCBA, 4, 8, 1
    }
});

device.write(1, 
    SimpleSendData{BASE_COMMAND::WRITE_SINGLE_REGISER, 0, 125});//Запись в UInt16 значения 125

auto readDevice = device.read(1); //Считывает структуры утсройства с id == 1

if (readDevice.first == true)
{
    /*Работа со структурами DeviceAnswer*/
}

```