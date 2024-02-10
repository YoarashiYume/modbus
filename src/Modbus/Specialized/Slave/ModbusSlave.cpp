#include "ModbusSlave.hpp"

void ModbusCPP::ModbusSlave::setId()
{
	if (ctx != nullptr)
		if (modbus_set_slave(ctx, currentID) == -1)
		{
			lastError = errno;
			this->close();
		}
		else
			headerSize = modbus_get_header_length(ctx);
}
modbus_mapping_t* ModbusCPP::ModbusSlave::handleRequest(MODBUS_ERROR& error)
{
	error = MODBUS_ERROR::OK;
	modbus_mapping_t* resultData{ nullptr };
	if (callback)
		error = callback(this);

	if (error == MODBUS_ERROR::OK)
	{
		auto type{ DEVICE_DATA_TYPE::HOLDING_REGISTERS };
		switch (getRequestFunction())
		{
		case BASE_COMMAND::WRITE_SINGLE_COIL:
		case BASE_COMMAND::WRITE_MULTIPLE_COILS:
		case BASE_COMMAND::READ_COILS:
			type = DEVICE_DATA_TYPE::COILS;
		case BASE_COMMAND::READ_DISCRETE_INPUT:
			if (type > DEVICE_DATA_TYPE::DISCRETE_INPUT)
				type = DEVICE_DATA_TYPE::DISCRETE_INPUT;
		case BASE_COMMAND::READ_INPUT_REGISTERS:
			if (type > DEVICE_DATA_TYPE::INPUT_REGISTERS)
				type = DEVICE_DATA_TYPE::INPUT_REGISTERS;
		case BASE_COMMAND::WRITE_SINGLE_REGISER:
		case BASE_COMMAND::WRITE_MULTIPLE_REGISERS:
		case BASE_COMMAND::READ_HOLDING_REGISTERS:
		{
			resultData = this->findData(error, type, 
				getRequestStartBit(), 
				getRequestCountOfElements());
			break;
		}
		default:
			error = MODBUS_ERROR::ILLEGAL_FUNCIONT;
			break;
		}
	}
	return resultData;
}
modbus_mapping_t* ModbusCPP::ModbusSlave::findData(MODBUS_ERROR& error, const DEVICE_DATA_TYPE what, const std::uint16_t startBit, const std::uint16_t count)
{
	/*
	Не гарантируется, что найдено count элементов, начинающихся со startBit.
	Найденные данные попадают в диапазон от startBit до startBit+count.
	Корректность результата зависит от корректности заполнения структуры устройства.
	*/
	ModbusSlave::device_info_type::value_type* value{ nullptr };
	std::uint32_t startBitOffset{ 0 },
		countOfBitsOffset{ 0 },
		offset{ 0 };
	//Отступ до данных, исходя из структуры modbus_mapping_t
	switch (what)
	{
	case DEVICE_DATA_TYPE::COILS:
		offset = 1;
		break;
	case DEVICE_DATA_TYPE::DISCRETE_INPUT:
		offset = 3;
		break;
	case DEVICE_DATA_TYPE::INPUT_REGISTERS:
		offset = 5;
		break;
	case DEVICE_DATA_TYPE::HOLDING_REGISTERS:
		offset = 7;
		break;
	default:
		error = MODBUS_ERROR::ILLEGAL_DATA_VALUE;
		return nullptr;
	}

	startBitOffset = offset * sizeof(decltype(modbus_mapping_t::nb_bits));
	countOfBitsOffset = (offset - 1) * sizeof(decltype(modbus_mapping_t::nb_bits));

	for (auto& el : data)
	{
		void* ptr{ &(modbus_mapping_t)el };
		uint16_t startBitInDevice{ static_cast<uint16_t>(*(uint32_t*)((uint8_t*)ptr + startBitOffset)) };
		uint16_t countInDevice{ static_cast<uint16_t>(*(uint32_t*)((uint8_t*)ptr + countOfBitsOffset)) };

		/*
		Имеем startBitInDevice = 0, countInDevice = 2
		Запрашиваем startBit = 0, count = 1
		left = 0 > 0 => 0, right = 2 < 1 => 1, left < right => есть пересечение данных

		Имеем startBitInDevice = 1, countInDevice = 2
		Запрашиваем startBit = 3, count = 5
		left = 3 > 1 => 3, right = 8 < 3 => 3, left == right => Пересечения нет (данные в 1м и 2м бите)

		Имеем startBitInDevice = 1, countInDevice = 2
		Запрашиваем startBit = 2, count = 5
		left = 2 > 1 => 2, right = 7 < 3 => 3, left < right => Пересечения нет (2й бит)
		*/

		auto left = startBit > startBitInDevice ? startBit : startBitInDevice;
		auto right = (startBit + count) < (startBitInDevice + countInDevice) ? startBit + count : startBitInDevice + countInDevice;

		if (left < right)
			return &el;
	}
	error = MODBUS_ERROR::ILLEGAL_DATA_ADDRESS;
	return nullptr;
}

