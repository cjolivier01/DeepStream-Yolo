#!/bin/bash

# Copyright 2024 The Bazel Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"

source "bazel.sh"

# The expected files are generated by:
# ../generate.py
# jq 'map(.directory = "EXECROOT")' compile_commands.json > ${expected}
if [[ "$(uname -s)" == "Darwin" ]]; then
  expected="expected_macos.json"
  expected_ycm="expected_ycm_macos.json"
else
  expected="expected_ubuntu.json"
  expected_ycm="expected_ycm_ubuntu.json"
fi

execroot="$(bazel info execution_root)"
readonly execroot

check_compdb() {
  local want="$expected"
  local got="$1"
  local full_compdb="$2"

  jq_got_cmd='sort_by(.file)'
  jq_want_cmd="${jq_got_cmd}"
  jq_want_cmd+=" | map(.directory = \"${execroot}\")"
  if ! "${full_compdb}"; then
    # Remove the source file for the target not included in our //:compdb target.
    jq_want_cmd+=" | map(. | select(.file != \"stdlib.cc\"))"
  fi

  # Check jq is installed.
  if ! command -v jq >/dev/null; then
    >&2 echo "jq not installed; aborting."
    exit 1
  fi

  diff --unified=100 <(jq "${jq_want_cmd}" "${want}") <(jq "${jq_got_cmd}" "${got}")
  test -f bazel-bin/_virtual_includes/a/dir_a/a.h
}

# Let's keep the C++ toolchain simple by not using Xcode specific bazel wrappers.
# https://cs.opensource.google/bazel/bazel/+/master:tools/cpp/cc_configure.bzl;l=122;drc=644b7d41748e09eff9e47cbab2be2263bb71f29a
# Also, tell xcode-select to always use CommandLineTools, whether or not Xcode is installed.
export BAZEL_USE_CPP_ONLY_TOOLCHAIN=1
export DEVELOPER_DIR=/Library/Developer/CommandLineTools
"${bazel}" sync --configure # Reset the cached toolchain.

echo
echo "Checking specific targets mode"
"${bazel}" clean
"${bazel}" build :compdb
check_compdb bazel-bin/compile_commands.json false
echo "SUCCESS!"

echo
echo "Checking full repo mode"
"${bazel}" clean
../generate.py
check_compdb compile_commands.json true
echo "SUCCESS!"

echo
echo "Checking YCM plugin"
diff --unified=100 \
  <(sed -e "s@EXECROOT@${execroot}@" -e "s@PWD@${PWD}@" "${expected_ycm}") \
  <(python3 ../.ycm_extra_conf.py a.cc)
echo "SUCCESS!"
