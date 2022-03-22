# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

# Base lttng stable-2.13
FROM lttng:stable-2.13
RUN apt update && apt dist-upgrade -y
RUN apt update
RUN apt install -y -V build-essential liblttng-ust-dev liblttng-ctl-dev