void ModbusCPP::ModbusSlave::coilCopy(const SlaveData::Data& src, uint8_t*& dest)
{
	if (src.count != 0)
		std::copy(src.data.data(), src.data.data() + src.count, dest);
}

bool ModbusCPP::ModbusSlave::coilRegisters(const SlaveData::Data& src, uint16_t*& dest, std::vector<BYTE_ORDER>& destEncode)
{
	bool isCorrect{ true };
	if (src.count != 0)
	{
		std::vector<std::uint8_t> buf;
		std::uint32_t currentPos{ 0 };
		for (auto i = 0u; i < src.count && isCorrect == true; ++i)
		{
			buf.assign(src.data.begin() + currentPos,
				src.data.begin() + currentPos + src.countOfDataOnElement.at(i));
			if (decode(buf, src.order.at(i)))
			{
				auto ptr = static_cast<uint16_t*>(static_cast<void*>(const_cast<std::uint8_t*>(buf.data())));
				std::copy(ptr, ptr + buf.size() / 2, data.back().tab_registers + currentPos / 2);
				destEncode.emplace_back(src.order.at(i));
				currentPos += static_cast<std::uint32_t>(buf.size());
			}
			else
				isCorrect = false;
		}
	}
	return isCorrect;
}

ModbusCPP::ModbusSlave::ModbusSlave(const it_type id)
	: currentID(id)
{
	setId();
}

bool ModbusCPP::ModbusSlave::addData(const initial_device_type& newData)
{
	data.push_back(
		ModbusData{
			newData.coil.startBits, newData.coil.count,
			newData.inputCoil.startBits, newData.inputCoil.count,
			newData.holdingRegisters.startBits, static_cast<std::uint32_t>(newData.holdingRegisters.data.size() / 2), //Данные хранятся в std::uint_8t, а размер регистра std::uint16_t => /2
			newData.inputRegisters.startBits,static_cast<std::uint32_t>(newData.inputRegisters.data.size() / 2)
		}
	);

	coilCopy(newData.coil, data.back().tab_bits);
	coilCopy(newData.inputCoil, data.back().tab_input_bits);

	bool isCorrect{coilRegisters(newData.holdingRegisters, data.back().tab_registers, data.back().holdingsRegistersOrder)};
	isCorrect &= coilRegisters(newData.inputRegisters, data.back().tab_input_registers, data.back().inputRegistersOrder);
	if (isCorrect == false)
		data.pop_back();
	return isCorrect;
	return true;
}

bool ModbusCPP::ModbusSlave::addData(const std::vector<initial_device_type>& newData)
{
	bool isCorrect{ true };
	for (auto& el : newData)
		isCorrect &= this->addData(el);
	return isCorrect;
}

const ModbusCPP::ModbusSlave::device_info_type& ModbusCPP::ModbusSlave::getDeviceData() const&
{
	return data;
}

ModbusCPP::ModbusSlave::device_info_type ModbusCPP::ModbusSlave::removeData()
{
	ModbusSlave::device_info_type temp;
	std::swap(temp, data);
	return temp;
}
ModbusCPP::ModbusSlave::LISTEN_RESULT ModbusCPP::ModbusSlave::listen()
{
	auto result = modbus_receive(ctx, query.data());
	this->messageSize = result;
	if (result == -1)
	{
		lastError = errno;
		this->messageSize = 0;
	}
	return static_cast<LISTEN_RESULT>(result > 1 ? 1 : result);
}
const ModbusCPP::ModbusSlave::message_type& ModbusCPP::ModbusSlave::getMessage()&
{
	return query;
}

std::uint32_t ModbusCPP::ModbusSlave::getHeaderSize() const
{
	return headerSize;
}

void ModbusCPP::ModbusSlave::setCallback(callback_type cb)
{
	this->callback = cb;
}

