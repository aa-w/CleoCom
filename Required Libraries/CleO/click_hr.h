
#ifndef CLICK_HR_H_
#define CLICK_HR_H_

// NOTE: Pulse Width determines the ADC resolution
// The Sample Rate also sets an upper bound on the Pulse Width
// device registers
#define I2C_ADR 0x57
#define EXP_PART_ID  0x11

// Registers addresses
#define INT_STATUS       0x00
#define INT_ENABLE       0x01
#define FIFO_WRITE_PTR   0x02
#define OVER_FLOW_CNT    0x03
#define FIFO_READ_PTR    0x04
#define FIFO_DATA_REG    0x05
#define MODE_CONFIG      0x06
#define SPO2_CONFIG      0x07
#define LED_CONFIG       0x09
#define TEMP_INTEGER     0x16
#define TEMP_FRACTION    0x17
#define REVISION_ID      0xFE

// @section Mode configuration bits
#define TEMP_EN          0x08
#define HR_ONLY          0x02
#define SPO2_EN          0x03


// @section SpO2 configuration bits
#define SPO2_HI_RES_EN   0x40

// @sectionInterrupt enable bits
#define ENA_A_FULL       0x80
#define ENA_TEP_RDY      0x40
#define ENA_HR_RDY       0x20
#define ENA_SO2_RDY      0x10

// Interrupt status bits
#define PWR_RDY          0x01

// sample rate control bits [samples per second]
#define SAMPLES_50       0x00
#define SAMPLES_100      0x04
#define SAMPLES_167      0x08
#define SAMPLES_200      0x0C
#define SAMPLES_400      0x10
#define SAMPLES_600      0x14
#define SAMPLES_800      0x18
#define SAMPLES_1000     0x1C

// LED pulse width control bits - pulse width [us]
#define PULSE_WIDTH_200  0x00 // 13-bit ADC resolution
#define PULSE_WIDTH_400  0x01 // 14-bit ADC resolution
#define PULSE_WIDTH_800  0x02 // 15-bit ADC resolution
#define PULSE_WIDTH_1600 0x03 // 16-bit ADC resolution

// LED current control bits [ma]
#define IR_CURRENT_0     0x00 // 0.0 mA
#define IR_CURRENT_44    0x01 // 4.4 mA
#define IR_CURRENT_76    0x02 // 7.6 mA
#define IR_CURRENT_110   0x03 // 11.0 mA
#define IR_CURRENT_142   0x04 // 14.2 mA
#define IR_CURRENT_174   0x05 // 17.4 mA
#define IR_CURRENT_208   0x06 // 20.8 mA
#define IR_CURRENT_240   0x07 // 24.0 mA
#define IR_CURRENT_271   0x08 // 27.1 mA
#define IR_CURRENT_306   0x09 // 30.6 mA
#define IR_CURRENT_338   0x0A // 33.8 mA
#define IR_CURRENT_370   0x0B // 37.0 mA
#define IR_CURRENT_402   0x0C // 40.2 mA
#define IR_CURRENT_436   0x0D // 43.6 mA
#define IR_CURRENT_468   0x0E // 46.8 mA
#define IR_CURRENT_500   0x0F // 50.0 mA

#define RED_CURRENT_0    0x00 // 0.0 mA
#define RED_CURRENT_44   0x10 // 4.4 mA
#define RED_CURRENT_76   0x20 // 7.6 mA
#define RED_CURRENT_110  0x30 // 11.0 mA
#define RED_CURRENT_142  0x40 // 14.2 mA
#define RED_CURRENT_174  0x50 // 17.4 mA
#define RED_CURRENT_208  0x60 // 20.8 mA
#define RED_CURRENT_240  0x70 // 24.0 mA
#define RED_CURRENT_271  0x80 // 27.1 mA
#define RED_CURRENT_306  0x90 // 30.6 mA
#define RED_CURRENT_338  0xA0 // 33.8 mA
#define RED_CURRENT_370  0xB0 // 37.0 mA
#define RED_CURRENT_402  0xC0 // 40.2 mA
#define RED_CURRENT_436  0xD0 // 43.6 mA
#define RED_CURRENT_468  0xE0 // 46.8 mA
#define RED_CURRENT_500  0xF0 // 50.0 mA

#define REG_INT_STATUS				0x00
#define REG_INT_ENABLE				0x01
#define REG_FIFO_WRITE_PTR			0x02
#define REG_FIFO_OVERFLOW_CNTR		0x03
#define REG_FIFO_READ_PTR			0x04
#define REG_FIFO_DATA_REG			0x05
#define REG_CONFIG_MODE				0x06
#define REG_CONFIG_SPIO2			0x07
#define REG_CONFIG_LED				0x09
#define REG_TEMP_INT				0x16
#define REG_TEMP_FRACT				0x17
#define REG_REV_ID					0xFE
#define REG_PART_ID					0xFF


