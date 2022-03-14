FROM lttng-dev:stable-2.13
COPY files/app2/* /app2/
RUN cd /app2 && make

FROM lttng:stable-2.13
COPY --from=0 /app2/app2 /usr/local/bin

CMD ["/usr/local/bin/app2"]
