# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

FROM lttng-dev:stable-2.13 AS builder
COPY files/lttng-notif-client/* /lttng-notif-client/
RUN cd /lttng-notif-client && make

# Notification client that archives trace chunks on rotation completed
FROM lttng:stable-2.13
COPY --from=builder /lttng-notif-client/lttng-notif-client /usr/local/bin
COPY --from=builder /lttng-notif-client/lttng-archive-chunk /usr/local/bin
CMD ["/usr/local/bin/lttng-notif-client"]
