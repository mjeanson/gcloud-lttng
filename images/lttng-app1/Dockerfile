FROM lttng-dev:stable-2.13
COPY files/app1/* /app1/
RUN cd /app1 && make

FROM lttng:stable-2.13
COPY --from=0 /app1/app1 /usr/local/bin

CMD ["/usr/local/bin/app1"]
