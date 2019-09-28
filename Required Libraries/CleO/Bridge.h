/*
 * Bridge.h
 *
 *    Created on: Mar 18, 2016
 */

#ifndef ARDUINO_BRIDGE_H_
#define ARDUINO_BRIDGE_H_

#include <stdint.h>
#include <string.h> //memset

#ifndef NULL
#define NULL    0u
#endif
#ifndef PACK
#define PACK    __attribute__((__packed__))
#endif

#define BRIDGE_TYPE_GPIO                    0x01
#define BRIDGE_TYPE_SPI                        0x02
#define BRIDGE_TYPE_UART                    0x03
#define BRIDGE_TYPE_I2CM                    0x04
#define BRIDGE_TYPE_ADC                        0x05
#define BRIDGE_TYPE_PWM                        0x06

#define GPIO_SPIM_SS1                        (33)
#define GPIO_SPIM_SS3                        (35)

#define GPIO_CLICK_1                        GPIO_SPIM_SS1
#define GPIO_CLICK_2                        GPIO_SPIM_SS3

#define INDEFINITE_BLOCKING 0xFFFF

 // GPIO Bridge: defines the order of the elements in the GPIOMap table stored internally in CleO
#define CLICK1_AN        0
#define CLICK1_RST        1
#define CLICK1_CS        2
#define CLICK1_PWM        3
#define CLICK1_INT        4
#define CLICK1_SDA        5
#define CLICK1_SCL        6
#define CLICK1_TXD        7
#define CLICK1_RXD        8
#define CLICK1_MOSI        9
#define CLICK1_MISO        10
#define CLICK1_SCK        11

#define CLICK2_AN        12
#define CLICK2_RST        13
#define CLICK2_CS        14
#define CLICK2_PWM        15
#define CLICK2_INT        16
#define CLICK2_SDA        CLICK1_SDA
#define CLICK2_SCL        CLICK1_SCL
#define CLICK2_TXD        17
#define CLICK2_RXD        18
#define CLICK2_MOSI        CLICK1_MOSI
#define CLICK2_MISO        CLICK1_MISO
#define CLICK2_SCK        CLICK1_SCK
#define CLEO35_CN5_1    19
#define CLEO35_CN5_2    20

#define GPIO_DIR_INPUT        1
#define GPIO_DIR_OUTPUT        0

#define ID_UART0    0u
#define ID_UART1    1u

#define CLICK1_UART_ID                ID_UART0
#define CLICK2_UART_ID                ID_UART1
#define UART_DEFAULT_BUFF_SIZE        (1024u)

#define CLICK1_ADC_CHANNEL                     (3)
#define CLICK2_ADC_CHANNEL                     (1)

#define CLICK1_PWM_CHANNEL                     (1)
#define CLICK2_PWM_CHANNEL                     (2)

#define Z_GPIO_PINMAP(gpio)                     (MASK_ONE_U32 << gpio)
#define Z_GPIO_DIRECTION(gpio, dir)              (((uint32_t)dir) << gpio)
#define Z_GPIO_PULL_UP_CFG(gpio, cfg)            (((cfg) << ((gpio) << 1)))

#define MASK_ONE_U32                            ((uint32_t)0x00000001)
#define Z_BitSet(Reg,Mask)                        ((Reg) |= (Mask))
#define Z_BitClear(Reg,Mask)                    ((Reg) &= ~(Mask))
#define Z_BitToggle(Reg,Mask)                    ((Reg) ^= (Mask))
#define Z_GetBitMask(x)                            (1u << (x))
#define Z_BitIsSet(val, mask)                    (((val) & (mask)) == (mask))

#define Z_MAKEU32(p)                            ((uint32_t) p[0] | ((uint32_t)p[1] << 8) | ((uint32_t) p[2] << 16) | ( (uint32_t)p[3] << 24) )


// This class provides wrappers for easy use of the Bridging API
class Bridge {

