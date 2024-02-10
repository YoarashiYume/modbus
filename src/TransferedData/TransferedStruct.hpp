#pragma once
#include "TransferedDataEnums.hpp"
#include <vector>

namespace ModbusCPP
{
	/**
	\brief ������� ������������� ������������� �������-������
	*/
	struct SendData
	{
		BASE_COMMAND command{};	///< ��� Modbus-�������
		std::uint32_t startBit{};		///< ��������� ��� �������
		/**
		\brief ����������
		*/
		virtual ~SendData() = default;
	};
	/**
	\brief ��������� ��� Modbus-������� 0x05 � 0x06
	*/
	struct SimpleSendData : SendData
	{
		std::uint16_t sendValue{};///< ������������ ��������
	};
	/**
	\brief ��������� ��� Modbus-������� 0x0F � 0x10
	*/
	struct MultipleSendData : SendData
	{
		std::vector<std::uint8_t> sendData;	///< ������������ �������� (� ���� ������ ����)
		std::int32_t count{};				///< ���������� ������������ ��������
		BYTE_ORDER order{ BYTE_ORDER::NONE };///< ����������� ������� ����
	};
	/**
	\brief ��������� ������������ ��� ���������� ���� ��������� ����������
	*/
	struct DeviceAnswer
	{
		std::uint32_t posInDevice;	///< ��������� ��� � ����������
		std::vector<std::uint8_t> data; ///< ������ �� ����������
	};
	/**
	\brief ���������, �������� ��������� �������� ����������
	*/
	struct DeviceInfo
	{
		DEVICE_DATA_TYPE dataType;///< ��� ������
		BYTE_ORDER byteOrder{ BYTE_ORDER::NONE };///< ������� ���� ������
		std::uint32_t startBit{};		///< ��������� ��� ������
		std::uint32_t sizeInByte{};		///< ������ ������ � ������
		std::uint32_t countOfElements{};///< ���������� ��������� ������
	};
	/**
	\brief ��������� ������������ ��� ������������� ��������� ����������
	*/
	struct SlaveData
	{
		/**
		\brief ��������������� ��������� ������������ ��� ������������� ������ � ��������� ����������
		*/
		struct Data
		{
			std::uint32_t startBits{};///< ��������� �����
			std::uint32_t count{};///< ���������� ���������
			std::vector<std::uint8_t> data;///< �������� ���������
			std::vector<std::uint8_t> countOfDataOnElement; ///< ���������� ���� �� �������
			std::vector<BYTE_ORDER> order;///< ������� ������ ��������� (������������ ��� ��������� ����� � ��������)
		};
		Data coil;///< ������ ��� ������������� ��������� ������ 
		Data inputCoil;///< ������ ��� ������������� ���������� ������
		Data inputRegisters;///< ������ ��� ������������� ��������� ������ 
		Data holdingRegisters;///< ������ ��� ������������� ��������� ������ 
	};


}