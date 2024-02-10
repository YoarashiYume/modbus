#include "ModbusMaster.hpp"


std::pair<bool, ModbusCPP::ModbusMaster::data_type> ModbusCPP::ModbusMaster::readCoil(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data)
{
	data_type res(count, 0);
	auto operationResult = modbus_read_bits(this->ctx, startBit, count, data == nullptr ? res.data() : data);
	if (operationResult == -1)
		lastError = errno;
	return{ operationResult != -1, data == nullptr ? std::move(res) : data_type{} };
}

std::pair<bool, ModbusCPP::ModbusMaster::data_type> ModbusCPP::ModbusMaster::readDiscreteInput(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data)
{
	data_type res(count, 0);
	auto operationResult = modbus_read_input_bits(this->ctx, startBit, count, data == nullptr ? res.data() : data);
	if (operationResult == -1)
		lastError = errno;
	return{ operationResult != -1, data == nullptr ? std::move(res) : data_type{} };
}

std::pair<bool, ModbusCPP::ModbusMaster::data_type> ModbusCPP::ModbusMaster::readInputRegisters(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data)
{
	data_type res(2 * count, 0); // Элементы sizeof(std::uint16_t), а sizeof(decltype(data_type)::value_type) == sizeof(std::uint8_t) => res.size() = 2 * count
	auto operationResult = modbus_read_input_registers(this->ctx, startBit, count, data == nullptr ? static_cast<uint16_t*>(static_cast<void*>(res.data())) :
		static_cast<uint16_t*>(static_cast<void*>(data)));
	if (operationResult == -1)
		lastError = errno;
	return{ operationResult != -1, data == nullptr ? std::move(res) : data_type{} };
}

std::pair<bool, ModbusCPP::ModbusMaster::data_type> ModbusCPP::ModbusMaster::readHoldingsRegisters(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data)
{
	data_type res(2 * count, 0); // Элементы sizeof(std::uint16_t), а sizeof(decltype(data_type)::value_type) == sizeof(std::uint8_t) => res.size() = 2 * count
	auto operationResult = modbus_read_registers(this->ctx, startBit, count, data == nullptr ? static_cast<uint16_t*>(static_cast<void*>(res.data())) :
		static_cast<uint16_t*>(static_cast<void*>(data)));
	if (operationResult == -1)
		lastError = errno;
	return{ operationResult != -1, data == nullptr ? std::move(res) : data_type{} };
}
void ModbusCPP::ModbusMaster::addDevice(const key_type id, const value_type& deviceInfo)
{
	if (connectedDevice.find(id) == connectedDevice.end())
		connectedDevice.emplace(id, std::vector<value_type>{ deviceInfo });
	else
		connectedDevice.at(id).emplace_back(deviceInfo);
}

void ModbusCPP::ModbusMaster::addDevice(const key_type id, const std::vector<value_type>& deviceInfo)
{
	if(connectedDevice.find(id) == connectedDevice.end())
		connectedDevice.emplace(id, deviceInfo);
	else
		connectedDevice.at(id).insert(connectedDevice.at(id).end(), deviceInfo.begin(), deviceInfo.end());
}
std::pair<bool, std::vector<ModbusCPP::ModbusMaster::value_type>> ModbusCPP::ModbusMaster::removeDevice(const key_type id)
{
	auto iterToData = connectedDevice.find(id);
	if (iterToData == connectedDevice.end())
		return{ false,{} };
	std::pair<bool, std::vector<value_type>> result = { true, iterToData->second };

	connectedDevice.erase(iterToData);

	return result;
}

std::vector<ModbusCPP::ModbusMaster::value_type> ModbusCPP::ModbusMaster::getDevice(const key_type id) const
{
	auto iter = connectedDevice.find(id);
	if (iter == connectedDevice.end())
		return{};
	return iter->second;
}