    /** @brief The SPI mode */
public:
    typedef enum
    {
        CleO50_click_1 = 0,  /** Click 1 on Cleo50 board */
        CleO50_click_2 = 1,  /** Click 2 on Cleo50 board */
        CleO35_CN5 = 0,  /** CN5 connector on Cleo35 board */
    } interfaces;
    typedef enum
    {
        spi_mode_0,     /**< CPOL = 0, CPHA = 0 */
        spi_mode_1,     /**< CPOL = 0, CPHA = 1 */
        spi_mode_2,     /**< CPOL = 1, CPHA = 0 */
        spi_mode_3     /**< CPOL = 1, CPHA = 1 */
    } spi_clock_mode_t;

    /** @brief Pad pull up and pull downs control. */
    typedef enum
    {
        pad_pull_none,        /**< No pull up or pull down */
        pad_pull_pullup,    /**< Weak pull up enabled */
        pad_pull_pulldown,    /**< Weak pull down enabled */
        pad_pull_keeper        /**< Weak pull up/down reflects output */
    } pad_pull_t;

    typedef enum {
        Output,
        Input
    }gpio_dir;

    typedef struct {
        uint8_t id;
        gpio_dir dir;
        pad_pull_t pullup;
    } gpio;

    typedef enum
    {
        adc_mode_single,        /*!< One analogue reading will be taken and then the ADC stopped */
        adc_mode_continuous        /*!< The ADC will continuously aquire analogue readings */
    } adc_mode_t;

    typedef enum{
        b2400 = 2400,
        b4800 = 4800,
        b9600 = 9600,
        b19200 = 19200,
        b31250 = 31250,
        b38400 = 38400,
        b57600 = 57600,
        b115200 = 115200,
        b230400 = 230400,
        b460800 = 460800,
        b921600 = 921600,
    }uart_baud;

    typedef struct PACK {
        union PACK {
            struct PACK {
                uint32_t Speed;
                uint16_t ClockDivider; /* Set to 0 to allow system to auto-select clock divider based on configured speed */
                uint8_t SlaveSelectPin;
                uint8_t BusWidth;
                uint8_t Mode;
            };
            uint8_t b[9];
        };
    } spim_options;

    typedef     struct PACK {
        union PACK {
            struct PACK {
                uint16_t SlaveAddress; /* Verify if SPIM actually supports 10 bit address ! */
                uint32_t ClockSpeed;
            };
            uint8_t b[6];
        };
    }i2cm_options;

    typedef struct PACK {
        union PACK {
            struct PACK {
                uint32_t PinMap;
                uint32_t Direction;
                uint64_t PullUpCfg;    // 4 possible states - none, up, down, keeper
            };
            uint8_t b[16];
        };
    }gpio_options;

    typedef struct PACK {
        union PACK {
            struct PACK {
                uint32_t BaudRate;    
                uint16_t BufferSize;
                uint16_t RxTimeout; /* Timeout for Reads in mS(0xFFFF = infinite, ) */
                uint8_t PortID;
            };
            uint8_t b[7];
        };
    }uart_options;

    typedef struct PACK {
        union PACK {
            struct PACK {
                uint8_t Mode; /* single shot or continuous */
                uint8_t Channel;
            };
            uint8_t b[2];
        };
    }adc_options;

    typedef struct PACK {
        union PACK {
            struct PACK {
                uint32_t Frequency; /* Hz */
                uint8_t Shots; // shot = 0, means continuous mode
                uint8_t Channels; // bits[0:1] for channel 1 and 2
                uint8_t DutyCycle1;
                uint8_t DutyCycle2;
                uint8_t InitState1; // 0= low, 1= high
                uint8_t InitState2; // 0= low, 1= high
            };
            uint8_t b[10];
        };
    }pwm_options;
private:
    void h2n64(uint64_t data, uint8_t* p) {
        *p++ = data & 0xFF;
        *p++ = (data >> 8) & 0xFF;
        *p++ = (data >> 16) & 0xFF;
        *p++ = (data >> 24) & 0xFF;
        *p++ = (data >> 32) & 0xFF;
        *p++ = (data >> 40) & 0xFF;
        *p++ = (data >> 48) & 0xFF;
        *p = (data >> 56) & 0xFF;
    }

