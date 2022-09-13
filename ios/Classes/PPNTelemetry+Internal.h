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

#ifdef __cplusplus

#import "googlemac/iPhone/Shared/PPN/API/PPNTelemetry.h"

#include "privacy/net/krypton/proto/krypton_telemetry.proto.h"

NS_ASSUME_NONNULL_BEGIN

@interface PPNTelemetry ()

/**
 * Initializes the class with a KryptonTelemetry.
 */
- (instancetype)initWithKryptonTelemetry:
                    (const privacy::krypton::KryptonTelemetry &)kryptonTelemetry
                           serviceUptime:(NSTimeInterval)serviceUptime
                        connectionUptime:(NSTimeInterval)connectionUptime
                           networkUptime:(NSTimeInterval)networkUptime
                  disconnectionDurations:(NSArray<NSNumber *> *)disconnectionDurations
                      disconnectionCount:(NSInteger)disconnectionCount NS_DESIGNATED_INITIALIZER;

- (instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END

#endif  // __cplusplus
