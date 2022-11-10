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

#import <Foundation/Foundation.h>

#import "googlemac/iPhone/Shared/PPN/Classes/PPNUptimeTracker.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Utility for tracking the uptime of a particular metric over an interval.
 */
@interface PPNUptimeDurationTracker : NSObject

/**
 * Initiates PPNUptimeDurationTracker with a PPN clock.
 */
- (instancetype)initWithClock:(PPNClock *)clock NS_DESIGNATED_INITIALIZER;

- (instancetype)init NS_UNAVAILABLE;

/**
 * Sets the uptime metric to be "started".
 */
- (void)start;

/**
 * Sets the uptime metric to be stopped。
 */
- (void)stop;

/**
 * Returns the List of uptimeDurations and resets list's size to zero.
 */
- (NSArray<NSNumber *> *)collectDurations;

@end

NS_ASSUME_NONNULL_END
