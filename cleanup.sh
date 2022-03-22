#!/bin/bash

# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

set -eu

. etc/conf.sh

# CLEANUP
gcloud container clusters delete ${CLUSTER}
gcloud artifacts repositories delete ${DOCKER_REPO} --location ${REGION}
