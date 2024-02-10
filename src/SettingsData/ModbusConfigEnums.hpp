#pragma once
#include <cstdint>

namespace ModbusCPP
{
	/**
	\brief Бит четности.
	*/
	enum class PARITY : std::int8_t
	{
		NONE = 'N',												///< Отсутствует.
		EVEN = 'E',												///< Нечетный.
		ODD = 'O'												///< Четный.
	};
	/**
		\brief Количество стоповых бит.
		*/
	enum class STOP_BITS : std::uint8_t
	{
		ONE = 1,												///< Один стоповый бит.
		TWO = 2													///< Два стоповых бита.
	};
	/**
	\brief Количество бит, приходящихся на данные.
	*/
	enum class DATA_BITS : std::uint8_t
	{
		FIVE = 5,												///< 5 бит.
		SIX = 6,												///< 6 бит.
		SEVEN = 7,												///< 7 бит.
		EIGHT = 8												///< 8 бит.
	};
	/**
	\brief Скорость передачи.
	*/
	enum class BAUD_RATE : std::uint32_t
	{
		SPEED_300 = 300,										///< 300 бод
		SPEED_600 = 600,										///< 600 бод
		SPEED_1200 = 1200,										///< 1200 бод
		SPEED_2400 = 2400,										///< 2400 бод
		SPEED_4800 = 4800,										///< 4800 бод
		SPEED_9600 = 9600,										///< 9600 бод
		SPEED_14400 = 14400,									///< 14400 бод
		SPEED_19200 = 19200,									///< 19200 бод
		SPEED_38400 = 38400,									///< 38400 бод
		SPEED_56000 = 56000,									///< 56000 бод
		SPEED_57600 = 57600,									///< 57600 бод
		SPEED_115200 = 115200,									///< 115200 бод
		SPEED_128000 = 128000,									///< 128000 бод
		SPEED_230400 = 230400,									///< 230400 бод
		SPEED_256000 = 256000									///< 256000 бод
	};
	/*
	\brief Тип подключения
	*/
	enum class MODBUS_TYPE : std::uint8_t
	{
		RTU = 1,												//< RTU
		TCP = 2,												//< TCP
		TCP_PI = 3												//< TCP PI
	};


}