#define ENABLE_HR_INT				(0x01 << 5)
#define ENABLE_S02_INT				(0x01 << 4)
#define MODE_RESET					(0x01 << 6)
#define MODE_HR_ONLY				0x02
#define MODE_SPIO2					0x03
#define LED_PW_400					0x01
#define PART_ID						0x11
#define CURRENT_11mA				0x03
#define CURRENT_50mA				0x0F
#define INT_STATUS_HR_RDY			(0x01 << 5)
#define SR_400_SPS					(0x04)
#define SR_200_SPS					(0x03)
#define SR_100_SPS					(0x01)


#define SAMPLE_RATE					(100)	/* Sample rate in Hz. */
#define SR_HZ						SAMPLE_RATE
#define HPBUFFER_LGTH				50
#define DERIV_LENGTH				4
#define LED_CURRENT					CURRENT_50mA
#define SAMPLE_RATE_CFG				(SR_100_SPS << 2)
#define SR							400u


/***************************************************************************//**
 An Average class to encapsulate averaging algorithms, note size can't be > 16
******************************************************************************/
class Avg {
private:
	int16_t dataWW[16];
	uint16_t ptr_mw;
	uint8_t Size;
	int32_t mwSum;
public:
	Avg(uint8_t siz) {
		Size = siz;
	}
	int16_t GetAvg(void) {
		return ((int16_t)(mwSum / Size));
	}
	int16_t Update(int16_t datum, bool init) {
		return simple(datum, init);
	}

private:
	int16_t simple(int16_t datum, bool init) {
		if (init) {
			for (uint16_t i = 0; i < Size; i++) {
				dataWW[i] = 0;
			}
			mwSum = 0;
			ptr_mw = 0;
			return 0;
		}
		mwSum += datum;
		mwSum -= dataWW[ptr_mw];
		dataWW[ptr_mw] = datum;
		if (++ptr_mw == Size) {
			ptr_mw = 0;
		}
		return ((int16_t)(mwSum / Size));
	}
};

#define PK_TIMEOUT				    (uint16_t)(SR_HZ * 2) /* ~ 2S without a peak */
#define MV_FILT_DELAY			    (6)
#define PK_MIN_AMP				    (10)
#define SIG_THRESH_TIMEOUT		(3)
#define SIG_THRESH_AVG_CNT		(5u)
class Filter {
private:
  int16_t X_n1;
  int16_t Y_n1;
  uint16_t max, timeSinceMax, lastDatum, threshTimeout;
  Avg SignalThresh; /* Averages the detector threshold */
public:
  Filter() : SignalThresh(SIG_THRESH_AVG_CNT) { }
  /* A differentiator followed by a leaky integrator */
    int16_t DC(int16_t datum, bool init) {
		int16_t y;
		if (init) {
			X_n1 = 0;
			Y_n1 = 0;
			return 0;
		}
		y = datum - X_n1;
		y = Y_n1 - (Y_n1 >> 6) + y;
		X_n1 = datum;
		Y_n1 = y;
		return y;
	}

	/* Returns non-zero if a peak was detected, along with the delay from true maximum sample.
	A peak is detected if the current sample is positive AND it reaches TRUE_MAX / 4
	If no peak is detected in 2 Seconds the filter is reset */
	uint16_t Peak(uint16_t datum, int16_t idatum, uint16_t* dly, bool init) {
		uint16_t pk = 0;
		*dly = 0;
		if (init) {
			max = 0;
			timeSinceMax = 0;
			lastDatum = 0;
			return pk;
		}
		timeSinceMax++;
		if ((idatum > 0) && (datum > max) && (datum > lastDatum)) {
			max = datum;
			timeSinceMax = 0;
		}
		else if ((idatum > 0) && (datum < (max >> 2))) {
			pk = max;
			*dly = timeSinceMax;
			timeSinceMax = 0;
			max = 0;
		}
		else if (timeSinceMax >= PK_TIMEOUT) {
			timeSinceMax = 0;
			max = 0;
            #if 0
			Serial.print("PK-Timeout: "); Serial.print(timeSinceMax); Serial.print('\n');
            #endif
		}
		lastDatum = datum;
		return pk;
	}

	/*
	Algorithm to decide if a detected peak is a valid Heart Rate peak.
	An adaptive threshold of the average of all valid peaks is stored. If the
	current peak is above this threshold, it is considered a valid peak.
	If there are more than 3 consequitive peaks that don't make it past
	the threshold, reset the threshold.
	*/
	bool Decide(uint16_t pk, uint16_t sampleIdx, bool init) {
		if (init) {
			SignalThresh.Update(0, true);
			threshTimeout = 0;
			return false;
		}
		if (((sampleIdx - MV_FILT_DELAY) > 0)) {
			if ((pk > (int16_t)PK_MIN_AMP) && (pk > SignalThresh.GetAvg() / 2)) {
				SignalThresh.Update(pk, false); /* Update signal threshold */
				threshTimeout = 0;
				return true;
			}
			else {
				threshTimeout++;
				if (threshTimeout >= SIG_THRESH_TIMEOUT) SignalThresh.Update(0, true);
#if 0
				Serial.print("thresh: "); Serial.print(SignalThresh.GetAvg());
				Serial.print("\tpk: "); Serial.print(pk);
				Serial.print("\n");
#endif
			}
		}
		return false;
	}

