#!/bin/bash

set -eu

. etc/conf.sh

# CLEANUP
gcloud container clusters delete ${CLUSTER}
gcloud artifacts repositories delete ${DOCKER_REPO} --location ${REGION}
