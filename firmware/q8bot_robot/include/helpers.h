#ifndef HELPERS_H
#define HELPERS_H

#include "MAX1704X.h"

#include "systemParams.h"

#include "macStorage.h"

static macStorage storage;
static bool started = false;  // Track robot start state

void queuePrint(SerialMsgType type, const char* format, ...);
bool addPeer(const uint8_t* mac);
void unpair();
void displayReading();
void addElementToArray(uint16_t*& array, size_t& currentSize, uint16_t newElement);

MAX1704X get_fuel_gauge();

#endif