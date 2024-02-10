#include "ModbusBase.hpp"
#include <modbus-rtu.h>
#include <modbus-tcp.h>

bool ModbusCPP::ModbusBase::canEncode(const BYTE_ORDER encode, const data_array_type::size_type size) const
{
#define FOUR_LETTER_SIZE 4
#define EIGHT_LETTER_SIZE 2 * FOUR_LETTER_SIZE
	if (encode == BYTE_ORDER::ABCD ||
		encode == BYTE_ORDER::BADC ||
		encode == BYTE_ORDER::CDAB ||
		encode == BYTE_ORDER::DCBA)
		if (size != FOUR_LETTER_SIZE)
			return false;
		else if (encode == BYTE_ORDER::ABCDEFGH ||
			encode == BYTE_ORDER::BADCFEHG ||
			encode == BYTE_ORDER::GHEFCDAB ||
			encode == BYTE_ORDER::HGFEDCBA)
			if (size != EIGHT_LETTER_SIZE)
				return false;
			else if (encode != BYTE_ORDER::NONE)
				return false;
#undef FOUR_LETTER_SIZE
#undef EIGHT_LETTER_SIZE
	return true;
}

bool ModbusCPP::ModbusBase::createContext()
{
	close();
	switch (config.getType())
	{
	case MODBUS_TYPE::RTU:
		ctx = modbus_new_rtu(config.deviceOrIP.data(), 
			static_cast<std::uint32_t>(config.baud),
			static_cast<std::uint32_t>(config.parity), 
			static_cast<std::uint32_t>(config.dataBit),
			static_cast<std::uint32_t>(config.stopBit));
	case MODBUS_TYPE::TCP:
		ctx = modbus_new_tcp(config.deviceOrIP.data(), std::stoi(config.port));
	case MODBUS_TYPE::TCP_PI:
		ctx = modbus_new_tcp_pi(config.deviceOrIP.data(), config.port.data());
	default:
		return false;
	}
	if (ctx == nullptr)
		lastError = errno;
	return ctx != nullptr;
}
void ModbusCPP::ModbusBase::resetSlave()
{
	modbus_set_slave(ctx, -1);
}
bool ModbusCPP::ModbusBase::setSlave(const std::int32_t id)
{
	auto operationResult = modbus_set_slave(ctx, id);
	if (operationResult == -1)
		lastError = errno;
	return operationResult != -1;
}

void ModbusCPP::ModbusBase::setConfig(const ModbusConfig& newConfig)
{
	this->config = newConfig;
}

bool ModbusCPP::ModbusBase::acceptConfig()
{
	if (ctx != nullptr)
	{
		if (modbus_set_debug(ctx, config.isDebug) == 0 &&
			(
				(config.respondTime.first == -1 || config.respondTime.second == -1) ||
				modbus_set_response_timeout(ctx,
				static_cast<std::uint32_t>(config.respondTime.first),
				static_cast<std::uint32_t>(config.respondTime.second)) == 0) &&
			((config.indicationTime.first == -1 || config.indicationTime.second == -1) ||
			modbus_set_indication_timeout(ctx,
				static_cast<std::uint32_t>(config.indicationTime.first),
				static_cast<std::uint32_t>(config.indicationTime.second)) == 0))
			return true;
		else
		{
			lastError = errno;
			return false;
		}
	}
	return true;
}
bool ModbusCPP::ModbusBase::acceptConfig(const ModbusConfig& newConfig)
{
	setConfig(newConfig);
	return acceptConfig();
}

ModbusCPP::ModbusConfig ModbusCPP::ModbusBase::getConfig()
{
	return this->config;
}

bool ModbusCPP::ModbusBase::open()
{
	if (ctx != nullptr || !acceptConfig() || !createContext())
		return false;
	return true;
}

void ModbusCPP::ModbusBase::close()
{
	if (ctx != nullptr)
	{
		modbus_flush(ctx);
		modbus_close(ctx);
		modbus_free(ctx);
		ctx = nullptr;
	}
}
ModbusCPP::ModbusBase::error_type ModbusCPP::ModbusBase::errorCode() const
{
	return lastError;
}
std::string ModbusCPP::ModbusBase::what() const
{
	return modbus_strerror(errorCode());
}
bool ModbusCPP::ModbusBase::decode(data_array_type& data, const BYTE_ORDER encode) const
{
	if (canEncode(encode, data.size()) == false)
		return false;

	switch (encode)
	{
	case BYTE_ORDER::ABCDEFGH:
	case BYTE_ORDER::ABCD:
	case BYTE_ORDER::F_ABCD:
	case BYTE_ORDER::NONE:
		break;
	case BYTE_ORDER::CDAB:
		std::rotate(data.begin(), data.begin() + data.size() / 2, data.end());
		break;
	case BYTE_ORDER::GHEFCDAB:
	case BYTE_ORDER::BADCFEHG:
	case BYTE_ORDER::BADC:
	case BYTE_ORDER::F_BADC:
		for (auto i = 0u; i < data.size(); i += 2)
			std::swap(data.at(i), data.at(i + 1));
		if (encode == BYTE_ORDER::BADCFEHG ||
			encode == BYTE_ORDER::BADC)
			break;
	case BYTE_ORDER::DCBA:
	case BYTE_ORDER::F_DCBA:
	case BYTE_ORDER::HGFEDCBA:
		std::reverse(data.begin(), data.end());
		break;
	default:
		return false;
	}
	if (config.needReverseFloat && (encode >= BYTE_ORDER::F_ABCD && encode >= BYTE_ORDER::F_DCBA))
		std::rotate(data.begin(), data.begin() + data.size() / 2, data.end());
	return true;
}
std::vector<std::uint8_t> ModbusCPP::ModbusBase::decode(const data_array_type& data, const BYTE_ORDER encode) const
{
	auto newData = data;
	if (decode(newData, encode) == false)
		newData.clear();
	return newData;
}
bool ModbusCPP::ModbusBase::connect()
{
	auto readyToWork = modbus_connect(ctx) != -1;
	if (readyToWork == false)
		lastError = errno;
	return readyToWork;

}
ModbusCPP::ModbusBase::~ModbusBase()
{
	close();
}
