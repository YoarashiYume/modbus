//#include "ModbusMaster.hpp"
#include "src/SettingsData/Config/ModbusConfig.hpp"
#include "src/Modbus/Specialized/Slave/ModbusSlave.hpp"
/*
template<typename T>
T* fromVector(std::vector<T> &vec)
{
	T* result = new T[vec.size()];
	std::copy(vec.begin(), vec.end(), result);
	return result;
}*/
void main()
{
	ModbusCPP::ModbusConfig conf{ "\\\\.\\COM3",ModbusCPP::BAUD_RATE::SPEED_115200,
	ModbusCPP::PARITY::EVEN, ModbusCPP::DATA_BITS::EIGHT, ModbusCPP::STOP_BITS::ONE };
	ModbusCPP::ModbusSlave mb{1};
	ModbusCPP::SlaveData sd;

	sd.holdingRegisters.count = 4;
	sd.holdingRegisters.startBits = 0;
	sd.holdingRegisters.data = { 0x1,0x12,0x5,0x5, 0x2, 0x12, 0x5, 0x5 };
	sd.holdingRegisters.order.emplace_back(ModbusCPP::BYTE_ORDER::NONE);
	sd.holdingRegisters.order.emplace_back(ModbusCPP::BYTE_ORDER::NONE);
	sd.holdingRegisters.order.emplace_back(ModbusCPP::BYTE_ORDER::NONE);
	sd.holdingRegisters.order.emplace_back(ModbusCPP::BYTE_ORDER::NONE);
	sd.holdingRegisters.countOfDataOnElement = {1,1,1,1};
	auto s = mb.addData(sd);
	return ;
	/*
	std::vector<uint8_t> vvv(8,0);
	double a = 5.14;
	memcpy(vvv.data(), &a, sizeof(double));
	auto mb = Modbus::ModbusMaster{Modbus::PORT::COM, 3,Modbus::BAUD_RATE::SPEED_115200,
	Modbus::PARITY::EVEN, Modbus::DATA_BITS::EIGHT, Modbus::STOP_BITS::ONE };
	mb.setDebug(true);
	auto res = mb.isCreated();
	auto conRes = mb.connect();
	//auto readRes = mb.rawRead(1, Modbus::BASE_COMMAND::READ_HOLDING_REGISTERS, 0, 8);
	std::vector<Modbus::DeviceInfo> di(4, {});
	di.at(0).dataType = Modbus::DEVICE_DATA_TYPE::INPUT_REGISTERS;
	di.at(0).countOfElements = 1;
	di.at(0).sizeInByte = sizeof(float);
	di.at(0).startBit = 0;
	di.at(0).byteOrder = Modbus::BYTE_ORDER::ABCD;

	di.at(1).dataType = Modbus::DEVICE_DATA_TYPE::INPUT_REGISTERS;
	di.at(1).countOfElements = 1;
	di.at(1).sizeInByte = sizeof(float);
	di.at(1).startBit = 2;
	di.at(1).byteOrder = Modbus::BYTE_ORDER::BADC;

	di.at(2).dataType = Modbus::DEVICE_DATA_TYPE::INPUT_REGISTERS;
	di.at(2).countOfElements = 1;
	di.at(2).sizeInByte = sizeof(float);
	di.at(2).startBit = 4;
	di.at(2).byteOrder = Modbus::BYTE_ORDER::DCBA;

	di.at(3).dataType = Modbus::DEVICE_DATA_TYPE::INPUT_REGISTERS;
	di.at(3).countOfElements = 1;
	di.at(3).sizeInByte = sizeof(float);
	di.at(3).startBit = 6;
	di.at(3).byteOrder = Modbus::BYTE_ORDER::CDAB;
	/*std::vector<Modbus::DeviceInfo > di;
	di.resize(4);

	di.at(0).startBit = 0;
	di.at(0).countOfElements = 2;
	di.at(0).sizeInByte = sizeof(uint16_t);
	di.at(0).dataType = Modbus::DEVICE_DATA_TYPE::HOLDING_REGISTERS;
	di.at(0).byteOrder = Modbus::BYTE_ORDER::NONE;

	di.at(1).startBit = 2;
	di.at(1).countOfElements = 1;
	di.at(1).sizeInByte = sizeof(uint32_t);
	di.at(1).dataType = Modbus::DEVICE_DATA_TYPE::HOLDING_REGISTERS;
	di.at(1).byteOrder = Modbus::BYTE_ORDER::BADC;

	di.at(2).startBit = 4;
	di.at(2).countOfElements = 4;
	di.at(2).sizeInByte = sizeof(uint8_t);
	di.at(2).dataType = Modbus::DEVICE_DATA_TYPE::DISCRETE_INPUT;
	di.at(2).byteOrder = Modbus::BYTE_ORDER::NONE;

	di.at(3).startBit = 8;
	di.at(3).countOfElements = 2;
	di.at(3).sizeInByte = sizeof(uint8_t);
	di.at(3).dataType = Modbus::DEVICE_DATA_TYPE::COILS;
	di.at(3).byteOrder = Modbus::BYTE_ORDER::NONE;

	mb.addDevice(2, di);
	//auto s = mb.rawRead(2, Modbus::BASE_COMMAND::READ_HOLDING_REGISTERS, 0, 2);
	uint32_t arraySize = 0;
	struct DeviceAnswerDLL
	{
		std::uint32_t posInDevice;
		std::uint32_t arraySize;
		std::uint8_t* data;
		~DeviceAnswerDLL()
		{
			if (data != nullptr)
				delete[] data;
		}
	};
	auto* master = &mb;
	DeviceAnswerDLL* result = nullptr;
	if (master != nullptr)
	{
		auto readResult = master->read(2);
		if (readResult.first == true)
		{
			arraySize = readResult.second.size();
			result = new DeviceAnswerDLL[arraySize]{};
			for (auto i = 0u; i < arraySize; ++i)
			{
				result[i].posInDevice = readResult.second.at(i).posInDevice;
				result[i].arraySize = readResult.second.at(i).data.size();
				if (result[i].arraySize != 0)
					result[i].data = fromVector<std::uint8_t>(readResult.second.at(i).data);
				else
					result[i].data = nullptr;
			}
		}
		auto i = 0;
	}
	delete[] result;
	auto write = mb.rawWrite<std::uint16_t>(2, Modbus::BASE_COMMAND::WRITE_SINGLE_REGISER, 134, 10);

	Modbus::MultipleSendData ss;
	ss.command = Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS;
	ss.sendData = std::vector<std::uint8_t>{0x01, 0x02,0x32, 0x00 };
	ss.order = Modbus::BYTE_ORDER::NONE;
	ss.startBit = 0;
	ss.count = 2;
	write = mb.rawWrite(2, Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS, ss.sendData.data(), 0, 2);
	write = mb.write(2, ss);
	mb.close();*/
}
/*
#include "ModbusSlave.hpp"
void main43()
{
	Modbus::SlaveData sd;

	sd.holdingRegisters.count = 4;
	sd.holdingRegisters.startBits = 0;
	sd.holdingRegisters.data = { 0x1,0x12,0x5,0x5, 0x2, 0x12, 0x5, 0x5 };
	sd.holdingRegisters.order.emplace_back(Modbus::BYTE_ORDER::NONE);
	sd.holdingRegisters.order.emplace_back(Modbus::BYTE_ORDER::NONE);
	sd.holdingRegisters.order.emplace_back(Modbus::BYTE_ORDER::NONE);
	sd.holdingRegisters.order.emplace_back(Modbus::BYTE_ORDER::NONE);
	std::uint16_t a;
	std::memcpy(&a, sd.holdingRegisters.data.data(), sizeof(std::uint16_t));
	auto mb = Modbus::ModbusSlave{ 2, Modbus::PORT::COM, 4,Modbus::BAUD_RATE::SPEED_115200,
		Modbus::PARITY::EVEN, Modbus::DATA_BITS::EIGHT, Modbus::STOP_BITS::ONE };
	auto s = mb.addData(sd);
	mb.setDebug(true);
	mb.setCallback([](Modbus::ModbusSlave* mb) -> Modbus::MODBUS_ERROR
	{
		if (mb->getRequestFunction() == static_cast<std::uint8_t>(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS))
		{
			auto data = mb->getRequestData(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS);
			auto cc0 = mb->getRequestCountOfElements(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS);
			auto c10 = mb->getRequestCountOfData(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS);
			auto s0 = cc0 < c10 * 8;
			mb->setRequestStartBit(0);

			data.resize(4);
			data.at(2) = data.at(0);
			data.at(3) = data.at(1);
			mb->setRequestCountOfElements(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS,2);
			mb->setRequestData(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS, data);

			auto cc = mb->getRequestCountOfElements(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS);
			auto c1 = mb->getRequestCountOfData(Modbus::BASE_COMMAND::WRITE_MULTIPLE_REGISERS);
			auto s = cc < c1 * 8;
			auto i = 0;
		}
		return Modbus::MODBUS_ERROR::OK;

	});
	auto res2 = mb.connect();
	while (mb.listen() != Modbus::LISTEN_RESULT::FAILED)
	{
		if (mb.getMessageSize() == 0)
			continue;
		auto s = mb.getMessage();
		
		mb.sendAnswer();
	}
}*/