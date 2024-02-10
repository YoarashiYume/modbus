#pragma once
#include <cstdint>
namespace ModbusCPP
{
	enum class DEVICE_DATA_TYPE : std::uint8_t
	{
		COILS,													///< Однобитовый тип (только чтение).
		DISCRETE_INPUT,											///< Однобитовый тип (чтение/запись).
		INPUT_REGISTERS,										///< 16-ти битовый тип (только чтение).
		HOLDING_REGISTERS										///< 16-ти битовый тип (чтение/запись).
	};
	/**
	\brief Команда в Modbus - запросе.
	*/
	enum class BASE_COMMAND : std::uint16_t
	{
		READ_COILS = 0x01,										///< чтение значений из нескольких флагов.
		READ_DISCRETE_INPUT = 0x02,								///< чтение значений из нескольких дискретных входов.
		READ_HOLDING_REGISTERS = 0x03,							///< чтение значений из нескольких регистров хранения.
		READ_INPUT_REGISTERS = 0x04,							///< чтение значений из нескольких регистров ввода.

		WRITE_SINGLE_COIL = 0x05,								///< запись значения одного флага.
		WRITE_SINGLE_REGISER = 0x06,							///< запись значения в один регистр хранения.
		WRITE_MULTIPLE_COILS = 0x0F,							///< запись значений в несколько регистров флагов.
		WRITE_MULTIPLE_REGISERS = 0x10							///< запись значений в несколько регистров хранения.
	};
	enum class BYTE_ORDER : std::uint16_t
	{
		NONE,
		ABCD,
		CDAB,
		BADC,
		DCBA,
		ABCDEFGH,
		GHEFCDAB,
		BADCFEHG,
		HGFEDCBA,

		F_ABCD,
		F_CDAB,
		F_BADC,
		F_DCBA

	};
}