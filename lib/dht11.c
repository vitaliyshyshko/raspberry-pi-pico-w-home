#include "dht11.h"

const uint DHT_PIN = 15;
const uint MAX_TIMINGS = 85;

static void dht11_rst(void)
{
    gpio_set_dir(DHT_PIN, GPIO_OUT);	//SET OUTPUT
    gpio_put(DHT_PIN, 0); 	//GPIOA.0=0
    sleep_ms(20);    	//Pull down Least 18ms
    gpio_put(DHT_PIN,1);	//GPIOA.0=1
    sleep_us(30);     	//Pull up 20~40us
}

static uint dht11_check(void)
{
    uint retry=0;
    gpio_set_dir(DHT_PIN, GPIO_IN);//SET INPUT
    while (gpio_get(DHT_PIN)&&retry<100)//DHT11 Pull down 40~80us
    {
        retry++;
        sleep_us(1);
    };
    if(retry>=100)
        return 1;
    else
        retry=0;
    while (!gpio_get(DHT_PIN)&&retry<100)//DHT11 Pull up 40~80us
    {
        retry++;
        sleep_us(1);
    };
    if(retry>=100)
        return 1;//chack error
    return 0;
}

static uint dht11_read_bit(void)
{
    uint retry=0;
    while(gpio_get(DHT_PIN)&&retry<100)//wait become Low level
    {
        retry++;
        sleep_us(1);
    }
    retry=0;
    while(!gpio_get(DHT_PIN)&&retry<100)//wait become High level
    {
        retry++;
        sleep_us(1);
    }
    sleep_us(40);//wait 40us
    if(gpio_get(DHT_PIN))
        return 1;
    else
        return 0;
}

static uint dht11_read_byte(void)
{
    uint i,dat;
    dat=0;
    for (i=0;i<8;i++)
    {
        dat<<=1;
        dat|=dht11_read_bit();
    }
    return dat;
}

uint dht11_read_data(dht_reading *result)
{
    uint buf[5];
    uint i;
    dht11_rst();
    if(dht11_check()==0)
    {
        for(i=0;i<5;i++)
        {
            buf[i]=dht11_read_byte();
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {
            result->humidity=buf[0];
            result->temp_celsius=buf[2];
        }
    }
    else
        return 1;
    return 0;
}

static uint dht11_init(void)
{
    dht11_rst();
    return dht11_check();
}