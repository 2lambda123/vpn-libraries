// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PRIVACY_NET_KRYPTON_DATAPATH_IPSEC_IPSEC_PACKET_H_
#define PRIVACY_NET_KRYPTON_DATAPATH_IPSEC_IPSEC_PACKET_H_

#include <cstddef>
#include <memory>
#include <string>

#include "base/logging.h"
#include "privacy/net/krypton/datapath/ipsec/ipsec.h"
#include "privacy/net/krypton/pal/packet.h"

namespace privacy {
namespace krypton {
namespace datapath {
namespace ipsec {

/* The maximum payload size of an IPsec packet.
 *
 * Working backwards from a 1500 byte MTU, we have:
 *  Headers:
 *   20/40 bytes of IP header
 *   8 byte UDP header
 *   12 byte ESP header
 *   8 byte Initialization Vector
 *  Ciphertext:
 *   Payload
 *   0-4 bytes of padding
 *   2 byte Pad Length
 *   2 byte Next Header
 *  Trailers:
 *   16 byte ICV
 * Assuming that we add minimal padding to only align the data section, we have:
 * 1500 - (40 + 8 + 12 + 8 + 4 + 2 + 2 + 16) = 1408.
 */
constexpr size_t kMaxIpsecDataSize = 1408;

class IpSecPacket {
 public:
  /** Constructs an empty IPsec packet. */
  IpSecPacket() : data_size_(0) {}

  // Disallow copy and assign.
  IpSecPacket(const IpSecPacket &) = delete;
  IpSecPacket(IpSecPacket &&) = delete;
  IpSecPacket &operator=(const IpSecPacket &) = delete;
  IpSecPacket &operator=(IpSecPacket &&) = delete;

  /** Returns the size of this packet's header. */
  size_t header_size() const { return sizeof(EspHeader); }

  /** Returns a pointer to the header section of this packet. */
  EspHeader *header() { return reinterpret_cast<EspHeader *>(buffer_); }

  /** Returns a pointer to the header section of this packet. */
  const EspHeader *header() const {
    return reinterpret_cast<const EspHeader *>(buffer_);
  }

  /** Returns the current size of the data section of this packet. */
  size_t data_size() const { return data_size_; }

  /** Returns the maximum amount of data that can be stored in this packet. */
  size_t max_data_size() const { return kMaxIpsecDataSize; }

  /** Returns a pointer to the data section of this packet. */
  char *data() { return buffer_ + sizeof(EspHeader); }

  /** Returns a pointer to the data section of this packet. */
  const char *data() const { return buffer_ + sizeof(EspHeader); }

  /**
   * Returns the IP protocol(V4, V6, etc.) of this packet.
   *
   * This method only works for inbound packets after decryption.
   */
  IPProtocol GetIPProtocol() const;

  /**
   * Changes the size of the data section of the packet to the given value.
   * This should be called to set the size after the data in the buffer has been
   * altered.
   */
  void resize_data(size_t new_data_size) {
    CHECK_LE(new_data_size, kMaxIpsecDataSize);
    data_size_ = new_data_size;
  }

  /**
   * Returns the size of the used section of the underlying buffer, including
   * the header and data.
   */
  size_t buffer_size() { return header_size() + data_size(); }

  /** Returns a pointer to the underlying buffer. */
  char *buffer() { return buffer_; }

  // Returns a debug string containing information about the packet. In
  // non-debug mode, the string contains up to 'max_packet_content_bytes'
  // of the actual packet contents. In debug mode or if
  // 'max_packet_content_bytes' is less than zero, the first
  // size() packet content bytes are included in the string.
  std::string GetDebugString(size_t max_packet_content_bytes) const;

 private:
  // The complete data for the packet, including header.
  char buffer_[sizeof(EspHeader) + kMaxIpsecDataSize];
  size_t data_size_;
};

}  // namespace ipsec
}  // namespace datapath
}  // namespace krypton
}  // namespace privacy

#endif  // PRIVACY_NET_KRYPTON_DATAPATH_IPSEC_IPSEC_PACKET_H_