ModbusCPP::DeviceAnswer ModbusCPP::ModbusMaster::readCoil(const DeviceInfo deviceData, read_function_signature readFunction, bool& isCorrect)
{
	//Попытка чтения
	auto readResult = (this->*readFunction)(deviceData.startBit, deviceData.countOfElements, nullptr);
	isCorrect &= readResult.first;
	//При успешном чтении - запись данных, иначе - пустой массив
	return DeviceAnswer{ deviceData.startBit, readResult.first ?
		std::move(readResult.second) : ModbusMaster::data_type{} };
}
ModbusCPP::DeviceAnswer ModbusCPP::ModbusMaster::readRegisters(const DeviceInfo deviceData, const std::uint32_t offset, read_function_signature readFunction, bool& isCorrect)
{
	//Попытка чтения
	auto readResult = (this->*readFunction)(deviceData.startBit + offset, deviceData.sizeInByte / sizeof(std::uint16_t), nullptr);
	isCorrect &= readResult.first;
	if (readResult.first &&
		decode(readResult.second, deviceData.byteOrder))
		//Если чтение и "декодировака" успешны - записываем 
		return DeviceAnswer{ deviceData.startBit + offset, std::move(readResult.second) };
	else
		//Иначе - пустой массив
		return DeviceAnswer{ deviceData.startBit + offset,{} };
}

std::pair<bool, std::vector<ModbusCPP::DeviceAnswer>> ModbusCPP::ModbusMaster::read(const key_type id)
{
	auto iterToData = connectedDevice.find(id);
	if (iterToData == connectedDevice.end())
		return{ true,{} };

	std::vector<DeviceAnswer> readedData;
	readedData.reserve(iterToData->second.size());


	std::pair<bool, ModbusMaster::data_type> readResult;
	bool totalResult = true;
	if (this->setSlave(id) == true)
	{
		//Проход по всей структуре
		for (auto& dataStruct : iterToData->second)
		{
			switch (dataStruct.dataType)
			{
			case DEVICE_DATA_TYPE::COILS:
				readedData.emplace_back(readCoil(dataStruct, &ModbusMaster::readCoil, totalResult));
				break;
			case DEVICE_DATA_TYPE::DISCRETE_INPUT:
				readedData.emplace_back(readCoil(dataStruct, &ModbusMaster::readDiscreteInput, totalResult));
				break;
			case DEVICE_DATA_TYPE::INPUT_REGISTERS:
				//Чтение для каждого регистра
				for (auto i = 0u; i < dataStruct.countOfElements; ++i)
					readedData.emplace_back(readRegisters(dataStruct,i, &ModbusMaster::readInputRegisters, totalResult));
				break;
			case DEVICE_DATA_TYPE::HOLDING_REGISTERS:
				//Чтение для каждого регистра
				for (auto i = 0u; i < dataStruct.countOfElements; ++i)
					readedData.emplace_back(readRegisters(dataStruct, i, &ModbusMaster::readHoldingsRegisters, totalResult));
				break;
			default:
				totalResult = false;
				break;
			}
		}
	}

	this->resetSlave();
	return{ totalResult, std::move(readedData) };
}
bool ModbusCPP::ModbusMaster::write(const key_type id, const SendData& data)
{
	auto simpleData = dynamic_cast<SimpleSendData*>(&const_cast<SendData&>(data));
	if (simpleData != nullptr) //Если запись с кодом 0x05 и 0x06
		return this->rawWrite<std::uint16_t>(id, simpleData->command, simpleData->sendValue, simpleData->startBit);
	auto multipleData = dynamic_cast<MultipleSendData*>(&const_cast<SendData&>(data));
	if (multipleData != nullptr)//Если запись с кодом 0x05 и 0x06
	{
		std::vector<std::uint8_t> resultArray;//Записываемый массив
		resultArray.reserve(multipleData->sendData.size());
		std::vector<std::uint8_t> arrayOnValue;//Массив для перестановки байт отдельного числа
		auto elSize = multipleData->sendData.size() / multipleData->count;
		for (auto i = 0; i < multipleData->count; ++i)
		{
			arrayOnValue.assign(multipleData->sendData.begin() + i * elSize, multipleData->sendData.begin() + (i + 1) * elSize);
			if (decode( arrayOnValue, multipleData->order) == false)//Отправка происходит только при успешном преобразовании всего массива
				return false;
			resultArray.insert(resultArray.end(), arrayOnValue.begin(), arrayOnValue.end());
		}
		return this->rawWrite(id, multipleData->command, resultArray, multipleData->startBit, multipleData->count);
	}
	//Базовая структура не записывается
	return false;
}

