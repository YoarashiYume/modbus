#pragma once
#include "TransferedDataEnums.hpp"
#include <vector>

namespace ModbusCPP
{
	/**
	\brief Базовое представление отправляемого запроса-записи
	*/
	struct SendData
	{
		BASE_COMMAND command{};	///< Код Modbus-команды
		std::uint32_t startBit{};		///< Начальный бит запросв
		/**
		\brief Дестркутор
		*/
		virtual ~SendData() = default;
	};
	/**
	\brief Структура для Modbus-функций 0x05 и 0x06
	*/
	struct SimpleSendData : SendData
	{
		std::uint16_t sendValue{};///< Отправляемое значение
	};
	/**
	\brief Структура для Modbus-функций 0x0F и 0x10
	*/
	struct MultipleSendData : SendData
	{
		std::vector<std::uint8_t> sendData;	///< Отправляемые значения (в виде набора байт)
		std::int32_t count{};				///< Количество отправляемых значений
		BYTE_ORDER order{ BYTE_ORDER::NONE };///< Необходимый порядок байт
	};
	/**
	\brief Структура возвращаемая при считывании всех состояний устройства
	*/
	struct DeviceAnswer
	{
		std::uint32_t posInDevice;	///< Начальный бит в устройстве
		std::vector<std::uint8_t> data; ///< Данные из устройства
	};
	/**
	\brief Структура, хранящая структуру ведомого устройства
	*/
	struct DeviceInfo
	{
		DEVICE_DATA_TYPE dataType;///< Тип данных
		BYTE_ORDER byteOrder{ BYTE_ORDER::NONE };///< порядок байт данных
		std::uint32_t startBit{};		///< Начальный бит данных
		std::uint32_t sizeInByte{};		///< Размер данных в байтах
		std::uint32_t countOfElements{};///< Количество элементов данных
	};
	/**
	\brief Структура используемая для инициализации структуры устройства
	*/
	struct SlaveData
	{
		/**
		\brief Вспомогательная структура используемая для инициализации данных в структуре устройства
		*/
		struct Data
		{
			std::uint32_t startBits{};///< Начальный адрес
			std::uint32_t count{};///< Количество элементов
			std::vector<std::uint8_t> data;///< значения элементов
			std::vector<std::uint8_t> countOfDataOnElement; ///< Количество байт на элемент
			std::vector<BYTE_ORDER> order;///< Порядок байтов элементов (используется для регистров ввода и хранения)
		};
		Data coil;///< Данные для инициализации регистров флагов 
		Data inputCoil;///< Данные для инициализации дискретных входов
		Data inputRegisters;///< Данные для инициализации регистров флагов 
		Data holdingRegisters;///< Данные для инициализации регистров флагов 
	};


}