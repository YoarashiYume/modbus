#pragma once
#include <unordered_map>

#include "../../Base/ModbusBase.hpp"
#include "../../../TransferedData/TransferedStruct.hpp"
namespace ModbusCPP
{
	/**
	\brief Класс, представляющий ведущее устройство Modbus
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
		storage_type connectedDevice;///< Список подключенных устройств
		/**
		\brief Метод для считывания coil`ов из структуры устройства
		\param[in] deviceData Структура для считывания данных
		\param[in] readFunction Функция для считывания coil`ов
		\param[in, out] isCorrect Переменная для записи успешности считывания
		\return Результат считывания
		*/
		DeviceAnswer readCoil(const DeviceInfo deviceData, read_function_signature readFunction, bool& isCorrect);
		/**
		\brief Метод для считывания register`ов из структуры устройства
		\param[in] deviceData Структура для считывания данных
		\param[in] offset Отступ от начального расположения в структуре структуры
		\param[in] readFunction Функция для считывания register`ов
		\param[in, out] isCorrect Переменная для записи успешности считывания
		\return Результат считывания
		*/
		DeviceAnswer readRegisters(const DeviceInfo deviceData,const std::uint32_t offset, read_function_signature readFunction, bool& isCorrect);
		/**
		\brief Метод для считывания регистров флагов из ведомого устройства
		\param startBit Начальный бит, с которого будет происходить чтение
		\param count Количество считываемых данных
		\param[out] data Опциональный параметр, при указании которого считывание данных будет происходить в данный, заранее созданный, массив
		\return Пару <bool, массив байт>. Первое значение хранит успешность выполненной операции. Второе значение хранит считанный массив данных (если параметр data == nullptr)
		*/
		std::pair<bool, data_type> readCoil(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
		/**
		\brief Метод для считывания дискретных входов из ведомого устройства
		\param startBit Начальный бит, с которого будет происходить чтение
		\param count Количество считываемых данных
		\param[out] data Опциональный параметр, при указании которого считывание данных будет происходить в данный, заранее созданный, массив
		\return Пару <bool, массив байт>. Первое значение хранит успешность выполненной операции. Второе значение хранит считанный массив данных (если параметр data == nullptr)
		*/
		std::pair<bool, data_type> readDiscreteInput(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
		/**
		\brief Метод для считывания регистров ввода из ведомого устройства
		\param startBit Начальный бит, с которого будет происходить чтение
		\param count Количество считываемых данных
		\param[out] data Опциональный параметр, при указании которого считывание данных будет происходить в данный, заранее созданный, массив
		\return Пару <bool, массив байт>. Первое значение хранит успешность выполненной операции. Второе значение хранит считанный массив данных (если параметр data == nullptr)
		*/
		std::pair<bool, data_type> readInputRegisters(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
		/**
		\brief Метод для считывания регистров хранения из ведомого устройства
		\param startBit Начальный бит, с которого будет происходить чтение
		\param count Количество считываемых данных
		\param[out] data Опциональный параметр, при указании которого считывание данных будет происходить в данный, заранее созданный, массив
		\return Пару <bool, массив байт>. Первое значение хранит успешность выполненной операции. Второе значение хранит считанный массив данных (если параметр data == nullptr)
		*/
		std::pair<bool, data_type> readHoldingsRegisters(const std::int32_t startBit, const std::int32_t count, data_type::value_type* data = nullptr);
	public:
		/**
		\brief Метод добавляющий информацию о ведомом устройстве(его структуру)
		\param id ID ведомого устройства
		\param deviceInfo Информация о ведомом устройстве
		*/
		void addDevice(const key_type id, const value_type& deviceInfo);
		/**
		\brief Метод добавляющий информацию о ведомом устройстве(его структуру)
		\param id ID ведомого устройства
		\param deviceInfo Информация о ведомом устройстве
		*/
		void addDevice(const key_type id, const std::vector<value_type>& deviceInfo);
		/**
		\brief Метод удаляющий информацию о ведомом устройстве
		\param id ID ведомого устройства
		\return Удаляемые данные
		*/
		std::pair<bool, std::vector<value_type>> removeDevice(const key_type id);
		/**
		\brief Метод позволяющий просмотреть структуру ведомого устройства
		\param id ID ведомого устройства
		\param deviceInfo Информация о ведомом устройстве
		*/
		std::vector<value_type> getDevice(const key_type id) const;
		/**
		\brief Метод, позволяющий считать данные из устройства
		Данный метод использует структуру ведомого устройства
		\param id ID устройства, из которого будет происходить считывание
		\return Пару <bool, Ответы устройства>. Первое значение хранит успешность выполненной операции (Если заданная структура отличается от действительной - вернет false).  Второе значение хранит список ответов устройства, если они были считаны (если ответ хранит пустое значение - отличие в структуре)
		*/
		std::pair<bool, std::vector<DeviceAnswer>> read(const key_type id);
		/**
		\brief Метод, позволяющий записать данные в устройство
		\param id ID устройства, в которое будет происходить запись
		\param data Передаваемые данные
		\return Возвращает true если запись была успешна, иначе false
		*/
		bool write(const key_type id, const SendData& data);
		/**
		\brief Метод, позволяющий записать данные в устройство
		\param id ID устройства, в которое будет происходить запись
		\param data Список передаваемых данных
		\return Возвращает true если запись была успешна, иначе false
		*/
		bool write(const key_type id, const std::vector<SendData>& data);
		/**
		\brief Метод позволяющий отправить raw-запрос к ведомому устройству
		\param id ID устройства, которому будет отправлен запрос
		\param request raw-запрос
		\return Возвратит true, если запрос был успешен. Иначе false
		*/
		bool writeRawRequest(const key_type id, const data_type request);
		/**
		\brief Метод, позволяющий записать данные в устройство
		\param id ID устройства, в которое будет происходить запись
		\param commandID Код Modbus-функции
		\param data Записываемые данные
		\param startBit Начальный адрес, для применения функции
		\param count Количество элементов, затрагиваемых Modbus-функцией
		\return При успешности операции возвращает true, иначе - false
		*/
		bool rawWrite(const key_type id, const BASE_COMMAND commandID, const data_type& data,
			const std::int32_t startBit, const std::int32_t count);
		template<typename T,
			typename = typename std::enable_if<
			std::is_unsigned<T>::value &&
			sizeof(T) <= sizeof(std::uint16_t)
		>::type>
		/**
		\brief Метод, позволяющий записать данные в устройство
		\param id ID устройства, в которое будет происходить запись
		\param commandID Код Modbus-функции
		\param data Записываемые данные
		\param startBit Начальный адрес, для применения функции
		\return При успешности операции возвращает true, иначе - false
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
		\brief Метод, позволяющий считать данные из устройства
		В данном методе структура устройства не используется
		\param id ID устройства, из которого будет происходить считывание
		\param commandID Код Modbus-функции
		\param startBit Начальный адрес, для применения функции
		\param count Количество элементов, затрагиваемых Modbus-функцией
		\return Пару <bool, массив байт>.  Первое значение хранит успешность выполненной операции. Второе значение хранит считанный массив данных.
		*/
		std::pair<bool, data_type> rawRead(const key_type id, const BASE_COMMAND commandID,
			const std::int32_t startBit, const std::int32_t count);
	};
}