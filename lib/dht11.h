#ifndef MAIN_DHT11_H
#define MAIN_DHT11_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

typedef struct {
    float humidity;
    float temp_celsius;
} dht_reading;

static void dht11_rst(void);

static uint dht11_check(void);

static uint dht11_read_bit(void);

static uint dht11_read_byte(void);

uint dht11_read_data(dht_reading *result);

static uint dht11_init(void);

#endif //MAIN_DHT11_H
