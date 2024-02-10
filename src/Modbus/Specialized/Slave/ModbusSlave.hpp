#pragma once
#include <array>
#include "../../Base/ModbusBase.hpp"
#include "../../../TransferedData/TransferedStruct.hpp"
#include "../../../TransferedData/ModbusData.hpp"
namespace ModbusCPP
{
	/**
	\brief Ошибки Modbus.
	*/
	enum class MODBUS_ERROR : std::int32_t
	{
		OK,																	///< Не ошибка.
		ILLEGAL_FUNCIONT = MODBUS_EXCEPTION_ILLEGAL_FUNCTION,				///< Принятый код функции не может быть обработан.
		ILLEGAL_DATA_ADDRESS = MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,		///< Адрес данных, указанный в запросе, недоступен.
		ILLEGAL_DATA_VALUE = MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,			///< Адрес данных, указанный в запросе, недоступен.
		SLAVE_OR_SERVER_FAILURE = MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE,	///< Невосстанавливаемая ошибка имела место, пока ведомое устройство пыталось выполнить затребованное действие.
		ACKNOWLEDGE = MODBUS_EXCEPTION_ACKNOWLEDGE,							///< Ведомое устройство приняло запрос и обрабатывает его, но это требует много времени (избежание Timeout).
		SLAVE_OR_SERVER_BUSY = MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,		///< Ведомое устройство занято обработкой команды.
		NEGATIVE_ACKNOWLEDGE = MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,		///< Ведомое устройство не может выполнить программную функцию, заданную в запросе.
		MEMORY_PARITY = MODBUS_EXCEPTION_MEMORY_PARITY,						///< Ведомое устройство при чтении расширенной памяти обнаружило ошибку контроля четности.
		NOT_DEFINED = MODBUS_EXCEPTION_NOT_DEFINED,							///< Неопределенная ошибка.
		GATEWAY_PATH = MODBUS_EXCEPTION_GATEWAY_PATH,						///< Ошибка путь шлюза.
		GATEWAY_TARGET = MODBUS_EXCEPTION_GATEWAY_TARGET,					///< Ошибка цели шлюза.
		MAX = MODBUS_EXCEPTION_MAX											///< Максимальное значение ошибки.
	};

	/**
	\brief Перечисление отступов в струкрутуре запроса Modbus
	Отступы не учитывают хэдер запроса и блок обнаружения ошибок.
	Блок обнаружения ошибок_1 = длина_сообщения - 1.
	Блок обнаружения ошибок_2 = длина_сообщения - 2.
	*/
	enum class MODBUS_REQUEST_OFFSETS : unsigned int
	{
		FUNCTION_OFFSET = 0,				///< Отступ до кода функции.
		ADDRESS_OFFSET = 1,					///< Отступ до адреса начального элемента.
		COUNT_OF_ELEMENT_OFFSET = 3,		///< Отступ до количества элементов.
		DATA_ON_SINGLE_WRITE_OFFSET = 3,	///< Отступ до данных записываемого элемента, при кодах функциях 0x05 и 0x06.
		DATA_ON_MULTIPLE_WRITE_OFFSET = 6,	///< Отступ до данных записываемых элементов, при кодах функциях 0x15 и 0x16.
		COUNT_OF_BYTE_OFFSET = 5			///< Отсутп до количества байт данных, при кодах функциях 0x15 и 0x16.
	};
	/**
	\brief Перечесление количества байт, занимающих соответствующими блоками.
	Количество байт данных, при множественной записи (0x15 и 0x16) указывается в запросе, с отступом MODBUS_REQUEST_OFFSETS::COUNT_OF_BYTE_OFFSET
	*/
	enum class MODBUS_REQUEST_ELEMENT_BYTE_COUNT : unsigned int
	{
		ADDRESS_SIZE = 2,					///< Количество байт для записи начального адреса элементов.
		COUNT_OF_ELEMENT_SIZE = 2,			///< Количество байт для записи количества запрашиваемых/изменяемых элементов.
		DATA_ON_SINGLE_WRITE_SIZE = 2,		///< Количество байт для данных записываемого элемента, при кодах функциях 0x05 и 0x06.
		COUNT_OF_BYTE_SIZE = 1				///< Количество байт для записи объема передаваемых даннных, при кодах функциях 0x15 и 0x16.
	};

	/**
	\brief Структура используемая для инициализации структуры устройства
	*/
	class ModbusSlave : public ModbusBase
	{
	public:
		using it_type = std::int32_t;
		using device_info_type = std::vector<ModbusData>;
		using initial_device_type = SlaveData;
		using message_type = std::array<uint8_t, MODBUS_RTU_MAX_ADU_LENGTH>;
		// Если после обработки пользовательской функцией запрос не должен быть выполнен, то функция должно вернуть MODBUS_ERROR != MODBUS_ERROR::OK, в ином случае MODBUS_ERROR::OK
		using callback_type = MODBUS_ERROR(*)(ModbusSlave*);
		/**
		\brief Перечисление результатов функции ожидания запроса
		*/
		enum class LISTEN_RESULT : std::int8_t
		{
			FAILED = -1,///< Запрос не удался
			IGNORED_DATA,///< Запрос не на это устройство, необходимо проигнорировать
			SUCCSES///< Запрос получен
		};
	private:
		it_type currentID; ///< ID текущего устройства
		device_info_type data;///< Структура подключенного текущего устройства
		message_type query;///< Пришедший запрос с ведомого устройства Modbus
		std::uint32_t headerSize{},///< Размер хэдера сообщения
			messageSize{};///< Размер пришедшего сообщения
		callback_type callback;///< Пользовательская функция, вызываемая при ответе на пришедший запрос

