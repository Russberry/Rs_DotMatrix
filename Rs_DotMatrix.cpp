#include "Rs_DotMatrix.h"
#include <SPI.h>
#include <arduino.h>
#include <stdint.h>
#include <util/delay.h>

Rs_DotMatrix::Rs_DotMatrix(uint8_t CS, uint8_t intensity) {
    _CS = CS;
    pinMode(_CS, OUTPUT);
    digitalWrite(_CS, HIGH);
    SPI.begin();
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE2));

    this->Write(CMD_TestMode, 0x00);
    this->Write(CMD_Shutdown, 0x01);
    this->Write(CMD_Intensity, intensity);
    this->Write(CMD_ScanLimit, 0x07);

    this->ClearDisplay();
}

void Rs_DotMatrix::PrintChar(char input) {
    uint8_t characidx = Convert2Idx(input);
    for (int i = 0; i < 8; i++) {
        this->Write(i + 1, FONT[characidx][i]);
    }
}

void Rs_DotMatrix::PrintString(char *str, uint16_t durationms) {
    for (int i = 0; str[i] != '\0'; i++) {
        this->PrintChar(str[i]);
        _delay_ms(durationms);
    }
}

void Rs_DotMatrix::PrintScrollString(char *str, uint16_t durationms) {
    uint8_t build = 0x00;
    for (int idx = 0; str[idx + 1] != '\0'; idx++) {
        for (int shift = 0; shift < 8; shift++) {
            for (int row = 0; row < 8; row++) {
                build = (FONT[Convert2Idx(str[idx])][row] << shift) & 0xFF;
                build |= (FONT[Convert2Idx(str[idx + 1])][row] >> (8 - shift)) & 0xFF;
                this->Write(row + 1, build);
            }
            _delay_ms(durationms / 8);
        }
    }
}

void Rs_DotMatrix::ClearDisplay() {
    for (int i = 0; i < 8; i++) {
        digitalWrite(_CS, LOW);  
        SPI.transfer(i + 1);
        SPI.transfer(0x00);
        digitalWrite(_CS, HIGH);  
    }
}

void Rs_DotMatrix::DisplayOff() {
    this->Write(CMD_Shutdown, 0x00);
}

void Rs_DotMatrix::DisplayOn() {
    this->Write(CMD_Shutdown, 0x01);
}

void Rs_DotMatrix::SetIntensity(uint8_t intensity) {
    if(intensity >= 0 && intensity <= 0x0F)
        this->Write(CMD_Intensity, intensity);
    else
        this->Write(CMD_Intensity, 0xFF);
}

uint8_t Rs_DotMatrix::Convert2Idx(char input) {
    switch (input) {
    case 48 ... 57 :
        return input - 48; break;
    case 65 ... 90 :
        return input - 55; break;
    case 97 ... 122 :
        return input - 60; break;
    default :
        return 63; break;
    }
}

void Rs_DotMatrix::Write(uint8_t cmd, uint8_t data) {
    digitalWrite(_CS, LOW);  
    SPI.transfer(cmd);
    SPI.transfer(data);
    digitalWrite(_CS, HIGH);  
}

