# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

FROM lttng-dev:stable-2.13 AS builder
COPY files/app2/* /app2/
RUN cd /app2 && make

# LTTng demo app 2, sends simple events every 5 seconds.
FROM lttng:stable-2.13
COPY --from=builder /app2/app2 /usr/local/bin
CMD ["/usr/local/bin/app2"]
