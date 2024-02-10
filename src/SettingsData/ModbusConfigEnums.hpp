#pragma once
#include <cstdint>

namespace ModbusCPP
{
	/**
	\brief ��� ��������.
	*/
	enum class PARITY : std::int8_t
	{
		NONE = 'N',												///< �����������.
		EVEN = 'E',												///< ��������.
		ODD = 'O'												///< ������.
	};
	/**
		\brief ���������� �������� ���.
		*/
	enum class STOP_BITS : std::uint8_t
	{
		ONE = 1,												///< ���� �������� ���.
		TWO = 2													///< ��� �������� ����.
	};
	/**
	\brief ���������� ���, ������������ �� ������.
	*/
	enum class DATA_BITS : std::uint8_t
	{
		FIVE = 5,												///< 5 ���.
		SIX = 6,												///< 6 ���.
		SEVEN = 7,												///< 7 ���.
		EIGHT = 8												///< 8 ���.
	};
	/**
	\brief �������� ��������.
	*/
	enum class BAUD_RATE : std::uint32_t
	{
		SPEED_300 = 300,										///< 300 ���
		SPEED_600 = 600,										///< 600 ���
		SPEED_1200 = 1200,										///< 1200 ���
		SPEED_2400 = 2400,										///< 2400 ���
		SPEED_4800 = 4800,										///< 4800 ���
		SPEED_9600 = 9600,										///< 9600 ���
		SPEED_14400 = 14400,									///< 14400 ���
		SPEED_19200 = 19200,									///< 19200 ���
		SPEED_38400 = 38400,									///< 38400 ���
		SPEED_56000 = 56000,									///< 56000 ���
		SPEED_57600 = 57600,									///< 57600 ���
		SPEED_115200 = 115200,									///< 115200 ���
		SPEED_128000 = 128000,									///< 128000 ���
		SPEED_230400 = 230400,									///< 230400 ���
		SPEED_256000 = 256000									///< 256000 ���
	};
	/*
	\brief ��� �����������
	*/
	enum class MODBUS_TYPE : std::uint8_t
	{
		RTU = 1,												//< RTU
		TCP = 2,												//< TCP
		TCP_PI = 3												//< TCP PI
	};


}