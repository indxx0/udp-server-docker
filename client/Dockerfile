# Этап сборки
FROM gcc:latest AS builder
WORKDIR /app
COPY client.cpp .
RUN g++ -O2 -static client.cpp -o client

FROM alpine:latest
WORKDIR /app
COPY --from=builder /app/client .
CMD ["./client"]