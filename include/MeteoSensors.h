#ifndef METEOSENSORS
#define METEOSENSORS

#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>

#define BMP_SCK       18
#define BMP_MISO      19
#define BMP_MOSI      23
#define BMP_CS        5

class MeteoSensors {
public:
    MeteoSensors();

    virtual ~MeteoSensors();

    bool begin();

    void read();

    float getTemperature() const;

    float getHumidity() const;

    float getPressure() const;

private:
    Adafruit_BMP280* m_bmp280;
    Adafruit_AHTX0* m_aht10;

    float m_temperature;
    float m_humidity;
    float m_pressure;
};

#endif