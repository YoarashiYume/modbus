#pragma once
#include "../../SettingsData/Config/ModbusConfig.hpp"
#include "../../TransferedData/TransferedDataEnums.hpp"
#include <modbus.h>
#include <vector>
namespace ModbusCPP
{
	/**
	\brief ������� ����� ��� ������� �� ��������� Modbus
	*/
	class ModbusBase
	{
	public:
		/**
		\brief ������� ���� ������.
		*/
		using error_type = std::int32_t;
		using data_array_type = std::vector<std::uint8_t>;
	private:
		
		bool canEncode(const BYTE_ORDER encode, const data_array_type::size_type size) const;
		bool createContext();
	protected:
		error_type lastError{};
		modbus_t* ctx; ///< �������� modbus 
		ModbusConfig config;
		/**
		\brief ����� ������������ ID ��������/�������� ����������
		*/
		void resetSlave();
		/**
		\brief ����� ��������������� ID ��������/�������� ����������
		\param id ��������������� ID
		*/
		bool setSlave(const std::int32_t id);

	public:
		/**
		\brief ����� ��������������� ����� config
		\param[in] newConfig ����� config
		*/
		void setConfig(const ModbusConfig& newConfig);
		/**
		\brief ����� ����������� ��������� �� config`�
		\return ��������� ���������� ����� ��������
		*/
		bool acceptConfig();
		/**
		\brief ����� ��������������� ����� config � ����������� ��������� �� ����
		\param[in] newConfig ����� config
		\return ��������� ���������� ����� ��������
		*/
		bool acceptConfig(const ModbusConfig& newConfig);
		/**
		\brief ����� ������������ ������� config
		\return config
		*/
		ModbusConfig getConfig();
		/**
		\brief ����� ��������� ��������
		\return ���� �������� ��� ������ - true, ����� false
		*/
		bool open();
		/**
		\brief ����� ����������� Modbus-����������
		*/
		void close();
		/**
		\brief ����� ��������������� Modbus-����������
		\return ���� ���������� ���� ����������� true, ����� false
		*/
		bool connect();
		/**
		\brief ����� ������������ ��������� ������
		�������������� ������� - errno
		\return ��� ��������� ������
		*/
		error_type errorCode() const;
		/**
		\brief ����� ������������ ��������� ������������� ��������� ������
		\return  ��������� ������������� ��������� ������
		*/
		std::string what() const;
		/**
		\brief ����� ���������� ������� ���� � ���������/���������� ������
		\param[in] data ���������� ������
		\param[in] encode ������� ������� ���� (��� ���������� � ���� ABCD) ��� ����������� ������� (���� ������ � ���� ABCD)
		\return � ������ ������ ���������� ������ � ���������� �������� ����, ����� ������ ������
		*/
		data_array_type decode(const data_array_type& data, const BYTE_ORDER encode) const;
		/**
		\brief ����� ���������� ������� ���� � ���������/���������� ������
		\param[in, out] data ���������� ������
		\param[in] encode ������� ������� ���� (��� ���������� � ���� ABCD) ��� ����������� ������� (���� ������ � ���� ABCD)
		\return ���� ��������� ������� ������� - true, ����� false
		*/
		bool decode(data_array_type& data, const BYTE_ORDER encode) const;
		virtual ~ModbusBase();
	};

}