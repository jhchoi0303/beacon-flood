struct radiotap_header {
  uint8_t version;
  uint8_t pad;
  uint16_t len;
  uint32_t present;
};

enum radiotap_present_flags {
  TSFT,
  FLAGS,
  RATE,
  CHANNEL,
  FHSS,
  DBM_SIGNAL,
  DBM_NOISE,
  LOCK_QUALITY,
  TX_ATTENUATION,
  DB_TX_ATTENUATION,
  DBM_TX_ATTENUATION,
  ANTENNA,
  DB_SIGNAL,
  DB_NOISE,
  RX_FLAGS,
  TX_FLAGS,
  DATA_RETRIES,
  XCHANNEL,
  CHANNEL_PLUS,
  MCS,
};