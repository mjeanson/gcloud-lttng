#!/bin/bash

# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

set -eu

. etc/conf.sh

# Generate pods yaml files
make pods

kubectl create -f configmaps/envmap.yaml
kubectl create -f pods/demo1.yaml
