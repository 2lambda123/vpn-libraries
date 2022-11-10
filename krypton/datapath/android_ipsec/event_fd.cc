// Copyright 2020 Google LLC
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

#include "privacy/net/krypton/datapath/android_ipsec/event_fd.h"

#include <sys/eventfd.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <string>

#include "third_party/absl/log/check.h"
#include "third_party/absl/status/status.h"
#include "third_party/absl/strings/substitute.h"

namespace privacy {
namespace krypton {
namespace datapath {
namespace android {

EventFd::EventFd() {
  int fd = eventfd(0, EFD_NONBLOCK);
  PCHECK(fd != -1) << "Unable to create eventfd";
  epoll_fd_ = fd;
}

EventFd::~EventFd() { close(epoll_fd_); }

int EventFd::fd() const { return epoll_fd_; }

absl::Status EventFd::Notify(uint64_t value) {
  const int ret = write(epoll_fd_, &value, sizeof(value));

  if (ret == -1) {
    return absl::InternalError(
        absl::Substitute("Failed to notify event fd: $0, reason: $1", epoll_fd_,
                         strerror(errno)));
  }
  return absl::OkStatus();
}

}  // namespace android
}  // namespace datapath
}  // namespace krypton
}  // namespace privacy
