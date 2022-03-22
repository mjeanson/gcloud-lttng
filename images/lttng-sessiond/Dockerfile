# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

# Sidepod lttng-sessiond
FROM lttng-dev:stable-2.13 AS builder
WORKDIR /lttng/dev/
COPY files/health-check/* ./
RUN gcc -o lttng-health-check health_check.c -llttng-ctl

FROM lttng:stable-2.13
COPY files/configs/* /configs/
COPY files/wait-session-active.sh /usr/local/bin/
COPY --from=builder /lttng/dev/lttng-health-check /usr/local/bin/
CMD ["/usr/bin/lttng-sessiond"]
