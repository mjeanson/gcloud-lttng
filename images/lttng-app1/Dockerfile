# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

FROM lttng-dev:stable-2.13 AS builder
COPY files/app1/* /app1/
RUN cd /app1 && make

# LTTng demo app 1, sends simple events every 5 seconds.
FROM lttng:stable-2.13
COPY --from=builder /app1/app1 /usr/local/bin
CMD ["/usr/local/bin/app1"]
