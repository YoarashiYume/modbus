#include "ModbusData.hpp"


/**
\brief Метод для выделение памяти под массивы структуры modbus_mapping_t
\param count Количество элементов
\param where Указатель на массив
\return Если память была выделена true, иначе - false
*/
template<typename T>
static 
bool setMemory(const std::uint32_t& count, T*& where)
{
	if (count == 0)
		where = nullptr;
	else
	{
		where = (T*)malloc(count * sizeof(T));
		memset(where, 0, count * sizeof(T));
		if (where == nullptr)
			return false;
	}
	return true;
}
bool ModbusCPP::ModbusData::createMemory()
{

	if (setMemory<std::uint8_t>(nb_bits, tab_bits) == false)
		return false;
	if (setMemory<std::uint8_t>(nb_input_bits, tab_input_bits) == false)
	{
		free(tab_bits);
		return false;
	}

	if (setMemory<std::uint16_t>(nb_registers, tab_registers) == false)
	{
		free(tab_bits);
		free(tab_input_bits);
		return false;
	}

	if (setMemory<std::uint16_t>(nb_input_registers, tab_input_registers) == false)
	{
		free(tab_bits);
		free(tab_input_bits);
		free(tab_registers);
		return false;
	}
	return true;
}
void ModbusCPP::ModbusData::clearMemory()
{
	if (tab_bits != nullptr)
		free(tab_bits);
	if (tab_bits != nullptr)
		free(tab_input_bits);
	if (tab_bits != nullptr)
		free(tab_registers);
	if (tab_bits != nullptr)
		free(tab_input_registers);
	tab_bits = tab_input_bits = nullptr;
	tab_registers = tab_input_registers = nullptr;
}
ModbusCPP::ModbusData::ModbusData(const ModbusData& other)
{
	this->start_bits = other.start_bits;
	this->start_input_bits = other.start_input_bits;
	this->start_registers = other.start_registers;
	this->start_input_registers = other.start_input_registers;

	this->nb_bits = other.nb_bits;
	this->nb_input_bits = other.nb_input_bits;
	this->nb_registers = other.nb_registers;
	this->nb_input_registers = other.nb_input_registers;
	this->isInit = other.isInit;

	if (this->isInit)
	{
		this->createMemory();

		std::copy(static_cast<std::uint8_t *>(other.tab_bits), other.tab_bits + other.nb_bits,
			this->tab_bits);
		std::copy(static_cast<std::uint8_t *>(other.tab_input_bits), other.tab_input_bits + other.nb_input_bits,
			this->tab_input_bits);
		std::copy(static_cast<std::uint16_t *>(other.tab_input_registers), other.tab_input_registers + other.nb_input_registers,
			this->tab_input_registers);
		std::copy(static_cast<std::uint16_t *>(other.tab_registers), other.tab_registers + other.nb_registers,
			this->tab_registers);

		this->inputRegistersOrder = other.inputRegistersOrder;
		this->holdingsRegistersOrder = other.holdingsRegistersOrder;
	}
}

ModbusCPP::ModbusData::ModbusData(ModbusData && other)
{

	this->start_bits = other.start_bits;
	this->start_input_bits = other.start_input_bits;
	this->start_registers = other.start_registers;
	this->start_input_registers = other.start_input_registers;

	this->nb_bits = other.nb_bits;
	this->nb_input_bits = other.nb_input_bits;
	this->nb_registers = other.nb_registers;
	this->nb_input_registers = other.nb_input_registers;
	this->isInit = other.isInit;

	if (this->isInit)
	{
		this->tab_bits = std::move(other.tab_bits);
		this->tab_input_bits = std::move(other.tab_input_bits);
		this->tab_registers = std::move(other.tab_registers);
		this->tab_input_registers = std::move(other.tab_input_registers);
		other.start_bits = other.start_input_bits = other.start_registers = other.start_input_registers = 0;
		other.nb_bits = other.nb_input_bits = other.nb_registers = other.nb_input_registers = 0;
		other.tab_bits = other.tab_input_bits = nullptr;
		other.tab_registers = other.tab_input_registers = nullptr;

		this->inputRegistersOrder = std::move(other.inputRegistersOrder);
		this->holdingsRegistersOrder = std::move(other.holdingsRegistersOrder);
	}
}

ModbusCPP::ModbusData::ModbusData(const value_type start_bits, const value_type nb_bits,
	const value_type start_input_bits, const value_type nb_input_bits,
	const value_type start_registers, const value_type nb_registers,
	const value_type start_input_registers, const value_type nb_input_registers)
{
	this->start_bits = static_cast<std::int32_t>(start_bits);
	this->start_input_bits = static_cast<std::int32_t>(start_input_bits);
	this->start_registers = static_cast<std::int32_t>(start_registers);
	this->start_input_registers = static_cast<std::int32_t>(start_input_registers);

	this->nb_bits = static_cast<std::int32_t>(nb_bits);
	this->nb_input_bits = static_cast<std::int32_t>(nb_input_bits);
	this->nb_registers = static_cast<std::int32_t>(nb_registers);
	this->nb_input_registers = static_cast<std::int32_t>(nb_input_registers);

	

	inputRegistersOrder.resize(nb_input_registers, BYTE_ORDER::NONE);
	holdingsRegistersOrder.resize(nb_input_registers, BYTE_ORDER::NONE);

	isInit = createMemory();
}
ModbusCPP::ModbusData::~ModbusData()
{
	clearMemory();
}
bool ModbusCPP::ModbusData::isCorrecInit() const
{
	return isInit;
}