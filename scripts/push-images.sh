#!/bin/bash

# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

set -eu

scriptdir="$(dirname "${BASH_SOURCE[0]}")"
basedir=$(readlink -f "$scriptdir/..")

# shellcheck source=etc/conf.sh
. "$basedir/etc/conf.sh"

build_image() {
	local image_name=$1
	local image_tag=$2
	local image_repo=${3:-}

	if [ "${image_repo}" != "" ]; then
		image_repo="${image_repo}/"
	fi

	pushd "images/${image_name}"
	docker build -t "${image_repo}${image_name}:${image_tag}" .
	popd
}

build_project_image() {

	local image_name=$1
	local image_tag=$2

	build_image "${image_name}" "${image_tag}" "${REGION}-docker.pkg.dev/${PROJECT_ID}/${DOCKER_REPO}"
}

push_project_image() {
	local image_name=$1
	local image_tag=$2

	docker push "${REGION}-docker.pkg.dev/${PROJECT_ID}/${DOCKER_REPO}/${image_name}:${image_tag}"
}

# Build base lttng docker images
build_image lttng stable-2.13
build_image lttng-dev stable-2.13

# Build cluster images
build_project_image lttng-sessiond v1
build_project_image lttng-relayd v1
build_project_image lttng-app1 v1
build_project_image lttng-app2 v1

# Push sessiond image to repo
push_project_image lttng-sessiond v1
push_project_image lttng-relayd v1
push_project_image lttng-app1 v1
push_project_image lttng-app2 v1