    void h2n32(uint32_t data, uint8_t* p) {
        *p++ = data & 0xFF;
        *p++ = (data >> 8) & 0xFF;
        *p++ = (data >> 16) & 0xFF;
        *p = (data >> 24) & 0xFF;
    }

    void h2n16(uint16_t data, uint8_t* p) {
        *p++ = data & 0xFF;
        *p = (data >> 8) & 0xFF;
    }

public:
    int16_t BytesWritten;
    // These pure virtual functions are auto generated in the encoder.i file, included in the CleO class
    virtual int16_t DeviceOpen(uint8_t Slot, uint8_t Interface, int16_t bytestowrite, uint8_t* pData) = 0;
    virtual int16_t DeviceWrite(uint8_t DevHandle, int16_t bytestowrite, uint8_t* Buffer, int16_t &byteswritten) = 0;
    virtual int16_t DeviceRead(uint8_t DevHandle,int16_t bytestowrite,int16_t bytestoread,uint8_t* Buffer,uint8_t* rBuffer) = 0;
    virtual int16_t DeviceClose(uint8_t DevHandle) = 0;

    /*========================================================*/
    /* SPI BRIDGE */
    uint8_t OpenSPI(interfaces ifs, uint32_t max_speed) {
        return OpenSPI(ifs, max_speed, spi_mode_0);
    }

    uint8_t OpenSPI(interfaces ifs, uint32_t max_speed, uint8_t mode) {
        spim_options opt;
        if (ifs == CleO50_click_1)
            opt.SlaveSelectPin = GPIO_CLICK_1;
        else
            opt.SlaveSelectPin = GPIO_CLICK_2;

        opt.ClockDivider = 0;
        opt.BusWidth = 1;
        opt.Mode = mode;
        h2n32(max_speed, &opt.b[0]); // Speed
        h2n16(opt.ClockDivider, &opt.b[4]);
        return DeviceOpen(0, BRIDGE_TYPE_SPI, sizeof(spim_options), (uint8_t*)&opt);
    }

    uint16_t WriteSPI(uint8_t Handle, uint16_t bytes_to_write, uint8_t* pData) {
        return DeviceWrite(Handle, bytes_to_write, pData, BytesWritten);
    }

    uint16_t ReadSPI(uint8_t Handle, uint16_t bytes_to_read, uint8_t* pData) {
        return DeviceRead(Handle, 0, bytes_to_read, NULL, pData);
    }

    uint16_t XChangeSPI(uint8_t Handle, uint16_t bytes_to_xchg, uint8_t* pDataWrite, uint8_t* pDataRead) {
        return DeviceRead(Handle, bytes_to_xchg, bytes_to_xchg, pDataWrite, pDataRead);
    }
    int16_t CloseSPI(uint8_t Handle){
        return DeviceClose(Handle);
    }
    /*========================================================*/
    /* I2CM BRIDGE */
    uint8_t OpenI2CM(uint32_t max_speed, uint8_t slave_7bit_address) {
        i2cm_options opt;
        opt.ClockSpeed = max_speed;
        opt.SlaveAddress = slave_7bit_address;
        // Convert to byte stream - handle endianness conversion
        h2n16(opt.SlaveAddress, &opt.b[0]);
        h2n32(opt.ClockSpeed, &opt.b[2]);

        return DeviceOpen(0, BRIDGE_TYPE_I2CM, sizeof(i2cm_options), (uint8_t*)&opt);
    }

    uint16_t WriteI2CM(uint8_t Handle, uint16_t bytes_to_write, uint8_t* pData) {
        return DeviceWrite(Handle, bytes_to_write, pData, BytesWritten);
    }

    uint16_t WriteI2CM(uint8_t Handle, uint8_t register_id, uint16_t bytes_to_write, uint8_t* pData) {
        uint8_t b[bytes_to_write + 1];
        b[0] = register_id;
        for (uint8_t i = 0; i < bytes_to_write; i++) {
            b[i + 1] = pData[i];
        }
        return DeviceWrite(Handle, bytes_to_write + 1, b, BytesWritten);
    }

