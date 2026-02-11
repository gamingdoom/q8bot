#include <Arduino.h>
#include <esp_now.h>
#include <MAX17043.h>

#include "helpers.h"

void queuePrint(SerialMsgType type, const char* format, ...) {
  SerialMessage msg;
  msg.type = type;
  va_list args;
  va_start(args, format);
  vsnprintf(msg.text, sizeof(msg.text), format, args);
  va_end(args);
  xQueueSend(debugQueue, &msg, 0);  // Non-blocking
}

bool addPeer(const uint8_t* mac) {
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, mac, 6);
  peer.channel = chan;
  peer.encrypt = false;
  return esp_now_add_peer(&peer) == ESP_OK;
}

void unpair() {
  queuePrint(MSG_INFO, "[HEARTBEAT] Connection lost - returning to pairing mode\n");

  esp_now_del_peer(clientMac);
  storage.clearPeerMAC();
  queuePrint(MSG_DEBUG, "[STORAGE] Cleared controller MAC from EEPROM\n");
  memset(clientMac, 0, sizeof(clientMac));
  paired = false;
  started = false;

  // Update robot state and event group
  robotState = STATE_UNPAIRED;
  xEventGroupClearBits(eventGroup, EVENT_PAIRED | EVENT_STARTED);
  xEventGroupSetBits(eventGroup, EVENT_UNPAIRED);
}

void addElementToArray(uint16_t*& array, size_t& currentSize, uint16_t newElement) {
    // Allocate a new array with one extra element
    uint16_t* newArray = new uint16_t[currentSize + 1];
    newArray[0] = newElement;
    memcpy(newArray + 1, array, currentSize * sizeof(uint16_t));
    // Delete the old array (free the memory)
    delete[] array;
    // Update the original pointer and size
    array = newArray;
    currentSize++;
}

MAX1704X get_fuel_gauge() {
  return FuelGauge;
}