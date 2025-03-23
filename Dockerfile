FROM gcc:latest

WORKDIR /app

COPY src/ /app/

RUN make

EXPOSE 8080

CMD ["./server"]

