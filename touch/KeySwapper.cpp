/*
 * Copyright (C) 2019 The LineageOS Project
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

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "KeySwapper.h"

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

KeySwapper::KeySwapper() {
    mHasKeySwapper = false;

    mFingerprintNavigation = IFingerprintNavigation::getService();
    if (mFingerprintNavigation != nullptr)
        mHasKeySwapper = !access(kControlPath, R_OK | W_OK);
}

// Methods from ::vendor::lineage::touch::V1_0::KeySwapper follow.
Return<bool> KeySwapper::isEnabled() {
    if (!mHasKeySwapper) return false;

    std::string buf;
    mFingerprintNavigation->getNavigationConfig(&buf);// defined as passing std::__1::function<void (com::fingerprints::extension::V1_0::NavigationConfig const&)>
                                                        // also refered to elsewhere as std::__1::function<void (android::hardware::Parcel&)>

    return std::stoi(android::base::Trim(buf)) == 1;
}

Return<bool> KeySwapper::setEnabled(bool enabled) {
    if (!mHasKeySwapper) return false;

    std::string buf;
    mFingerprintNavigation->setNavigationConfig(&buf);// defined as passing com::fingerprints::extension::V1_0::NavigationConfig const&
                                                        // also refered to elsewhere as std::__1::function<void (android::hardware::Parcel&)>

    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