    uint16_t ReadI2CM(uint8_t Handle, uint8_t register_id, uint16_t bytes_to_read, uint8_t* pData) {
        return DeviceRead(Handle, 1, bytes_to_read, &register_id, pData);
    }

    uint16_t ReadI2CM(uint8_t Handle, uint16_t bytes_to_write, uint16_t bytes_to_read, uint8_t* pDataWrite, uint8_t* pDataRead) {
        return DeviceRead(Handle, bytes_to_write, bytes_to_read, pDataWrite, pDataRead);
    }

    int16_t CloseI2CM(uint8_t Handle){
        return DeviceClose(Handle);
    }
    /*========================================================*/
    /* GPIO BRIDGE */
    // Open One GPIO Port
    uint8_t OpenGPIO(uint8_t gpio, gpio_dir direction, pad_pull_t pullup) {
        gpio_options opt;
        memset(&opt, 0, sizeof(opt));


        opt.PinMap = Z_GPIO_PINMAP(gpio);
        opt.Direction = Z_GPIO_DIRECTION(gpio, direction); /*bit is set for input and cleared for output */
        opt.PullUpCfg = Z_GPIO_PULL_UP_CFG(gpio, pullup);

        // Convert to byte stream - handle endianness conversion
        h2n32(opt.PinMap, &opt.b[0]);
        h2n32(opt.Direction, &opt.b[4]);
        h2n64(opt.PullUpCfg, &opt.b[8]);

        return DeviceOpen(0, BRIDGE_TYPE_GPIO, sizeof(gpio_options), (uint8_t*)&opt);
    }
    // Open Multiple GPIO ports
    uint8_t OpenGPIOs(gpio* gpios, uint8_t number_of_elements) {
        gpio_options opt;
        memset(&opt, 0, sizeof(opt));
        for (int i = 0; i < number_of_elements; i++) {
            gpio* g = gpios++;

            opt.PinMap |= Z_GPIO_PINMAP(g->id);
            opt.Direction |= Z_GPIO_DIRECTION(g->id, g->dir); /*bit is set for input and cleared for output */
            opt.PullUpCfg |= Z_GPIO_PULL_UP_CFG(g->id, g->pullup);
        }

        // Convert to byte stream - handle endianness conversion
        h2n32(opt.PinMap, &opt.b[0]);
        h2n32(opt.Direction, &opt.b[4]);
        h2n64(opt.PullUpCfg, &opt.b[8]);

        return DeviceOpen(0, BRIDGE_TYPE_GPIO, sizeof(gpio_options), (uint8_t*)&opt);
    }

    // Read all gpio pins opened as input for this handle
    uint32_t ReadGPIOs(uint8_t Handle) {
        uint8_t data[4];
        DeviceRead(Handle, 0, 4, NULL, &data[0]);
        return Z_MAKEU32(data);
    }

    // read a particular gpio pin
    bool ReadGPIO(uint8_t Handle, uint8_t gpio) {
        uint8_t data[4];
        DeviceRead(Handle, 0, 4, NULL, data);
        uint32_t val = Z_MAKEU32(data);
        return Z_BitIsSet(val, (MASK_ONE_U32 << gpio)) ? 1 : 0;
    }

    // Write all gpio pins opened as output for this handle
    uint16_t WriteGPIOs(uint8_t Handle, uint32_t value) {
        uint8_t val[4];
        h2n32(value, val);
        return DeviceWrite(Handle, 4, val, BytesWritten);
    }

    // write one port
    void WriteGPIO(uint8_t Handle, uint32_t value) {
        uint8_t val[4];
        h2n32(value, val);
        DeviceWrite(Handle, 4, val, BytesWritten);
    }

    int16_t CloseGPIO(uint8_t Handle){
        return DeviceClose(Handle);
    }
    /*========================================================*/
    /* UART BRIDGE */
    uint8_t OpenUART(interfaces ifs, uart_baud BaudRate, uint16_t readTimeout = INDEFINITE_BLOCKING) {
        return OpenUART(ifs, BaudRate, UART_DEFAULT_BUFF_SIZE, readTimeout);
    }

