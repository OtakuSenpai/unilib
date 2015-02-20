// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014 Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// UniLib version: $UNILIB_VERSION
// Unicode version: $UNICODE_VERSION

#pragma once

#include <string>

namespace ufal {
namespace unilib {

struct version {
  unsigned major;
  unsigned minor;
  unsigned patch;
  std::string prerelease;

  // Returns current version.
  static version current() {
    return {$UNILIB_MAJOR_VERSION, $UNILIB_MINOR_VERSION, $UNILIB_PATCH_VERSION, $UNILIB_PRERELEASE_VERSION};
  }
};

} // namespace unilib
} // namespace ufal