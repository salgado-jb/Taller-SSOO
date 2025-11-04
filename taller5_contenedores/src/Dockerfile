FROM ubuntu AS external
# COMPLETAR
RUN mkdir /app
COPY ./externalServer /app/
EXPOSE 8080
CMD ["./app/externalServer"]


FROM ubuntu AS local
# COMPLETAR
RUN mkdir /app
COPY ./localServer /app/
COPY ./localClient /app/
CMD ["./app/localServer"]