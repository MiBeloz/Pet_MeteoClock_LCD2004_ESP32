#include "MeteoSensors.h"

MeteoSensors::MeteoSensors() : m_temperature(0.0f), m_humidity(0.0f), m_pressure(0.0f) {
    m_bmp280 = new Adafruit_BMP280(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);
    m_aht10 = new Adafruit_AHTX0;
}

MeteoSensors::~MeteoSensors() {
    delete m_bmp280;
    delete m_aht10;
}

bool MeteoSensors::begin() {
    if (!m_bmp280->begin()) {
        return false;
    }
    m_bmp280->setSampling(
        Adafruit_BMP280::MODE_NORMAL,                           // Режим работы
        Adafruit_BMP280::SAMPLING_X16,                          // Точность изм. температуры
        Adafruit_BMP280::SAMPLING_X16,                          // Точность изм. давления
        Adafruit_BMP280::FILTER_X16,                            // Уровень фильтрации
        Adafruit_BMP280::STANDBY_MS_2000);                      // Период просыпания, мСек

    if (!m_aht10->begin()) {
        return false;
    }

    return true;
}

void MeteoSensors::read() {
    sensors_event_t temperatureSensor, humiditySensor;
    m_aht10->getEvent(&humiditySensor, &temperatureSensor);
    m_temperature = temperatureSensor.temperature;
    m_humidity = humiditySensor.relative_humidity;
    m_pressure = m_bmp280->readPressure() * 0.00750063755419211;
}

float MeteoSensors::getTemperature() const {
    return m_temperature;
}

float MeteoSensors::getHumidity() const {
    return m_humidity;
}

float MeteoSensors::getPressure() const {
    return m_pressure;
}
