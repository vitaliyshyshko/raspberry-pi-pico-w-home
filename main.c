#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

#include "lib/display.h"
#include "lib/dht11.h"

const char TEMPERATURE_UNIT = 'C';
const char VOLTAGE_UNIT = 'V';
const char HUMIDITY_UNIT = '%';
const float CONVERSION_FACTOR = 3.3f / (1 << 12);
const uint SLEEP_MS = 1000 * 60; // 1min

struct onboard_data {
    float voltage;
    float temperature;
};

int init_wifi() {
    // Initialize the CYW43 architecture
    printf("Initialize WiFi\n");

    if (cyw43_arch_init()) {
        printf("Failed to initialise WiFi\n");
        return 1;
    }
    printf("Initialised WiFi\n");

    // Enables Wi-Fi STA (Station) mode
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Failed to connect WiFi\n");
        return 1;
    }
    printf("WiFi connected\n");

    return 0;
}

void init_adc() {
    printf("Initialize ADC\n");

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    printf("Initialised ADC\n");
}

void read_onboard_data(struct onboard_data *result) {
    float voltage = (float)adc_read() * CONVERSION_FACTOR;
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;

    result->voltage = voltage;
    result->temperature = temperature;
}

int main() {
    printf("Initialize\n");

    // Initialize all the present standard stdio types that are linked into the binary
    stdio_init_all();
    init_wifi();
    init_adc();
    init_lcd();
    gpio_init(15);

    while (true) {
        // Onboard Voltage/Temperature
        struct onboard_data onboard_data;
        read_onboard_data(&onboard_data);

        // DHT11 Temperature/Humidity
        dht_reading dht;
        dht11_read_data(&dht);

        lcd_set_cursor(0, 0);
        char onboardVoltageMessage[20];
        sprintf(onboardVoltageMessage, "B.volt=%.02f%c", onboard_data.voltage, VOLTAGE_UNIT);
        printf("%s\n", onboardVoltageMessage);
        lcd_string(onboardVoltageMessage);

        lcd_set_cursor(1, 0);
        char onboardTemperatureMessage[20];
        sprintf(onboardTemperatureMessage, "B.temp=%.02f%c", onboard_data.temperature, TEMPERATURE_UNIT);
        printf("%s\n", onboardTemperatureMessage);
        lcd_string(onboardTemperatureMessage);

        lcd_set_cursor(2, 0);
        char temperatureMessage[20];
        sprintf(temperatureMessage, "Temperature=%.02f%c", dht.temp_celsius, TEMPERATURE_UNIT);
        printf("%s\n", temperatureMessage);
        lcd_string(temperatureMessage);

        lcd_set_cursor(3, 0);
        char humidityMessage[20];
        sprintf(humidityMessage, "Humidity=%.02f%c", dht.humidity, HUMIDITY_UNIT);
        printf("%s\n", humidityMessage);
        lcd_string(humidityMessage);

        sleep_ms(SLEEP_MS);
        lcd_clear();
    }

    // De-initialize the CYW43 architecture
    cyw43_arch_deinit();

    return 0;
}