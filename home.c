#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

const char TEMPERATURE_UNIT = 'C';
const char VOLTAGE_UNIT = 'V';
const float CONVERSION_FACTOR = 3.3f / (1 << 12);
const uint SLEEP_MS = 5000;

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

    while (true) {
        // Onboard Voltage/Temperature
        struct onboard_data onboard_data;
        read_onboard_data(&onboard_data);
        printf("Onboard voltage = %.02f%c\n", onboard_data.voltage, VOLTAGE_UNIT);
        printf("Onboard temperature = %.02f%c\n", onboard_data.temperature, TEMPERATURE_UNIT);

        sleep_ms(SLEEP_MS);
    }

    // De-initialize the CYW43 architecture
    cyw43_arch_deinit();

    return 0;
}