    uint8_t OpenUART(interfaces ifs, uart_baud BaudRate, uint16_t rx_buffer_size, uint16_t readTimeout) {
        uart_options opt;
                
        opt.BaudRate = BaudRate;
        opt.PortID = ifs;
        opt.BufferSize = rx_buffer_size;
        opt.RxTimeout = readTimeout;

        // Convert to byte stream - handle endianness conversion
        h2n32(opt.BaudRate, &opt.b[0]);
        h2n16(opt.BufferSize, &opt.b[4]);
        h2n16(opt.RxTimeout, &opt.b[6]);

        return DeviceOpen(0, BRIDGE_TYPE_UART, sizeof(uart_options), (uint8_t*)&opt);
    }

    uint16_t WriteUART(uint8_t Handle, uint16_t bytes_to_write, uint8_t* pData) {
        return DeviceWrite(Handle, bytes_to_write, pData, BytesWritten);
    }

#define OPT_NONBLOCKING        0u
#define OPT_BLOCKING        1u
    // Will read all the available bytes currenty in the Rx buffer. Won't exceed the size of the buffer
    uint16_t ReadAvailableUART(uint8_t Handle, uint8_t* pData) {
        uint8_t opt = OPT_NONBLOCKING;
        return DeviceRead(Handle, 1, 0, &opt, pData);
    }

    // WIll read the Min of [bytes_to_read, bytes_available]
    uint16_t ReadUART(uint8_t Handle, uint16_t bytes_to_read, uint8_t* pData) {
        uint8_t opt = OPT_BLOCKING;
        return DeviceRead(Handle, 1, bytes_to_read, &opt, pData);
    }

    int16_t CloseUART(uint8_t Handle){
        return DeviceClose(Handle);
    }
    /*========================================================*/
    /* ADC BRIDGE */
    uint8_t OpenADC(uint8_t channel) {
        adc_options opt;

        opt.Channel = channel;
        opt.Mode = adc_mode_single;

        return DeviceOpen(0, BRIDGE_TYPE_ADC, sizeof(adc_options), (uint8_t*)&opt);
    }

    uint16_t ReadADC(uint8_t Handle, uint16_t samples_to_read, uint16_t* pData) {
        return DeviceRead(Handle, 0, samples_to_read << 1, NULL, (uint8_t*) pData);
    }

    uint16_t ReadADCSample(uint8_t Handle) {
        uint8_t datum[2] = {0,0};
        DeviceRead(Handle, 0, 2, NULL, datum);
        return (uint16_t)datum[0] | (datum[1] << 8);
    }

    int16_t CloseADC(uint8_t Handle){
        return DeviceClose(Handle);
    }
    /*========================================================*/
    /* PWM BRIDGE */
    // Only one frequency possible for both channels. Multiple channels are selected by bit-OR
    uint8_t OpenPWM(uint8_t channels, uint32_t frequency, uint8_t shots, uint8_t duty_cycle_chn_1, uint8_t duty_cycle_chn_2, bool init_state_chn_1, bool init_state_chn_2) {
        pwm_options opt;

        if (duty_cycle_chn_1 > 100)
            duty_cycle_chn_1 = 100;
        if (duty_cycle_chn_2 > 100)
            duty_cycle_chn_2 = 100;

        opt.Channels = channels;
        opt.DutyCycle1 = duty_cycle_chn_1;
        opt.DutyCycle2 = duty_cycle_chn_2;
        opt.InitState1 = init_state_chn_1;
        opt.InitState2 = init_state_chn_2;
        opt.Shots = shots;
        opt.Frequency = frequency;
        
        h2n32(opt.Frequency, &opt.b[0]);

        return DeviceOpen(0, BRIDGE_TYPE_PWM, sizeof(pwm_options), (uint8_t*)&opt);
    }
    
    int16_t ClosePWM(uint8_t Handle){
        return DeviceClose(Handle);
    }

};

#endif /* ARDUINO_BRIDGE_H_ */
