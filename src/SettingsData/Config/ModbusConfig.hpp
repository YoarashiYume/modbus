#pragma once
#include <string>
#include "../ModbusConfigEnums.hpp"

namespace ModbusCPP
{
	struct ModbusConfig
	{
	public:
		using string_type = std::string;
		using timeout_type = std::pair<std::int32_t, std::int32_t>;
	private:
		MODBUS_TYPE type;
		
	public:
		string_type deviceOrIP;
		string_type port;
		BAUD_RATE baud{};
		PARITY parity{};
		DATA_BITS dataBit{};
		STOP_BITS stopBit{};
		bool isDebug{ false };
		bool needReverseFloat{false};
		timeout_type respondTime{-1, -1};
		timeout_type indicationTime{ -1, -1 };


		ModbusConfig& setIP(const string_type& ip)&;
		ModbusConfig& setDevice(const string_type& device)&;
		ModbusConfig& setPort(const string_type& port)&;
		ModbusConfig& setPort(const std::int32_t port)&;
		ModbusConfig& setBaudRate(const BAUD_RATE& baud)&;
		ModbusConfig& setParity(const PARITY parity)&;
		ModbusConfig& setDataBits(const DATA_BITS dataBit)&;
		ModbusConfig& setStopBits(const STOP_BITS stopBit)&;

		MODBUS_TYPE getType() const;
		string_type getIP() const;
		string_type getPort() const;
		string_type getDevice() const;
		BAUD_RATE getBaudRate()const;
		PARITY getParity()const;
		DATA_BITS getDataBits()const;
		STOP_BITS getStopBits()const;

		ModbusConfig& setDebug(const bool needDebug)&;
		ModbusConfig& setFloatReversing(const bool needReverse)&;
		ModbusConfig& setIndicationTime(const timeout_type& time)&;
		ModbusConfig& setIndicationTime(const timeout_type::first_type to_sec, const timeout_type::first_type to_usec)&;
		ModbusConfig& setResponseTime(const timeout_type& time)&;
		ModbusConfig& setResponseTime(const timeout_type::first_type to_sec, const timeout_type::first_type to_usec)&;

		bool getDebug() const;
		bool getFloatReverse() const;
		timeout_type getResponseTime() const;
		timeout_type getIndicationTime() const;

		ModbusConfig();
		ModbusConfig(const MODBUS_TYPE type);
		ModbusConfig(const string_type& ip, const std::int32_t port);
		ModbusConfig(const string_type& node, const string_type& service);
		ModbusConfig(const string_type& device, const BAUD_RATE baud, const PARITY parity, const DATA_BITS dataBit, const STOP_BITS stopBit);

	};
}