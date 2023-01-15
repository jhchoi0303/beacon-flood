#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include "radiotap.h"
#include "ieee80211.h"

void usage() {
  printf("syntax: beacon-flood <interface> <ssid-list-file>\n");
  printf("sample: beacon-flood mon0 ssid-list.txt\n");
}

typedef struct {
  char* dev_;
} Param;

Param param = {
  .dev_ = NULL
};

bool parse(Param* param, int argc, char* argv[]) {
  if (argc != 3) {
    usage();
    return false;
  }
  param->dev_ = argv[1];
  return true;
}

int main(int argc, char* argv[]) {
  if (!parse(&param, argc, argv))
    return -1;

  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t* pcap = pcap_open_live(param.dev_, BUFSIZ, 1, 1000, errbuf);
  if (pcap == NULL) {
    fprintf(stderr, "pcap_open_live(%s) return null - %s\n", param.dev_, errbuf);
    return -1;
  }
   
  std::vector<std::string> ssid_list;
  std::vector<mac_address> bssid_list;
  FILE *fp = fopen(argv[2], "r");
  if (fp == NULL) {
    fprintf(stderr, "fopen(%s) return null\n", argv[2]);
    return -1;
  }
  
  char ssid_buffer[256];
  while (fgets(ssid_buffer, 256, fp)) {
    int len = strlen(ssid_buffer);
    ssid_buffer[len - 1] = 0;
    ssid_list.push_back(ssid_buffer);
    uint8_t t = (uint8_t)ssid_list.size();
    mac_address tmp_bssid = {t, t, t, t, t, t};
    bssid_list.push_back(tmp_bssid);
  }
  fclose(fp);
  
  int i = 0;
  uint8_t packet[BUFSIZ];
  
  while (true) {
    uint8_t *packet_ptr = packet;
    
    radiotap_header rt_header;
    
    rt_header.version = 0x00;
    rt_header.pad = 0x00;
    rt_header.len = 0x0008;
    rt_header.present = 0x00000000;
    
    memcpy(packet_ptr, reinterpret_cast<uint8_t*>(&rt_header), sizeof(radiotap_header));
    packet_ptr += sizeof(radiotap_header);
    
    IEEE80211_request_header request_header;
    
    request_header.version = 0x0;
    request_header.type = 0x0;
    request_header.subtype = 0x8; // beacon frame
    request_header.flags = 0x00;
    request_header.duration = 0x0000;
    
    memcpy(packet_ptr, reinterpret_cast<uint8_t*>(&request_header), sizeof(request_header));
    packet_ptr += sizeof(request_header);
    
    IEEE80211_beacon_frame beacon_frame;
    
    beacon_frame.receiver_address = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    beacon_frame.transmitter_address = bssid_list[i];
    beacon_frame.bss_id = bssid_list[i];
    beacon_frame.fragment_number = 0x0;
    beacon_frame.sequence_number = 0x00;
    
    memcpy(packet_ptr, reinterpret_cast<uint8_t*>(&beacon_frame), sizeof(IEEE80211_beacon_frame));
    packet_ptr += sizeof(IEEE80211_beacon_frame);
    
    IEEE80211_wireless_management wireless_management;
    
    wireless_management.timestamp = 0x0000000000000000;
    wireless_management.beacon_interval = 0x0064;
    wireless_management.capabilities_inform = 0x0c11;
    memcpy(packet_ptr, reinterpret_cast<uint8_t*>(&wireless_management), sizeof(IEEE80211_wireless_management));
    packet_ptr += sizeof(IEEE80211_wireless_management);
    
    packet_ptr[0] = IEEE80211_wireless_management::tag_number::SSID_PARAMETER_SET;
    packet_ptr[1] = ssid_list[i].length();
    packet_ptr += 2;
    char tmp[256];
    sprintf(tmp, "%s", ssid_list[i].c_str());
    memcpy(packet_ptr, reinterpret_cast<uint8_t*>(tmp), ssid_list[i].length());
    packet_ptr += ssid_list[i].length();
    
    int send_res = pcap_sendpacket(pcap, packet, packet_ptr - packet);
    usleep(1000);
    if (send_res != 0) {
      fprintf(stderr, "pcap_sendpacket return %d\n", send_res);
      break;
    }
    
    
    i++;
    if (i == (int)ssid_list.size()) {
      i = 0;
    }
  }

  pcap_close(pcap);
}