bool ModbusCPP::ModbusMaster::write(const key_type id, const std::vector<SendData>& data)
{
	bool writeResult = true;
	for (auto& msg : data)
		writeResult &= this->write(id, msg);
	return writeResult;
}
bool ModbusCPP::ModbusMaster::writeRawRequest(const key_type id, const data_type request)
{
	std::int32_t writeResult{ 0 };
	if (this->setSlave(id) == true)
	{
		writeResult = modbus_send_raw_request(this->ctx, request.data(), static_cast<uint32_t>(request.size()));
		if (writeResult == -1)
		{
			writeResult = -1;
			lastError = errno;
		}
	}
	this->resetSlave();
	return writeResult != -1;
}
bool ModbusCPP::ModbusMaster::rawWrite(const key_type id, const BASE_COMMAND commandID, const data_type& data, const std::int32_t startBit, const std::int32_t count)
{
	std::int32_t writeResult = 0;
	if (this->setSlave(id))
	{
		//Single-запись через шаблонный метод
		switch (commandID)
		{
		case BASE_COMMAND::WRITE_MULTIPLE_COILS:
			writeResult = modbus_write_bits(this->ctx, startBit, count, data.data());
			break;
		case BASE_COMMAND::WRITE_MULTIPLE_REGISERS:
			writeResult = modbus_write_registers(this->ctx, startBit, count,
				static_cast<uint16_t*>(static_cast<void*>(const_cast<data_type::value_type*>(data.data()))));
			break;
		default:
			writeResult = -1;
			break;
		}
	}
	if (writeResult == -1)
		lastError = errno;
	this->resetSlave();
	return writeResult != -1;
}

std::pair<bool, ModbusCPP::ModbusMaster::data_type> ModbusCPP::ModbusMaster::rawRead(const key_type id, const BASE_COMMAND commandID, const std::int32_t startBit, const std::int32_t count)
{
	data_type::size_type dataSize = count;

	if (commandID > BASE_COMMAND::READ_DISCRETE_INPUT)
		dataSize *= 2; // Элементы sizeof(std::uint16_t), а sizeof(data_type::value_type) == sizeof(std::uint8_t) => data_type.size() = 2 * count

	data_type readedData(dataSize, 0);
	read_function_signature readFunction = nullptr;
	
	if (this->setSlave(id) == true)
	{
		switch (commandID)
		{
		case BASE_COMMAND::READ_COILS:
			readFunction = &ModbusMaster::readCoil;
			break;
		case BASE_COMMAND::READ_DISCRETE_INPUT:
			readFunction = &ModbusMaster::readDiscreteInput;
			break;
		case BASE_COMMAND::READ_HOLDING_REGISTERS:
			readFunction = &ModbusMaster::readHoldingsRegisters;
			break;
		case BASE_COMMAND::READ_INPUT_REGISTERS:
			readFunction = &ModbusMaster::readInputRegisters;
			break;
		default:
			readedData.clear();
			break;
		}
	}
	bool readResult{ readFunction != nullptr };
	if (readResult)
	{
		readResult = (this->*readFunction)(startBit, count, readedData.data()).first;
		if (readResult == false)
		{
			lastError = errno;
			readedData.clear();
		}
	}
	this->resetSlave();
	return{ readResult, std::move(readedData) };
}