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

#include "privacy/net/krypton/utils/status.h"

#include "privacy/net/krypton/proto/ppn_status.proto.h"
#include "testing/base/public/gmock.h"
#include "testing/base/public/gunit.h"
#include "third_party/absl/status/status.h"

namespace privacy {
namespace krypton {
namespace utils {
namespace {

using absl::StatusCode;

StatusCode GetStatusCodeForHttpStatus(int code) {
  return GetStatusForHttpStatus(code, "").code();
}

class StatusTest : public ::testing::Test {};

TEST_F(StatusTest, GetStatusCodeForHttpStatus) {
  EXPECT_EQ(StatusCode::kOk, GetStatusCodeForHttpStatus(200));
  EXPECT_EQ(StatusCode::kOk, GetStatusCodeForHttpStatus(201));
  EXPECT_EQ(StatusCode::kInvalidArgument, GetStatusCodeForHttpStatus(400));
  EXPECT_EQ(StatusCode::kUnauthenticated, GetStatusCodeForHttpStatus(401));
  EXPECT_EQ(StatusCode::kPermissionDenied, GetStatusCodeForHttpStatus(403));
  EXPECT_EQ(StatusCode::kNotFound, GetStatusCodeForHttpStatus(404));
  EXPECT_EQ(StatusCode::kAborted, GetStatusCodeForHttpStatus(409));
  EXPECT_EQ(StatusCode::kFailedPrecondition, GetStatusCodeForHttpStatus(412));
  EXPECT_EQ(StatusCode::kResourceExhausted, GetStatusCodeForHttpStatus(429));
  EXPECT_EQ(StatusCode::kCancelled, GetStatusCodeForHttpStatus(499));
  EXPECT_EQ(StatusCode::kInternal, GetStatusCodeForHttpStatus(500));
  EXPECT_EQ(StatusCode::kUnimplemented, GetStatusCodeForHttpStatus(501));
  EXPECT_EQ(StatusCode::kUnavailable, GetStatusCodeForHttpStatus(503));
  EXPECT_EQ(StatusCode::kDeadlineExceeded, GetStatusCodeForHttpStatus(504));
  EXPECT_EQ(StatusCode::kInternal, GetStatusCodeForHttpStatus(505));
  EXPECT_EQ(StatusCode::kUnknown, GetStatusCodeForHttpStatus(600));
}

TEST_F(StatusTest, TestPermanentFailures) {
  EXPECT_TRUE(IsPermanentError(absl::PermissionDeniedError("")));
  EXPECT_FALSE(IsPermanentError(absl::InvalidArgumentError("")));
  EXPECT_FALSE(IsPermanentError(absl::UnauthenticatedError("")));
  EXPECT_FALSE(IsPermanentError(absl::NotFoundError("")));
  EXPECT_FALSE(IsPermanentError(absl::AbortedError("")));
  EXPECT_FALSE(IsPermanentError(absl::FailedPreconditionError("")));
  EXPECT_FALSE(IsPermanentError(absl::ResourceExhaustedError("")));
  EXPECT_FALSE(IsPermanentError(absl::CancelledError("")));
  EXPECT_FALSE(IsPermanentError(absl::InternalError("")));
  EXPECT_FALSE(IsPermanentError(absl::UnimplementedError("")));
  EXPECT_FALSE(IsPermanentError(absl::UnavailableError("")));
  EXPECT_FALSE(IsPermanentError(absl::DeadlineExceededError("")));
}

TEST_F(StatusTest, TestPpnStatusDetailsDefault) {
  auto status = absl::FailedPreconditionError("error");
  PpnStatusDetails details = GetPpnStatusDetails(status);
  EXPECT_EQ(PpnStatusDetails::ERROR_CODE_UNKNOWN,
            details.detailed_error_code());
}

TEST_F(StatusTest, TestPpnStatusDetails) {
  PpnStatusDetails input;
  input.set_detailed_error_code(PpnStatusDetails::DISALLOWED_COUNTRY);
  auto status = absl::FailedPreconditionError("error");
  SetPpnStatusDetails(&status, input);

  PpnStatusDetails details = GetPpnStatusDetails(status);
  EXPECT_EQ(PpnStatusDetails::DISALLOWED_COUNTRY,
            details.detailed_error_code());
}

TEST_F(StatusTest, TestHttpStatus412) {
  absl::Status status = GetStatusForHttpStatus(412, "disallowed country");
  EXPECT_EQ(absl::StatusCode::kFailedPrecondition, status.code());
  EXPECT_EQ("Disallowed country: disallowed country", status.message());

  PpnStatusDetails details = GetPpnStatusDetails(status);
  EXPECT_EQ(PpnStatusDetails::DISALLOWED_COUNTRY,
            details.detailed_error_code());

  EXPECT_TRUE(IsPermanentError(status));
}

}  // anonymous namespace
}  // namespace utils
}  // namespace krypton
}  // namespace privacy
