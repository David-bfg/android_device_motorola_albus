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
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

#include "KeySwapper.h"

namespace {
constexpr const char kControlPath[] = "/system/vendor/usr/keylayout/uinput-fpc.kl";
};  // anonymous namespace

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

    std::ifstream input_file(kControlPath);
    std::string line;
    const std::regex default_pattern_app_switch(R"(key\s+620\s+APP_SWITCH\s+VIRTUAL)");
    const std::regex default_pattern_back(R"(key\s+621\s+BACK\s+VIRTUAL)");

    // Read the lines from the input file into a vector
    while (std::getline(input_file, line)) {
        if (std::regex_search(line, default_pattern_app_switch) || 
                std::regex_search(line, default_pattern_back)) {
            input_file.close();
            return false;
        }
    }
    input_file.close();

    return true;
}

Return<bool> KeySwapper::setEnabled(bool enabled) {
    if (!mHasKeySwapper) return false;

    std::ifstream input_file(kControlPath);
    std::vector<std::string> lines;
    std::string line;
    const std::regex pattern(R"(key\s+62[01])");

    // Read the lines from the input file into a vector
    while (std::getline(input_file, line)) {
        if (!std::regex_search(line, pattern)) {
            lines.push_back(line);
        }
    }
    input_file.close();

    // write keys swapping positions
    lines.push_back(std::string("key ") + (enabled ? "621" : "620") + "   APP_SWITCH   VIRTUAL");
    lines.push_back(std::string("key ") + (enabled ? "620" : "621") + "   BACK         VIRTUAL");
    
    // Write the updated lines to the input file
    std::ofstream output_file(kControlPath);
    for (const auto& line : lines) {
        output_file << line << std::endl;
    }
    output_file.close();

    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
