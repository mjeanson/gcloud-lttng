#!/bin/bash

# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

set -eu

. etc/conf.sh

# Set defaults for the CLI
gcloud config set project $PROJECT_ID
gcloud config set compute/region $REGION

# Enable kubernetes and the docker registry on the gcloud project
gcloud services enable artifactregistry.googleapis.com container.googleapis.com

# Create the docker repo to host container images
gcloud artifacts repositories create ${DOCKER_REPO} \
   --repository-format=docker \
   --location=$REGION \
   --description="Docker repository"

# Configure local docker to push to the remote repo
gcloud auth configure-docker ${REGION}-docker.pkg.dev

# Create kubernetes cluster
gcloud container clusters create-auto ${CLUSTER}
gcloud container clusters get-credentials ${CLUSTER}
