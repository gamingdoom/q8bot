#include <cstdint>
#include <memory>

/* A message should be sent in this format:

  uint8_t        uint8_t                              float[] or int32_t[]
----------------------------------------------------------------------------
| Command Type | Payload Length (number of elements) |       Payload       |
----------------------------------------------------------------------------

*/

enum CommandType {
  MOVE,
  BATTERY,
  RECORD,
  SEND_RECORDED,
  JUMP,
  SET_TORQUE,
  SET_PROFILE,
  SET_GAIT,
};

union MessagePayload_t {
  int32_t ints;
};

class Message {
  public:
    Message(unsigned char* data);
    ~Message() = default;

    CommandType GetCommand();

    uint8_t GetPayloadLen();
    int32_t *GetPayloadInts();

  private:
    CommandType cmd;
    uint8_t payload_len;
    std::shared_ptr<MessagePayload_t[]> payload;
};
