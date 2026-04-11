// Minimal Arduino.h mock for native testing
#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Include button IDs and defines
#define RGREEN  1
#define RRED    2
#define RBLUE   3
#define RBLACK  4
#define RUP     5
#define RDOWN   6
#define RRIGHT  7
#define RLEFT   8
#define RPUSH   9
#define LRED    10
#define LYELLOW 11
#define LWHITE  12

#define POT_MIN     0
#define POT_MAX     127

// map function from Arduino
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Minimal String class mock
class String {
private:
    char* buffer;
    size_t len;
public:
    String() { buffer = new char[1]; buffer[0] = '\0'; len = 0; }
    String(const char* s) { len = strlen(s); buffer = new char[len+1]; strcpy(buffer, s); }
    String(const String& s) { len = s.len; buffer = new char[len+1]; strcpy(buffer, s.buffer); }
    ~String() { delete[] buffer; }
    short toInt() const { return (short)atoi(buffer); }
    int compareTo(const String& s) const { return strcmp(buffer, s.buffer); }
    const char* c_str() const { return buffer; }
};

// Mock Serial global
class String Serial;

// Arduino functions
void delay(int ms) {}
void pinMode(int pin, int mode) {}
void digitalWrite(int pin, int value) {}
int digitalRead(int pin) { return 0; }
int analogRead(int pin) { return 0; }
void analogWrite(int pin, int value) {}

#endif // ARDUINO_H