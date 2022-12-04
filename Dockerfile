FROM golang:1.19.1-alpine as build
RUN apk add make gcc musl-dev
RUN mkdir -p /app/src
WORKDIR /app/src

COPY go.mod . 
COPY go.sum . 
RUN go mod download
COPY . .
RUN make build

FROM alpine:3.7
COPY --from=build /app/src/main /root/main
COPY config.yaml /root/config.yaml

WORKDIR /root/

CMD ["./main"]
