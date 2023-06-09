/*
 * Copyright (C) 2021 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "googlemac/iPhone/Shared/PPN/Classes/PPNStatusDetails+Internal.h"
#import "privacy/net/common/proto/ppn_status.proto.h"

static NSString *const kDetailedErrorCode = @"detailedErrorCode";

@implementation PPNStatusDetails

- (instancetype)initWithPpnStatusDetails:(const privacy::ppn::PpnStatusDetails &)ppnStatusDetails {
  self = [super init];
  if (self != nullptr) {
    _detailedErrorCode = PPNDetailedErrorCode(ppnStatusDetails.detailed_error_code());
  }
  return self;
}

- (NSString *)description {
  return
      [[NSString alloc] initWithFormat:@"<%@: %p; detailedErrorCode:%lu>",
                                       NSStringFromClass([self class]), self, _detailedErrorCode];
}

#pragma mark - NSSecureCoding

- (instancetype)initWithCoder:(NSCoder *)coder {
  privacy::ppn::PpnStatusDetails ppnStatusDetails;
  ppnStatusDetails.set_detailed_error_code(privacy::ppn::PpnStatusDetails_DetailedErrorCode(
      [coder decodeIntegerForKey:kDetailedErrorCode]));
  return [self initWithPpnStatusDetails:ppnStatusDetails];
}

- (void)encodeWithCoder:(NSCoder *)coder {
  [coder encodeInteger:_detailedErrorCode forKey:kDetailedErrorCode];
}

+ (BOOL)supportsSecureCoding {
  return YES;
}

@end
