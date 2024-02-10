#pragma once
#include <array>
#include "../../Base/ModbusBase.hpp"
#include "../../../TransferedData/TransferedStruct.hpp"
#include "../../../TransferedData/ModbusData.hpp"
namespace ModbusCPP
{
	/**
	\brief ������ Modbus.
	*/
	enum class MODBUS_ERROR : std::int32_t
	{
		OK,																	///< �� ������.
		ILLEGAL_FUNCIONT = MODBUS_EXCEPTION_ILLEGAL_FUNCTION,				///< �������� ��� ������� �� ����� ���� ���������.
		ILLEGAL_DATA_ADDRESS = MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,		///< ����� ������, ��������� � �������, ����������.
		ILLEGAL_DATA_VALUE = MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,			///< ����� ������, ��������� � �������, ����������.
		SLAVE_OR_SERVER_FAILURE = MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE,	///< ������������������� ������ ����� �����, ���� ������� ���������� �������� ��������� ������������� ��������.
		ACKNOWLEDGE = MODBUS_EXCEPTION_ACKNOWLEDGE,							///< ������� ���������� ������� ������ � ������������ ���, �� ��� ������� ����� ������� (��������� Timeout).
		SLAVE_OR_SERVER_BUSY = MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,		///< ������� ���������� ������ ���������� �������.
		NEGATIVE_ACKNOWLEDGE = MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,		///< ������� ���������� �� ����� ��������� ����������� �������, �������� � �������.
		MEMORY_PARITY = MODBUS_EXCEPTION_MEMORY_PARITY,						///< ������� ���������� ��� ������ ����������� ������ ���������� ������ �������� ��������.
		NOT_DEFINED = MODBUS_EXCEPTION_NOT_DEFINED,							///< �������������� ������.
		GATEWAY_PATH = MODBUS_EXCEPTION_GATEWAY_PATH,						///< ������ ���� �����.
		GATEWAY_TARGET = MODBUS_EXCEPTION_GATEWAY_TARGET,					///< ������ ���� �����.
		MAX = MODBUS_EXCEPTION_MAX											///< ������������ �������� ������.
	};

	/**
	\brief ������������ �������� � ����������� ������� Modbus
	������� �� ��������� ����� ������� � ���� ����������� ������.
	���� ����������� ������_1 = �����_��������� - 1.
	���� ����������� ������_2 = �����_��������� - 2.
	*/
	enum class MODBUS_REQUEST_OFFSETS : unsigned int
	{
		FUNCTION_OFFSET = 0,				///< ������ �� ���� �������.
		ADDRESS_OFFSET = 1,					///< ������ �� ������ ���������� ��������.
		COUNT_OF_ELEMENT_OFFSET = 3,		///< ������ �� ���������� ���������.
		DATA_ON_SINGLE_WRITE_OFFSET = 3,	///< ������ �� ������ ������������� ��������, ��� ����� �������� 0x05 � 0x06.
		DATA_ON_MULTIPLE_WRITE_OFFSET = 6,	///< ������ �� ������ ������������ ���������, ��� ����� �������� 0x15 � 0x16.
		COUNT_OF_BYTE_OFFSET = 5			///< ������ �� ���������� ���� ������, ��� ����� �������� 0x15 � 0x16.
	};
	/**
	\brief ������������ ���������� ����, ���������� ���������������� �������.
	���������� ���� ������, ��� ������������� ������ (0x15 � 0x16) ����������� � �������, � �������� MODBUS_REQUEST_OFFSETS::COUNT_OF_BYTE_OFFSET
	*/
	enum class MODBUS_REQUEST_ELEMENT_BYTE_COUNT : unsigned int
	{
		ADDRESS_SIZE = 2,					///< ���������� ���� ��� ������ ���������� ������ ���������.
		COUNT_OF_ELEMENT_SIZE = 2,			///< ���������� ���� ��� ������ ���������� �������������/���������� ���������.
		DATA_ON_SINGLE_WRITE_SIZE = 2,		///< ���������� ���� ��� ������ ������������� ��������, ��� ����� �������� 0x05 � 0x06.
		COUNT_OF_BYTE_SIZE = 1				///< ���������� ���� ��� ������ ������ ������������ �������, ��� ����� �������� 0x15 � 0x16.
	};

	/**
	\brief ��������� ������������ ��� ������������� ��������� ����������
	*/
	class ModbusSlave : public ModbusBase
	{
	public:
		using it_type = std::int32_t;
		using device_info_type = std::vector<ModbusData>;
		using initial_device_type = SlaveData;
		using message_type = std::array<uint8_t, MODBUS_RTU_MAX_ADU_LENGTH>;
		// ���� ����� ��������� ���������������� �������� ������ �� ������ ���� ��������, �� ������� ������ ������� MODBUS_ERROR != MODBUS_ERROR::OK, � ���� ������ MODBUS_ERROR::OK
		using callback_type = MODBUS_ERROR(*)(ModbusSlave*);
		/**
		\brief ������������ ����������� ������� �������� �������
		*/
		enum class LISTEN_RESULT : std::int8_t
		{
			FAILED = -1,///< ������ �� ������
			IGNORED_DATA,///< ������ �� �� ��� ����������, ���������� ���������������
			SUCCSES///< ������ �������
		};
	private:
		it_type currentID; ///< ID �������� ����������
		device_info_type data;///< ��������� ������������� �������� ����������
		message_type query;///< ��������� ������ � �������� ���������� Modbus
		std::uint32_t headerSize{},///< ������ ������ ���������
			messageSize{};///< ������ ���������� ���������
		callback_type callback;///< ���������������� �������, ���������� ��� ������ �� ��������� ������

