#pragma once
#include "../../SettingsData/Config/ModbusConfig.hpp"
#include "../../TransferedData/TransferedDataEnums.hpp"
#include <modbus.h>
#include <vector>
namespace ModbusCPP
{
	/**
	\brief Базовый класс для общения по протоколу Modbus
	*/
	class ModbusBase
	{
	public:
		/**
		\brief Порядок байт данных.
		*/
		using error_type = std::int32_t;
		using data_array_type = std::vector<std::uint8_t>;
	private:
		
		bool canEncode(const BYTE_ORDER encode, const data_array_type::size_type size) const;
		bool createContext();
	protected:
		error_type lastError{};
		modbus_t* ctx; ///< Контекст modbus 
		ModbusConfig config;
		/**
		\brief Метод сбрасывающий ID текущего/ведомого устройства
		*/
		void resetSlave();
		/**
		\brief Метод устанавливающий ID текущего/ведомого устройства
		\param id Устанавливаемое ID
		*/
		bool setSlave(const std::int32_t id);

	public:
		/**
		\brief Метод устанавливающий новый config
		\param[in] newConfig Новый config
		*/
		void setConfig(const ModbusConfig& newConfig);
		/**
		\brief Метод применяющий настройки из config`а
		\return Результат применения новых настроек
		*/
		bool acceptConfig();
		/**
		\brief Метод устанавливающий новый config и применяющий настройки из него
		\param[in] newConfig Новый config
		\return Результат применения новых настроек
		*/
		bool acceptConfig(const ModbusConfig& newConfig);
		/**
		\brief Метод возвращающий текущий config
		\return config
		*/
		ModbusConfig getConfig();
		/**
		\brief Метод создающий контекст
		\return Если контекст был создан - true, иначе false
		*/
		bool open();
		/**
		\brief Метод закрывающий Modbus-соединение
		*/
		void close();
		/**
		\brief Метод устанавливающий Modbus-соединение
		\return Если соединение было установлено true, иначе false
		*/
		bool connect();
		/**
		\brief Метод Возвращающий последнюю ошибку
		Альтернативный вариант - errno
		\return Код последней ошибки
		*/
		error_type errorCode() const;
		/**
		\brief Метод Возвращающий текстовое представление последней ошибки
		\return  Текстовое представление последней ошибки
		*/
		std::string what() const;
		/**
		\brief Метод изменяющий порядок байт в пришедших/хранящихся данных
		\param[in] data Изменяемые данные
		\param[in] encode Текущий порядок байт (для приведения к виду ABCD) или необходимый порядок (если данные в виде ABCD)
		\return В случае успеха возвращает данные с измененным порядком байт, иначе пустой массив
		*/
		data_array_type decode(const data_array_type& data, const BYTE_ORDER encode) const;
		/**
		\brief Метод изменяющий порядок байт в пришедших/хранящихся данных
		\param[in, out] data Изменяемые данные
		\param[in] encode Текущий порядок байт (для приведения к виду ABCD) или необходимый порядок (если данные в виде ABCD)
		\return Если изменение порядка удалось - true, иначе false
		*/
		bool decode(data_array_type& data, const BYTE_ORDER encode) const;
		virtual ~ModbusBase();
	};

}