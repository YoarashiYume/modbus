#pragma once
#include <unordered_map>

#include "../../Base/ModbusBase.hpp"
#include "../../../TransferedData/TransferedStruct.hpp"
namespace ModbusCPP
{
	/**
	\brief �����, �������������� ������� ���������� Modbus
	*/
	class ModbusMaster : public ModbusBase
	{
	public:
		using key_type = std::uint32_t;
		using value_type = DeviceInfo;
		using storage_type = std::unordered_map<key_type, std::vector<value_type>>;
		using data_type = std::vector<std::uint8_t>;
		using read_function_signature = std::pair<bool, data_type>(ModbusMaster::*)(const std::int32_t, const std::int32_t, data_type::value_type*);
	private:
		storage_type connectedDevice;///< ������ ������������ ���������
		/**
		\brief ����� ��� ���������� coil`�� �� ��������� ����������
		\param[in] deviceData ��������� ��� ���������� ������
		\param[in] readFunction ������� ��� ���������� coil`��
		\param[in, out] isCorrect ���������� ��� ������ ���������� ����������
		\return ��������� ����������
		*/
		DeviceAnswer readCoil(const DeviceInfo deviceData, read_function_signature readFunction, bool& isCorrect);
		/**
		\brief ����� ��� ���������� register`�� �� ��������� ����������
		\param[in] deviceData ��������� ��� ���������� ������
		\param[in] offset ������ �� ���������� ������������ � ��������� ���������
		\param[in] readFunction ������� ��� ���������� register`��
		\param[in, out] isCorrect ���������� ��� ������ ���������� ����������
		\return ��������� ����������
		*/
		DeviceAnswer readRegisters(const DeviceInfo deviceData,const std::uint32_t offset, read_function_signature readFunction, bool& isCorrect);
		/**
		\brief ����� ��� ���������� ��������� ������ �� �������� ����������
		\param startBit ��������� ���, � �������� ����� ����������� ������
		\param count ���������� ����������� ������
		\param[out] data ������������ ��������, ��� �������� �������� ���������� ������ ����� ����������� � ������, ������� ���������, ������
		\return ���� <bool, ������ ����>. ������ �������� ������ ���������� ����������� ��������. ������ �������� ������ ��������� ������ ������ (���� �������� data == nullptr)
		*/
		std::pair<bool, data_type> readCoil(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
		/**
		\brief ����� ��� ���������� ���������� ������ �� �������� ����������
		\param startBit ��������� ���, � �������� ����� ����������� ������
		\param count ���������� ����������� ������
		\param[out] data ������������ ��������, ��� �������� �������� ���������� ������ ����� ����������� � ������, ������� ���������, ������
		\return ���� <bool, ������ ����>. ������ �������� ������ ���������� ����������� ��������. ������ �������� ������ ��������� ������ ������ (���� �������� data == nullptr)
		*/
		std::pair<bool, data_type> readDiscreteInput(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
		/**
		\brief ����� ��� ���������� ��������� ����� �� �������� ����������
		\param startBit ��������� ���, � �������� ����� ����������� ������
		\param count ���������� ����������� ������
		\param[out] data ������������ ��������, ��� �������� �������� ���������� ������ ����� ����������� � ������, ������� ���������, ������
		\return ���� <bool, ������ ����>. ������ �������� ������ ���������� ����������� ��������. ������ �������� ������ ��������� ������ ������ (���� �������� data == nullptr)
		*/
		std::pair<bool, data_type> readInputRegisters(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
		/**
		\brief ����� ��� ���������� ��������� �������� �� �������� ����������
		\param startBit ��������� ���, � �������� ����� ����������� ������
		\param count ���������� ����������� ������
		\param[out] data ������������ ��������, ��� �������� �������� ���������� ������ ����� ����������� � ������, ������� ���������, ������
		\return ���� <bool, ������ ����>. ������ �������� ������ ���������� ����������� ��������. ������ �������� ������ ��������� ������ ������ (���� �������� data == nullptr)
		*/
		std::pair<bool, data_type> readHoldingsRegisters(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
	public:
		/**
		\brief ����� ����������� ���������� � ������� ����������(��� ���������)
		\param id ID �������� ����������
		\param deviceInfo ���������� � ������� ����������
		*/
		void addDevice(const key_type id, const value_type& deviceInfo);
		/**
		\brief ����� ����������� ���������� � ������� ����������(��� ���������)
		\param id ID �������� ����������
		\param deviceInfo ���������� � ������� ����������
		*/
		void addDevice(const key_type id, const std::vector<value_type>& deviceInfo);
		/**
		\brief ����� ��������� ���������� � ������� ����������
		\param id ID �������� ����������
		\return ��������� ������
		*/
		std::pair<bool, std::vector<value_type>> removeDevice(const key_type id);
		/**
		\brief ����� ����������� ����������� ��������� �������� ����������
		\param id ID �������� ����������
		\param deviceInfo ���������� � ������� ����������
		*/
		std::vector<value_type> getDevice(const key_type id) const;
		/**
		\brief �����, ����������� ������� ������ �� ����������
		������ ����� ���������� ��������� �������� ����������
		\param id ID ����������, �� �������� ����� ����������� ����������
		\return ���� <bool, ������ ����������>. ������ �������� ������ ���������� ����������� �������� (���� �������� ��������� ���������� �� �������������� - ������ false).  ������ �������� ������ ������ ������� ����������, ���� ��� ���� ������� (���� ����� ������ ������ �������� - ������� � ���������)
		*/
		std::pair<bool, std::vector<DeviceAnswer>> read(const key_type id);
		/**
		\brief �����, ����������� �������� ������ � ����������
		\param id ID ����������, � ������� ����� ����������� ������
		\param data ������������ ������
		\return ���������� true ���� ������ ���� �������, ����� false
		*/
		bool write(const key_type id, const SendData& data);
		/**
		\brief �����, ����������� �������� ������ � ����������
		\param id ID ����������, � ������� ����� ����������� ������
		\param data ������ ������������ ������
		\return ���������� true ���� ������ ���� �������, ����� false
		*/
		bool write(const key_type id, const std::vector<SendData>& data);
		/**
		\brief ����� ����������� ��������� raw-������ � �������� ����������
		\param id ID ����������, �������� ����� ��������� ������
		\param request raw-������
		\return ��������� true, ���� ������ ��� �������. ����� false
		*/
		bool writeRawRequest(const key_type id, const data_type request);
		/**
		\brief �����, ����������� �������� ������ � ����������
		\param id ID ����������, � ������� ����� ����������� ������
		\param commandID ��� Modbus-�������
		\param data ������������ ������
		\param startBit ��������� �����, ��� ���������� �������
		\param count ���������� ���������, ������������� Modbus-��������
		\return ��� ���������� �������� ���������� true, ����� - false
		*/
		bool rawWrite(const key_type id, const BASE_COMMAND commandID, const data_type& data,
			const std::int32_t startBit, const std::int32_t count);
		template<typename T,
			typename = typename std::enable_if<
			std::is_unsigned<T>::value &&
			sizeof(T) <= sizeof(std::uint16_t)
		>::type>
		/**
		\brief �����, ����������� �������� ������ � ����������
		\param id ID ����������, � ������� ����� ����������� ������
		\param commandID ��� Modbus-�������
		\param data ������������ ������
		\param startBit ��������� �����, ��� ���������� �������
		\return ��� ���������� �������� ���������� true, ����� - false
		*/
		bool rawWrite(const key_type id, const BASE_COMMAND commandID, const T data,
			const std::int32_t startBit)
		{
			std::int32_t operationResult = 0;
			modbus_set_slave(this->ctx, id);
			switch (commandID)
			{
			case BASE_COMMAND::WRITE_SINGLE_COIL:
				operationResult = modbus_write_bit(this->ctx, startBit, data);
				if (operationResult == -1)
					lastError = errno;
				break;
			case BASE_COMMAND::WRITE_SINGLE_REGISER:
				operationResult = modbus_write_register(this->ctx, startBit, static_cast<std::uint16_t>(data));
				if (operationResult == -1)
					lastError = errno;
				break;
			default:
				operationResult = -1;
				break;
			}
			this->resetSlave();
			return operationResult != -1;
		}
		/**
		\brief �����, ����������� ������� ������ �� ����������
		� ������ ������ ��������� ���������� �� ������������
		\param id ID ����������, �� �������� ����� ����������� ����������
		\param commandID ��� Modbus-�������
		\param startBit ��������� �����, ��� ���������� �������
		\param count ���������� ���������, ������������� Modbus-��������
		\return ���� <bool, ������ ����>.  ������ �������� ������ ���������� ����������� ��������. ������ �������� ������ ��������� ������ ������.
		*/
		std::pair<bool, data_type> rawRead(const key_type id, const BASE_COMMAND commandID,
			const std::int32_t startBit, const std::int32_t count);
	};
}