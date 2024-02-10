#pragma once
#include <modbus.h>
#include <vector>
#include "TransferedDataEnums.hpp"
namespace ModbusCPP
{
	/**
	\brief Структура - обертка над modbus_mapping_t, позвалающая хранить порядок байт у регистров
	*/
    struct ModbusData : modbus_mapping_t 
    {
	public:
		using value_type = std::uint32_t;
	private:
		bool isInit{false}; ///< Флаг инициализации структуры modbus_mapping_t
		
		/**
		\brief Метод выделяющий память под modbus_mapping_t
		\return Если память была выделена true, иначе - false
		*/
		bool createMemory();
		/**
		\brief Метод очищающий память modbus_mapping_t
		*/
		void clearMemory();
	public:
		
        std::vector<BYTE_ORDER> inputRegistersOrder;///< порядок байт у регистров ввода
        std::vector<BYTE_ORDER> holdingsRegistersOrder;///< порядок байт у регистров хранения 

		ModbusData() = delete;
		/**
		\brief Конструктор копирования
		\param other копируемый объект
		*/
		ModbusData(const ModbusData& other);
		/**
		\brief Конструктор переноса
		\param other переносимый объект
		*/
		ModbusData(ModbusData&& other);
		/**
		\brief Конструктор
		\param start_bits Начальный бит регистров флагов
		\param nb_bits Количество регистров флагов
		\param start_input_bits Начальный бит дискретных входов
		\param nb_input_bits Количество дискретных входов
		\param start_registers Начальный бит регистров хранения
		\param nb_registers Количество регистров хранения
		\param start_input_registers Начальный бит регистров ввода
		\param nb_input_registers Начальный бит регистров ввода
		*/
		ModbusData(const value_type start_bits, const value_type nb_bits,
			const value_type start_input_bits, const value_type nb_input_bits,
			const value_type start_registers, const value_type nb_registers,
			const value_type start_input_registers, const value_type nb_input_registers);
        /**
		\brief Деструктор
		*/
		virtual ~ModbusData();
		/**
		\brief Метод проверяющий корректость инициализации структуры modbus_mapping_t
		\return Если структура инициализировалась корректно true, иначе false
		*/
		bool isCorrecInit() const;        
	};
}