	uint16_t GetThreshold(void) {
		return SignalThresh.GetAvg();
	}

};

typedef  void (*WriteHook) (byte reg, byte* pData, byte len);
typedef  void (*WriteByteHook) (byte reg, byte value);
typedef void (*ReadHook) (byte reg, byte* pData, byte len);

class HRClick {
private:
  WriteHook i2c_write;
  ReadHook i2c_read;
  WriteByteHook  i2c_write_byte;

public:
	/* Configure the Click to work in "Heart Rate" mode with 100
	samples-per-second and 14 bit resolution */
	bool Init(WriteHook w, WriteByteHook wb, ReadHook r) {
        i2c_write = w;
        i2c_write_byte = wb;
        i2c_read = r;
		/* Setup the Wire Library to talk to the Heart Rate Click */
		uint8_t data[4];
		// reset the sensor
		i2c_write_byte(REG_CONFIG_MODE, MODE_RESET);
		delay(500);

		data[0] = (uint8_t)LED_CURRENT << 4 | (uint8_t)LED_CURRENT;
		i2c_write(REG_CONFIG_LED, data, 1);
		i2c_read(REG_CONFIG_LED, data, 1);

		i2c_write_byte(REG_INT_ENABLE, ENABLE_HR_INT);
		i2c_write_byte(REG_CONFIG_SPIO2, SAMPLE_RATE_CFG | LED_PW_400);

		i2c_read(REG_CONFIG_SPIO2, data, 1);
		// Clear FIFO pointers
		i2c_write_byte(REG_FIFO_WRITE_PTR, 0);
		i2c_write_byte(REG_FIFO_READ_PTR, 0);
		i2c_write_byte(REG_FIFO_OVERFLOW_CNTR, 0);
		i2c_write_byte(REG_CONFIG_MODE, MODE_HR_ONLY);

		i2c_read(REG_CONFIG_MODE, data, 1);
		i2c_read(REG_FIFO_OVERFLOW_CNTR, data, 1);
        if(read_part_id() == EXP_PART_ID){
            InitSuccessful = true;
        }
        else {
            InitSuccessful = false;
            Serial.print("HR Click Init failed!");
        }
        return InitSuccessful;
	}

	uint8_t ReadAvailable(int16_t* pData) {
		uint8_t data;
        if(!InitSuccessful){
            /* If sensor is not available, return 0's */
            data = 0;
        }
		/* Read the MAX30100 status register */
		i2c_read(REG_INT_STATUS, &data, 1);
		/* Check if new samples are available */
		if ((data & INT_STATUS_HR_RDY) == INT_STATUS_HR_RDY) {
			data = get_raw_value(pData, NULL);
		}
		else {
			data = 0;
		}
		return data;
	}

    byte read_part_id(void) {
		byte id;
		i2c_read(byte(REG_PART_ID), &id, 1);
		//Serial.println("Part ID:");
		//Serial.print(id, HEX);         // expected value is 0x11
        return id;
	}
    
    byte read_revision_id(void) {
		byte id;
		i2c_read(byte(REG_REV_ID), &id, 1);
		//Serial.print("REG_REV_ID ID:");
		//Serial.println(id, HEX);
        return id;
	}
private:
    bool InitSuccessful; /* Flag to know if sensor is initialized properly */	

	/* In HR mode we don't use the RED LED values, so that's skipped */
	byte get_raw_value(int16_t *ir_buff, int16_t *red_buff) {
		byte i, sampleNum = 0;
		byte wrPtr = 0, rdPtr = 0, samples[4];
		i2c_read(FIFO_WRITE_PTR, &wrPtr, 1);
		i2c_read(FIFO_READ_PTR, &rdPtr, 1);

		sampleNum = (wrPtr - rdPtr) & 0x0F;
		
		if (sampleNum >= 1) {
			for (i = 0; i < sampleNum; ++i) {
				// read data
				i2c_read(FIFO_DATA_REG, samples, 4);
				((uint8_t *)ir_buff)[0] = samples[1];
				((uint8_t *)ir_buff++)[1] = samples[0];
#if 0 /* enable to read red led */
				((uint8_t *)red_buff)[0] = samples[3];
				((uint8_t *)red_buff++)[1] = samples[2];
#endif
			}
		}
		return sampleNum;
	}

};

#endif /* CLICK_HR_H_ */
