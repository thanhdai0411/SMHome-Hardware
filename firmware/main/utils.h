
String calculateCRC16(String data) {
  uint16_t crc = 0xFFFF;
  for (int i = 0; i < data.length(); i++) {
    crc ^= (uint8_t)data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  char crcStr[5];
  sprintf(crcStr, "%04X", crc); // Chuyển CRC thành chuỗi hex
  return String(crcStr);
}

char* buildTopicPubDevice(int index) {
  // Cấp phát bộ nhớ động để chứa chuỗi
  char* topic = (char*)malloc(100 * sizeof(char));
  if (topic == NULL) {
    return NULL; // Kiểm tra lỗi cấp phát bộ nhớ
  }

  // Tạo chuỗi topic
  snprintf(topic, 100, "/%s/%s/SMH_DEVICE%d/button", SMH_ROOT, SMH_NODE_ID, index + 1);
  return topic;
}

char* buildDeviceIdForDevice(int index) {

  switch (index)
  {
    case 0:
      return SMH_DEVICE1_ID;
    case 1:
      return SMH_DEVICE2_ID;
    case 2:
      return SMH_DEVICE3_ID;
    case 3:
      return SMH_DEVICE4_ID;
    case 4:
      return SMH_DEVICE5_ID;
    default:
      break;
  }
  return SMH_DEVICE1_ID;


}

const char* buildFrameTrans(String deviceID, String dataSent ) {
  String frameData = deviceID + "," + dataSent;
  String crc = calculateCRC16(frameData);
  String frame = (String)START_FRAME +  "," + frameData + "," + crc + "," + (String)END_FRAME;
  return frame.c_str();
}