		/**
		\brief ���������� coil`� �� ������� ������
		\param[in] src ������ ��� �����������
		\param[in, out] dest ����� ���������� ������
		*/
		void coilCopy(const SlaveData::Data& src, uint8_t*& dest);
		/**
		\brief ���������� register`�� �� ������� ������
		\param[in] src ������ ��� �����������
		\param[in, out] dest ����� ���������� ������
		\param[in, out] destEncode ����� ���������� ������� ������
		*/
		bool coilRegisters(const SlaveData::Data& src, uint16_t*& dest, std::vector<BYTE_ORDER>& destEncode);
		/**
		\brief ����� ��������������� ID ���������� � ������������ ������ ������
		*/
		void setId();
		/**
		\brief ����� �������������� ��������� ������ ����� ���������������� �������
		\param[out] error ������� ��������� �������
		\return ��������� �� ������������� ������ � �������
		*/
		modbus_mapping_t* handleRequest(MODBUS_ERROR& error);
		/**
		\brief ����� ��� ������ ������ �� ������� � ����� ������� 0x01-0x04
		\param[out] error ������� ��������� �������
		\param what ������������� ��� ������ � �������
		\param startBit ��������� ��� �� �������
		\param count ���������� �������� �� �������
		\return ��������� �� ������������� ������ � �������
		*/
		modbus_mapping_t* findData(MODBUS_ERROR& error, const DEVICE_DATA_TYPE what, const std::uint16_t startBit, const std::uint16_t count);
	public:
		ModbusSlave(const it_type id);
		/**
		\brief ����� ������������ ��� ������������� Modbus-����������(���������� ������ � ��� ���������)
		\param newData ����������� ���������� � ���������
		\return true, ���� ������ ���� ���������. ����� false.
		*/
		bool addData(const initial_device_type& newData);
		/**
		\brief ����� ������������ ��� ������������� Modbus-����������(���������� ������ � ��� ���������)
		\param newData ����������� ���������� � ���������
		\return true, ���� ��� ������ ���� ���������. ����� false.
		*/
		bool addData(const std::vector<initial_device_type>& newData);
		/**
		\brief ����� ������������ �������� ��������� ����������
		\return ��������� ����������
		*/
		const device_info_type& getDeviceData() const&; //l-value only
		/**
		\brief ����� ������� ��������� Modbus-����������
		\return ��������� ��������� Modbus-����������
		*/
		device_info_type removeData();
		/**
		\brief ����� ��������� Modbus-������ � ����������
		\return ��������� �������� ��������� (��������/������/������������ ������)
		*/
		LISTEN_RESULT listen();
		/**
		\brief ����� ����������� ���������������� �������-��������� � ������������ ����� �� ���������� ������
		\return true, ���� ����� ��� ������� ���������, ����� false
		*/
		bool sendAnswer();
		/**
		\brief ����� ��������������� ���������������� �������-��������� �������
		\param cb ���������������� �������-����������
		*/
		void setCallback(callback_type cb);
		/**
		\brief ����� ������������ ��������� Modbus-������
		\return ��������� Modbus-������
		*/
		const message_type& getMessage()&; //l-value only
		/**
		\brief ����� ������������ ������ ������ ���������
		\return ������ ������ ���������
		*/
		std::uint32_t getHeaderSize() const;
		/**
		\brief ����� ������������ ��������� ��� �� �������
		\return ��������� ��� �� �������
		*/
		std::uint16_t getRequestStartBit() const;
		/**
		\brief ����� ������������ ��� ������� �� �������
		\return ��� ������� �� �������
		*/
		BASE_COMMAND getRequestFunction() const;
		/**
		\brief ����� ������������ ���������� ���������, ������������� ��������
		\return ���������� ���������, ������������� ��������
		*/
		std::uint16_t getRequestCountOfElements() const;
		/**
		\brief ����� ������������ ������ ���������� ������ � �������
		\return ������ ���������� ������ � �������
		*/
		std::uint8_t getRequestCountOfData() const;
		/**
		\brief ����� ������������ ���������� � �������
		\return ���������� ������ � �������
		*/
		const std::vector<std::uint8_t> getRequestData() const;
		/**
		\brief ����� ������������ ������ ���������� �������
		\return ������ ���������� �������
		*/
		std::uint32_t getMessageSize() const;
	};
}