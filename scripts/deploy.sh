#!/bin/bash

# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

set -eu

scriptdir="$(dirname "${BASH_SOURCE[0]}")"
basedir=$(readlink -f "$scriptdir/..")

# shellcheck source=etc/conf.sh
. "$basedir/etc/conf.sh"

# shellcheck source=scripts/common.sh
. "$basedir/scripts/common.sh"

echo_green "Generate pods yaml files"
make pods

echo_green "Provision kubernetes resources"
kubectl create -f pods/demo1.yaml
