#pragma once
#include <cstdint>
namespace ModbusCPP
{
	enum class DEVICE_DATA_TYPE : std::uint8_t
	{
		COILS,													///< ����������� ��� (������ ������).
		DISCRETE_INPUT,											///< ����������� ��� (������/������).
		INPUT_REGISTERS,										///< 16-�� ������� ��� (������ ������).
		HOLDING_REGISTERS										///< 16-�� ������� ��� (������/������).
	};
	/**
	\brief ������� � Modbus - �������.
	*/
	enum class BASE_COMMAND : std::uint16_t
	{
		READ_COILS = 0x01,										///< ������ �������� �� ���������� ������.
		READ_DISCRETE_INPUT = 0x02,								///< ������ �������� �� ���������� ���������� ������.
		READ_HOLDING_REGISTERS = 0x03,							///< ������ �������� �� ���������� ��������� ��������.
		READ_INPUT_REGISTERS = 0x04,							///< ������ �������� �� ���������� ��������� �����.

		WRITE_SINGLE_COIL = 0x05,								///< ������ �������� ������ �����.
		WRITE_SINGLE_REGISER = 0x06,							///< ������ �������� � ���� ������� ��������.
		WRITE_MULTIPLE_COILS = 0x0F,							///< ������ �������� � ��������� ��������� ������.
		WRITE_MULTIPLE_REGISERS = 0x10							///< ������ �������� � ��������� ��������� ��������.
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