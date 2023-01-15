struct IEEE80211_request_header {
  uint8_t version:2;
  uint8_t type:2;
  uint8_t subtype:4;
  uint8_t flags;
  uint16_t duration;
};

struct mac_address {
  uint8_t addr[6];
  
  mac_address& operator = (const mac_address &rhs) {
    for (int i = 0; i < 6; i++) {
      addr[i] = rhs.addr[i];
    }
    return *this;
  }
};

struct IEEE80211_beacon_frame {
  mac_address receiver_address;
  mac_address transmitter_address;
  mac_address bss_id;
  uint8_t fragment_number:4;
  uint16_t sequence_number:12;
};

struct IEEE80211_wireless_management {
  uint64_t timestamp;
  uint16_t beacon_interval;
  uint16_t capabilities_inform;
  
  enum tag_number {
        SSID_PARAMETER_SET = 0,
        SUPPORTED_RATES = 1,
        FH_PARAMETER_SET = 2,
        DS_PARAMETER_SET = 3,
        CF_PARAMETER_SET = 4,
        TRAFFIC_INDICATION_MAP = 5,
        IBSS_PARAMETER_SET = 6,
        COUNTRY = 7,
        HOPPING_PATTERN_PARAMETERS = 8,
        HOPPING_PATTERN_TABLE = 9,
        REQUEST = 10,
        CHALLENGE_TEXT = 16,
        POWER_CONSTRAINT = 32,
        POWER_CAPABILITY = 33,
        TPC_REQUEST = 34,
        TPC_REPORT = 35,
        SUPPORTED_CHANNELS = 36,
        CHANNEL_SWITCH_ANNOUNCEMENT = 37,
        MEASUREMENT_REQUEST = 38,
        MEASUREMENT_REPORT = 39,
        QUIET = 40,
        IBSS_DFS = 41,
        ERP_INFORMATION = 42,
        ROBUST_SECURITY_NETWORK = 48,
        EXTENDED_SUPPORTED_RATES = 50,
        WIFI_PROTECTED_ACCESS = 221
    };
};