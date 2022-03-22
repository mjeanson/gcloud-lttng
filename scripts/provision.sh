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

echo_green "Set defaults for the gcloud CLI"
gcloud config set project $PROJECT_ID
gcloud config set compute/region $REGION

echo_green "Enable kubernetes and the docker registry on the gcloud project"
gcloud services enable artifactregistry.googleapis.com container.googleapis.com

echo_green "Create the docker repo to host container images"
gcloud artifacts repositories create ${DOCKER_REPO} \
   --repository-format=docker \
   --location=$REGION \
   --description="Docker repository"

echo_green "Configure local docker to push to the remote repo"
gcloud auth configure-docker ${REGION}-docker.pkg.dev

echo_green "Create kubernetes cluster"
gcloud container clusters create-auto ${CLUSTER}
gcloud container clusters get-credentials ${CLUSTER}
