#include "ModbusConfig.hpp"

namespace ModbusCPP
{
	ModbusConfig& ModbusConfig::setIP(const string_type& ip)&
	{
		this->deviceOrIP = ip;
		return *this;
	}

	ModbusConfig& ModbusConfig::setDevice(const string_type& device)&
	{
		return this->setIP(device);
	}
	ModbusConfig& ModbusConfig::setPort(const string_type& port)&
	{
		this->port = port;
		return *this;
	}
	ModbusConfig& ModbusConfig::setPort(const std::int32_t port)&
	{
		return this->setPort(std::to_string(port));
	}
	ModbusConfig& ModbusConfig::setBaudRate(const BAUD_RATE& baud)&
	{
		this->baud = baud;
		return *this;
	}

	ModbusConfig& ModbusConfig::setParity(const PARITY parity)&
	{
		this->parity = parity;
		return *this;
	}

	ModbusConfig& ModbusConfig::setDataBits(const DATA_BITS dataBit)&
	{
		this->dataBit = dataBit;
		return *this;
	}

	ModbusConfig& ModbusConfig::setStopBits(const STOP_BITS stopBit)&
	{
		this->stopBit = stopBit;
		return *this;
	}

	MODBUS_TYPE ModbusConfig::getType() const
	{
		return this->type;
	}

	ModbusConfig::string_type ModbusConfig::getIP() const
	{
		return this->deviceOrIP;
	}

	ModbusConfig::string_type ModbusConfig::getPort() const
	{
		return this->port;
	}

	ModbusConfig::string_type ModbusConfig::getDevice() const
	{
		return getIP();
	}

	BAUD_RATE ModbusConfig::getBaudRate() const
	{
		return this->baud;
	}

	PARITY ModbusConfig::getParity() const
	{
		return this->parity;
	}

	DATA_BITS ModbusConfig::getDataBits() const
	{
		return this->dataBit;
	}
	STOP_BITS ModbusConfig::getStopBits() const
	{
		return this->stopBit;
	}

	ModbusConfig& ModbusConfig::setDebug(const bool needDebug)&
	{
		this->isDebug = needDebug;
		return *this;
	}

	ModbusConfig& ModbusConfig::setFloatReversing(const bool needReverse)&
	{
		this->needReverseFloat = needReverse;
		return *this;
	}

	ModbusConfig& ModbusConfig::setIndicationTime(const timeout_type& time)&
	{
		this->indicationTime = time;
		return *this;
	}

	ModbusConfig& ModbusConfig::setIndicationTime(const timeout_type::first_type to_sec, const timeout_type::first_type to_usec)&
	{
		return this->setIndicationTime({ to_sec, to_usec });
	}

	ModbusConfig& ModbusConfig::setResponseTime(const timeout_type& time)&
	{
		this->respondTime = time;
		return *this;
	}

	ModbusConfig& ModbusConfig::setResponseTime(const timeout_type::first_type to_sec, const timeout_type::first_type to_usec)&
	{
		return this->setResponseTime({ to_sec, to_usec });
	}

	bool ModbusConfig::getDebug() const
	{
		return isDebug;
	}

	bool ModbusConfig::getFloatReverse() const
	{
		return needReverseFloat;
	}

	ModbusConfig::timeout_type ModbusConfig::getResponseTime() const
	{
		return respondTime;
	}

	ModbusConfig::timeout_type ModbusConfig::getIndicationTime() const
	{
		return indicationTime;
	}

	ModbusConfig::ModbusConfig() = default;

	ModbusConfig::ModbusConfig(const MODBUS_TYPE type)
		:ModbusConfig()
	{
		this->type = type;
	}

	ModbusConfig::ModbusConfig(const string_type& ip, const std::int32_t port)
		:ModbusConfig(MODBUS_TYPE::TCP)
	{
		this->setIP(ip).setPort(port);
	}

	ModbusConfig::ModbusConfig(const string_type& node, const string_type& service)
		:ModbusConfig(MODBUS_TYPE::TCP_PI)
	{
		this->setIP(node).setPort(port);
	}

	ModbusConfig::ModbusConfig(const string_type& device, const BAUD_RATE baud, const PARITY parity, const DATA_BITS dataBit, const STOP_BITS stopBit)
		:ModbusConfig(MODBUS_TYPE::RTU)
	{
		this->setDevice(device).setBaudRate(baud).setParity(parity).setDataBits(dataBit).setStopBits(stopBit);
	}

}