		/**
		\brief Копирующий coil`ы из входных данных
		\param[in] src Данные для копирования
		\param[in, out] dest Место сохранения данных
		*/
		void coilCopy(const SlaveData::Data& src, uint8_t*& dest);
		/**
		\brief Копирующий register`ов из входных данных
		\param[in] src Данные для копирования
		\param[in, out] dest Место сохранения данных
		\param[in, out] destEncode Место сохранения порядка данных
		*/
		bool coilRegisters(const SlaveData::Data& src, uint16_t*& dest, std::vector<BYTE_ORDER>& destEncode);
		/**
		\brief Метод устанавливающий ID устройства и определяющий размер хэдера
		*/
		void setId();
		/**
		\brief Метод обрабатывающий пришедщий запрос через пользовательскую функцию
		\param[out] error Текущее состояние запроса
		\return Указатель на запрашиваемые данные в запросе
		*/
		modbus_mapping_t* handleRequest(MODBUS_ERROR& error);
		/**
		\brief Метод для поиска данных на запросы с кодом функции 0x01-0x04
		\param[out] error Текущее состояние запроса
		\param what Запрашиваемый тип данных в запросе
		\param startBit Начальный бит из запроса
		\param count Количество элеентов из запроса
		\return Указатель на запрашиваемые данные в запросе
		*/
		modbus_mapping_t* findData(MODBUS_ERROR& error, const DEVICE_DATA_TYPE what, const std::uint16_t startBit, const std::uint16_t count);
	public:
		ModbusSlave(const it_type id);
		/**
		\brief Метод используется для инициализации Modbus-устройства(добавление данных о его структуре)
		\param newData Добавляемая информация в структуру
		\return true, если данные были добавлены. Иначе false.
		*/
		bool addData(const initial_device_type& newData);
		/**
		\brief Метод используется для инициализации Modbus-устройства(добавление данных о его структуре)
		\param newData Добавляемая информация в структуру
		\return true, если все данные были добавлены. Иначе false.
		*/
		bool addData(const std::vector<initial_device_type>& newData);
		/**
		\brief Метод возвращающий заданную структуру устройства
		\return Структура устройства
		*/
		const device_info_type& getDeviceData() const&; //l-value only
		/**
		\brief Метод очищает структуру Modbus-устройства
		\return Очищаемая структура Modbus-устройства
		*/
		device_info_type removeData();
		/**
		\brief Метод ожидающий Modbus-запрос к устройству
		\return Результат ожидания сообщения (получено/ошибка/игнорировать запрос)
		*/
		LISTEN_RESULT listen();
		/**
		\brief Метод запускающий пользовательскую функцию-обработки и отправляющий ответ на полученный запрос
		\return true, если ответ был успешно отправлен, иначе false
		*/
		bool sendAnswer();
		/**
		\brief Метод устанавливающий пользовательскую функцию-обработки запроса
		\param cb Пользовательская функция-обработчик
		*/
		void setCallback(callback_type cb);
		/**
		\brief Метод возвращающий пришедшее Modbus-запрос
		\return Пришедший Modbus-запрос
		*/
		const message_type& getMessage()&; //l-value only
		/**
		\brief Метод возвращающий размер хэдера сообщения
		\return Размер хэдера сообщения
		*/
		std::uint32_t getHeaderSize() const;
		/**
		\brief Метод возвращающий начальный бит из запроса
		\return Начальный бит из запроса
		*/
		std::uint16_t getRequestStartBit() const;
		/**
		\brief Метод возвращающий код функции из запроса
		\return Код функции из запроса
		*/
		BASE_COMMAND getRequestFunction() const;
		/**
		\brief Метод возвращающий количество элементов, затрагиваемых запросом
		\return Количество элементов, затрагиваемых запросом
		*/
		std::uint16_t getRequestCountOfElements() const;
		/**
		\brief Метод возвращающий размер переданных данных в запросе
		\return Размер переданных данных в запросе
		*/
		std::uint8_t getRequestCountOfData() const;
		/**
		\brief Метод возвращающий переданные в запросе
		\return Переданные данные в запросе
		*/
		const std::vector<std::uint8_t> getRequestData() const;
		/**
		\brief Метод возвращающий размер пришедшего запроса
		\return Размер пришедшего запроса
		*/
		std::uint32_t getMessageSize() const;
	};
}