std::uint16_t ModbusCPP::ModbusSlave::getRequestStartBit() const
{
	constexpr auto offset = static_cast<std::uint32_t>(MODBUS_REQUEST_OFFSETS::ADDRESS_OFFSET);
	constexpr auto size = static_cast<std::uint32_t>(MODBUS_REQUEST_ELEMENT_BYTE_COUNT::ADDRESS_SIZE) - 1;

	return (query.at(headerSize + offset) << 8)
		+ query.at(headerSize + offset + size);
}
ModbusCPP::BASE_COMMAND ModbusCPP::ModbusSlave::getRequestFunction() const
{
	return static_cast<BASE_COMMAND>(query.at(headerSize));
}
const std::vector<std::uint8_t> ModbusCPP::ModbusSlave::getRequestData() const
{
	auto commandID{ getRequestFunction() };
	std::uint16_t count = this->getRequestCountOfData();
	std::vector<std::uint8_t> requestData;
	switch (commandID)
	{
	case BASE_COMMAND::WRITE_SINGLE_COIL:
	case BASE_COMMAND::WRITE_SINGLE_REGISER:
		requestData.resize(sizeof(count) / sizeof(decltype(requestData)::value_type));
		//Данные лежат в 4м и 5м отступе
		count = (query.at(headerSize + static_cast<std::uint32_t>(MODBUS_REQUEST_OFFSETS::COUNT_OF_ELEMENT_OFFSET)) << 8)
			+ query.at(headerSize + static_cast<std::uint32_t>(MODBUS_REQUEST_OFFSETS::COUNT_OF_ELEMENT_OFFSET)
				+ static_cast<std::uint32_t>(MODBUS_REQUEST_ELEMENT_BYTE_COUNT::COUNT_OF_ELEMENT_SIZE) - 1);
		std::memcpy(requestData.data(), &count, requestData.size());
		break;
	case BASE_COMMAND::WRITE_MULTIPLE_COILS:
	case BASE_COMMAND::WRITE_MULTIPLE_REGISERS:
		requestData.resize(count);
		std::memcpy(requestData.data(), &query.at(headerSize + static_cast<std::uint32_t>(MODBUS_REQUEST_OFFSETS::DATA_ON_MULTIPLE_WRITE_OFFSET)),
			count * sizeof(std::uint8_t));
		break;
	default:
		break;
	}
	return requestData;
}
std::uint32_t ModbusCPP::ModbusSlave::getMessageSize() const
{
	return this->messageSize;
}
std::uint16_t ModbusCPP::ModbusSlave::getRequestCountOfElements() const
{
	constexpr auto offset = static_cast<std::uint32_t>(MODBUS_REQUEST_OFFSETS::COUNT_OF_ELEMENT_OFFSET);
	constexpr auto size = static_cast<std::uint32_t>(MODBUS_REQUEST_ELEMENT_BYTE_COUNT::COUNT_OF_ELEMENT_SIZE) - 1;
	auto commandID{ getRequestFunction() };
	switch (commandID)
	{
	case BASE_COMMAND::WRITE_SINGLE_COIL:
	case BASE_COMMAND::WRITE_SINGLE_REGISER:
		return 1;//Т.к. при подобных кодах запрос не содержит инфомации
	default:
		return (query.at(headerSize + offset) << 8) + query.at(headerSize + offset + size);
	}
}

std::uint8_t ModbusCPP::ModbusSlave::getRequestCountOfData() const
{
	std::uint8_t count{ 0 };
	auto commandID{ getRequestFunction() };
	switch (commandID)
	{
	case BASE_COMMAND::WRITE_MULTIPLE_COILS:
	case BASE_COMMAND::WRITE_MULTIPLE_REGISERS:
		count = query.at(headerSize +
			static_cast<std::uint32_t>(MODBUS_REQUEST_OFFSETS::COUNT_OF_BYTE_OFFSET));
		break;
	case BASE_COMMAND::WRITE_SINGLE_COIL:
	case BASE_COMMAND::WRITE_SINGLE_REGISER:
		count = 2;//Т.к. при подобных кодах запрос не содержит инфомации, объем данных sizeof(std::uint16_t)
		break;
	default:
		break; // Т.к.при оставшихся запросах данные не передаются
	}
	return count;
}
bool ModbusCPP::ModbusSlave::sendAnswer()
{
	MODBUS_ERROR error;
	auto answer = handleRequest(error);
	std::int32_t operationResult{ 0 };
	if (answer != nullptr)
		operationResult = modbus_reply(ctx, query.data(), messageSize, answer);
	else if (data.empty())
		operationResult = modbus_reply_exception(ctx, query.data(), static_cast<std::uint32_t>(MODBUS_ERROR::ILLEGAL_FUNCIONT));
	else
		operationResult = modbus_reply_exception(ctx, query.data(), static_cast<std::uint32_t>(error));
	if (operationResult == -1)
		lastError = errno;
	return operationResult != -1;
}