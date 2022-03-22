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

echo_green "Deprovision gcloud resources"
gcloud container clusters delete ${CLUSTER}
gcloud artifacts repositories delete ${DOCKER_REPO} --location ${REGION}
